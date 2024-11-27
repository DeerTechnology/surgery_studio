#include "canbus/canhandler.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
canHandler::canHandler(QObject *parent):QObject (parent)
{
    SerialPortNow = new QSerialPort();
}

canHandler::~canHandler()
{

}

bool canHandler::init()
{
    bool res = true;
//    if (QCanBus::instance()->plugins().contains(QStringLiteral("socketcan")))
//    {
//        qDebug()<<__FILE__<<__LINE__<<"拥有socketcan";
//    }
//    else
//    {
//        qDebug()<<__FILE__<<__LINE__<<"没有socketcan";
//    }
//    canDevice = QCanBus::instance()->createDevice("socketcan","can0");
//    if(!canDevice)
//    {
//        qDebug()<<__FILE__<<__LINE__<<"创建can 设备失败";
//        return false;
//    }
//    connect(&timer_handler, &QTimer::timeout, this, &canHandler::task_handler);
//    timer_handler.start(0);
//    connect(canDevice, &QCanBusDevice::framesReceived, this, &canHandler::read_frame);
//    connect(canDevice, &QCanBusDevice::errorOccurred, this, &canHandler::can_erro);
//    res = canDevice->connectDevice();
    connect(&timer_handler, &QTimer::timeout, this, &canHandler::task_handler);
    timer_handler.start(10);
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    QFile serial_file("./serial.txt");
    serial_file.open(QIODevice::ReadWrite);
    QString serialName = serial_file.read(10);
    qDebug()<<__FILE__<<__LINE__<<"文件："<<serialName;
// 如果com.txt不存在或未连接成功，则自动连接

    qDebug()<<__FILE__<<__LINE__<<"串口数量："<<infos.size();

    for (int i = 0; i < infos.size(); i++)
    {
        QSerialPortInfo info = infos.at(i);
        qDebug()<<__FILE__<<__LINE__<<"串口："<<info.portName();
        if(serialName == info.portName())
        {
            this->SerialPortNow->setPort(info);
            this->SerialPortNow->setBaudRate(QSerialPort::Baud115200);
            this->SerialPortNow->setDataBits(QSerialPort::Data8);
            this->SerialPortNow->setParity(QSerialPort::NoParity);
            this->SerialPortNow->setStopBits(QSerialPort::OneStop);
            this->SerialPortNow->setFlowControl(QSerialPort::NoFlowControl);
            if (this->SerialPortNow->open(QIODevice::ReadWrite))
            {
                connect(this->SerialPortNow, SIGNAL(readyRead()), this, SLOT(read_data()));
                qDebug()<<__FILE__<<__LINE__<<"打开串口成功!";
            }
            else
            {
                res = false;
                qDebug()<<__FILE__<<__LINE__<<"打开串口失败";
            }
        }

    }
    serial_file.close();
    return res;
}


void canHandler::read_frame()
{
//    QVector<QCanBusFrame> frames = canDevice->readAllFrames();
//    foreach(QCanBusFrame frame, frames)
//    {
//        m_frames.append(frame);
//    }
    status = true;
}

void canHandler::task_handler()
{
    if(m_frames.size()  < 1)
    {
//        qDebug()<<__FILE__<<__LINE__<<"无数据";
        return;
    }
    deviceInfo *current_device = nullptr;
    QCanBusFrame frame = m_frames.first();
    m_frames.pop_front();
//    qDebug()<<__FILE__<<__LINE__<<frame.toString();
    int id = frame.frameId();
//    qDebug()<<__FILE__<<__LINE__<<id;
    if(id >= 1024 && id <= 2047)
    {
        //上线帧
        int canid = devices.size() * 2 + 21;
        QCanBusFrame send_frame = onLine_init(frame.payload(), canid);
//        qDebug()<<__FILE__<<__LINE__<<send_frame.toString();
        QString str = frame_to_str(send_frame);
        QByteArray test = QByteArray::fromHex(str.toUtf8());

//        qDebug()<<__FILE__<<__LINE__<<test.toHex();

        SerialPortNow->write(QByteArray::fromHex(str.toUtf8()));
    }
    else
    {
        foreach(deviceInfo *device, devices)
        {
            if(device->Can_id == id)
            {
                current_device = device;
//                qDebug()<<__FILE__<<__LINE__<<"找到设备："<< current_device->Can_id;
                break;
            }
        }
        if(current_device == nullptr)
        {
//            qDebug()<<__FILE__<<__LINE__<<"未找到设备";
            QByteArray array = frame.payload();
            if(array.size() == 8 && array.at(1) == SlaveOnlineCheck)
            {
                current_device = new deviceInfo;
                current_device->device_type = DeviceType(array[3] & 0xFF);
                current_device->Can_id = id;
                current_device->count = 0;
                current_device->message.clear();
                current_device->device_id = array[4] * 16 * 16 + array[5];
                devices.append(current_device);
            }
            else
                return ;
        }
        //数据帧
        QByteArray array = frame_to_arry(frame , &current_device->count);
        current_device->message.append(array);
        if(current_device->message.size() >= current_device->count * 2)
        {
            current_device->message = "FEFE" + current_device->message + "FFFF";
            emit recv_frame(current_device->message.toUtf8(), current_device->device_type, current_device->device_id);
            current_device->message.clear();
            current_device->count = 0;
        }
    }
}

void canHandler::can_erro( QCanBusDevice::CanBusError error)
{
    emit recv_frame("错误", DeviceType(2), error);
}

//上线处理
QCanBusFrame canHandler::onLine_init(QByteArray arry, int canid)
{
    deviceInfo *current_device = nullptr;
    foreach(deviceInfo* device, devices)
    {
        if(device->device_type == DeviceType(arry[3] & 0xFF))
        {
            current_device = device;
        }
    }
    if(current_device == nullptr)
    {
        current_device = new deviceInfo;
        current_device->device_type = DeviceType(arry[3] & 0xFF);
        current_device->Can_id = canid;
        devices.append(current_device);
    }
    current_device->count = 0;
    current_device->message.clear();
    current_device->device_id = arry[4] * 16 * 16 + arry[5];

    QString bytes = "070706";
    bytes += arry.mid(3,3).toHex();
    QString canID_str = QString::number(current_device->Can_id, 16);
    int size = canID_str.size();
    for (int i = 0; i < 4 - size; i++)
    {
        canID_str = "0" + canID_str;
    }
    bytes += canID_str;
//    emit recv_frame(current_device->message.toUtf8(), current_device->device_type, current_device->device_id);
    QCanBusFrame frame;
    frame.setFrameId(20);
    frame.setPayload(QByteArray::fromHex(bytes.toUtf8()));
    return frame;
}


//临时中转
QString canHandler::frame_to_str(QCanBusFrame frame)
{
    QString res;
    uint32_t id = frame.frameId();
    QString id_str = QString::number(id, 16);
    id_str = add_bytes(true, id_str, "0", 4);
    QString data_str = frame.payload().toHex();
    res = "FE"+id_str + data_str + "FF";
    return res;
}

QString canHandler::add_bytes(bool statu, QString array, QString byte, int tail)
{
    int i = array.size();
    for (;i < tail; i++)
    {
        if(statu)
        {
            array = byte + array;
        }
        else
        {
            array =  array + byte ;
        }
    }
    return array;
}


QByteArray buffer;
void canHandler::read_data()
{
    QByteArray data;
    data = SerialPortNow->readAll();
//    qDebug()<<__FILE__<<__LINE__<<data.toHex();

    if (!data.isEmpty())
    {
        // qDebug()<<data;
        foreach (char b, data)
        {
            buffer.append(b);

            if(buffer.size() == 12)
            {
                if((buffer.at(0) & 0xFF) == 0xFE && (b & 0xFF) == 0xFF )
                {
//                    qDebug()<<__FILE__<<__LINE__<<buffer.toHex();
    //                QString strNow = QString::fromUtf8(buffer.constData());
                    m_frames.append(str_to_frame(buffer));
                }
                buffer.clear();
            }
        }
    }
}

QCanBusFrame canHandler::str_to_frame(QByteArray str)
{
    QCanBusFrame frame;
    str = str.mid(1, str.size() - 2);
//    qDebug()<<__FILE__<<__LINE__<<str.toHex();

    frame.setFrameId(*(uint16_t*)(str.mid(1,1)+str.mid(0,1)).constData());

    frame.setPayload(str.mid(2));
    return frame;
}

void canHandler::online_check()
{
    QCanBusFrame frame;
    frame.setFrameId(1);
    frame.setPayload(QByteArray::fromHex("AAAAAAAAAAAAAAAA"));
    QString data = frame_to_str(frame);
    QByteArray send_array = QByteArray::fromHex(data.toUtf8());
    SerialPortNow->write(send_array);
}

QByteArray canHandler::frame_to_arry(QCanBusFrame frame, int *count)
{
    QByteArray arry = frame.payload();
    switch (arry.at(0) & 0xF0)
    {
        case Single:
            *count = arry.at(0) &0xF;
            arry = arry.mid(1);
            break;
        case First:
            *count = (arry.at(0) & 0xF) * 16 * 16 + arry.at(1);
            arry = arry.mid(2);
            break;
        case Consecutive:
            arry = arry.mid(1);
            break;
        case Flow:
            arry.clear();
            break;
    }
    return arry.toHex();
}

int canHandler::send_frame(QString text, DeviceType deviceType, int device_id)
{
    int res = 0;
    QByteArray message;
    int can_id = 0;
    foreach(deviceInfo* device, devices)
    {
        if(device->device_type == deviceType && device->device_id == device_id)
        {
            can_id = device->Can_id + 1;
            break;
        }
    }

    if(can_id == 0)
    {
        res = devices.size();
        return res;
    }
    QList<QCanBusFrame> frames = arry_to_frame(text, can_id);
    for(int i = 0; i < frames.size(); i++)
    {
//        qDebug()<<__FILE__<<__LINE__<<frames.at(i).toString();
//        if(i == 0)
//        {
//            if(!canDevice->writeFrame(frames.at(i)))
//            {
//                res = -2;
//                return res;
//            }
//        }
//        else
//        {
//           canDevice->writeFrame(frames.at(i));
//        }
        QCanBusFrame frame = frames.at(i);
        qDebug()<<__FILE__<<__LINE__<<frame.toString();
        QString data = frame_to_str(frame);
        qDebug()<<__FILE__<<__LINE__<<data;
        QByteArray send_array = QByteArray::fromHex(data.toUtf8());
        SerialPortNow->write(send_array);
    }
    return res;
}

bool canHandler::send_check(DeviceType deviceType, int device_id)
{

}

QList<QCanBusFrame> canHandler::arry_to_frame(QString messages, int can_id)
{

    QList<QCanBusFrame> frames;
    messages = messages.mid(4, messages.size() - 8);
    int size = messages.size() / 2;
//    qDebug()<<__FILE__<<__LINE__<<size<<messages;
    if(size > 7 )
    {
        QString message;
        int num = 0;
        int count = 0;
        bool first = true;
        for(int i = 0; i < messages.size(); i++)
        {
            message += messages.at(i);
            num++;
            if(first && num == 12)
            {
                count++;
                first = false;
                num = 0;
                QString DLC = QString::number(size, 16);
                int size = DLC.size();
                for (int i = 0; i <3 - size; i++)
                {
                    DLC = "0" + DLC;
                }
                QString head = "1" + DLC;
                message = head + message;
                QCanBusFrame frame;
                frame.setFrameId(can_id);
                frame.setPayload(QByteArray::fromHex(message.toUtf8()));
                frames.append(frame);
                message.clear();
            }
            else if(num == 14)
            {
                num = 0;
                QString head = "2" + QString::number(count, 16);
                message = head + message;
                QCanBusFrame frame;
                frame.setFrameId(can_id);
                frame.setPayload(QByteArray::fromHex(message.toUtf8()));
                frames.append(frame);
                count++;
                if(count == 16) count = 0;
                message.clear();
            }
        }
//        qDebug()<<__FILE__<<__LINE__<<num;
        if(num != 0)
        {
            QString head = "2" + QString::number(count, 16);
            message = head + message;
            int size = message.size();
            for (int i = 0; i < 16 - size; i++)
            {
                message += "A";
            }
            QCanBusFrame frame;
            frame.setFrameId(can_id);
            frame.setPayload(QByteArray::fromHex(message.toUtf8()));
            frames.append(frame);
        }
    }
    else
    {
        QString DLC = QString::number(size, 16);
        QString head = "0" + DLC;
        messages = head + messages;
        int size = messages.size();
        for (int i = 0; i < 16 - size; i++)
        {
            messages += "A";
        }
        QCanBusFrame frame;
        frame.setFrameId(can_id);
        frame.setPayload(QByteArray::fromHex(messages.toUtf8()));
        frames.append(frame);
    }
    return frames;
}

