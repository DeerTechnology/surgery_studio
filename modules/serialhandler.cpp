#include "serialhandler.h"
#include <QJsonDocument> // 提供了一种读写JSON文档的方法
#include <QJsonObject>   // 封装了一个JSON对象
#include <QJsonValue>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QTime>
LOCATION globalLocNow;

void set_globalLocNow(LOCATION locNow)
{
    globalLocNow.x = locNow.x;
}

LOCATION get_globalLocNow()
{
    return globalLocNow;
}

SerialHandler::SerialHandler(QObject *parent)
    : QObject{parent}
{
    this->init_logic();
}


void SerialHandler::init_logic()
{

#ifdef CANBUS
    canHandlerNow = new canHandler();
    if(canHandlerNow->init())
    {
        connect(this->canHandlerNow, &canHandler::recv_frame, this, &SerialHandler::read_data_can);
    }
    else
    {
        waring.setText("canBus 初始化失败！");
        waring.exec();
    }
#else
    SerialPortNow = new QSerialPort;
    connect(this->SerialPortNow, SIGNAL(readyRead()), this, SLOT(read_data()));
    connect(this->SerialPortNow,
            SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
            this, SLOT(serial_error(QSerialPort::SerialPortError)));
#endif
}

void SerialHandler::serial_error(QSerialPort::SerialPortError error)
{
#ifndef CANBUS
    qDebug() << error;
    if (error != QSerialPort::NoError && SerialPortNow->isOpen())
    {
        SerialPortNow->close();

        waring.setText("连接中断，请检查是否正确连接！");
        waring.exec();
        emit Signal_change_connect_state(false);
        // ui->label_status->setText("未连接");
        // m_pTimer->start(1000);
    }
#endif
}

void SerialHandler::delaymsec(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void SerialHandler::send_move_msg(int steps, double speed, int8_t ifRelative)
{
    QString strToSend = QString("{\"t\":%1,"
                                "\"d\":{\"x\":%2,\"s\":%3,\"ifR\":%4}}\r\n")
                            .arg(1)
                            .arg(steps)
                            .arg(speed)
                            .arg(ifRelative);
    qDebug() <<__FILE__<<__LINE__<< strToSend;
//    if(this->send_serial_message(strToSend))
//        runningState = true;
//    else
//    {
//        runningState = false;
//    }
}

int SerialHandler::send_serial_message(QString strToSend, DeviceType deviceType, int num = 0)
{
#ifdef CANBUS
    return canHandlerNow->send_frame(strToSend, deviceType, num);
#else
    if (this->SerialPortNow->isOpen())
    {

//        qDebug() <<__FILE__<<__LINE__<< "send_serial:" + strToSend;
        strToSend += '\n';
        QByteArray dataToSend = strToSend.toUtf8();
        // const char* myCharPointer = strToSend.toUtf8().constData();
        // qDebug()<<myCharPointer;
        this->SerialPortNow->write(dataToSend);
        return true;
    }
    else
    {
       waring.setText("未连接，请检查是否正确连接！");
        waring.exec();
        return false;
    }
#endif
}
void SerialHandler::close_serial()
{
#ifdef CANBUS
#else

    if (this->SerialPortNow->isOpen())
    {
        this->SerialPortNow->close();
        emit Signal_change_connect_state(false);
    }
#endif
}

int SerialHandler::auto_connect_serial()
{
#ifdef CANBUS
#else
    SerialPortNow->close();
    connectState = false;

    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    QFile log("/root/log.txt");
    log.open(QIODevice::ReadWrite);
    log.write("尝试连接/n");
    log.write(QString("%1/n").toUtf8());

// 如果com.txt不存在或未连接成功，则自动连接
    if (infos.size() == 0)
    {
        qDebug() << "No serial port";
        waring.setText("未检测到串口，请检查是否正确连接！");
        waring.exec();
        return 1;
    }

    for (int i = 0; i < infos.size(); i++)
    {
        QSerialPortInfo info = infos.at(i);
        qDebug() << info.portName();
        log.write(info.portName().toUtf8());
        this->SerialPortNow->setPort(info);
        this->SerialPortNow->setBaudRate(QSerialPort::Baud115200);
        this->SerialPortNow->setDataBits(QSerialPort::Data8);
        this->SerialPortNow->setParity(QSerialPort::NoParity);
        this->SerialPortNow->setStopBits(QSerialPort::OneStop);
        this->SerialPortNow->setFlowControl(QSerialPort::NoFlowControl);
        if (this->SerialPortNow->open(QIODevice::ReadWrite))
        {
            send_check_msg();
            delaymsec(800);
            if (this->connectState == true)
            {
                qDebug() << "Opened serial port successfully";
                emit Signal_change_connect_state(true);
                return 0;
            }
            else
            {
                this->SerialPortNow->close();
            }
        }
    }
    log.close();
    qDebug() << "Open serial port failed";
    waring.setText("未检测到串口，请检查是否正确连接！");
    waring.exec();
    return 1;
#endif
}

void SerialHandler::send_check_msg()
{
#ifdef CANBUS
#else
    SerialPortNow->write(QString("{\"t\":%1}\r\n").arg(M_CHECK).toUtf8());
#endif
}

void SerialHandler::set_connect_state(bool state)
{
    this->connectState = state;
}


void SerialHandler::S_MOVE_handler(QJsonObject jObject)
{
    qDebug() << "S_MOVE_handler called with:" << jObject; // 输出整个 jObject 对象

    if (jObject.contains("d"))
    {
        QString state = jObject.value("d").toString();
        qDebug() << "State value is:" << state; // 输出 "d" 键对应的值

        if (state == "MV")
        {
            runningState = true;
//            emit Signal_run_state_change(_START);
            qDebug() << "Signal_run_state_change "
                        "emitted with _START"; // 输出状态变更信息
        }
        else if (state == "ED")
        {
            runningState = false;
//            emit Signal_run_state_change(_END);
            qDebug() << "Signal_run_state_change "
                        "emitted with _END"; // 输出状态变更信息
        }
    }
    else
    {
        qDebug() << "Key 'd' not "
                    "found in jObject"; // 如果 "d" 键不存在，则输出此信息
    }
}

void SerialHandler::send_online()
{
    canHandlerNow->online_check();
}

void SerialHandler::prase_message(QString strNow)
{
    QJsonDocument jdoc = QJsonDocument::fromJson(strNow.toUtf8());
    if (jdoc.isObject())
    {
        QJsonObject jObject = jdoc.object();
        if (jObject.contains("t"))
        {
//            int datatype = jObject.value("t").toInt();
//            switch (datatype)
//            {
//            case M_CHECK:
//                // this->set_connect_state(true);
//                break;
//            case S_CHECK:
//            {
//                if (jObject.contains("d"))
//                {
//                    QString state = jObject.value("d").toString();
//                    if (state == NAME_DEVICE)
//                    {
//                        qDebug()<<__FILE__<<"进入设备:"<<jdoc;
//                        this->set_connect_state(true);
//                    }
//                }
//                break;
//            }
//            case S_MOVE:
//                this->S_MOVE_handler(jObject);
//                break;

//            case S_SETDATA:
//            {
//                break;
//            }
//            }
        }
    }
    else // 直接返回的值
    {
        if (strNow == "ok")
        {
            // 处理 "ok"
        }
        else
        {
            strNow.replace('\r',"");
            QStringList list = strNow.split(",");
            
            //qDebug() << "data list handler"<<list;
            if (list.size() == 3)
            {
                bool ok1, ok2, ok3;
                int num1 = list[0].toInt(&ok1);
                int num2 = list[1].toInt(&ok2);
                int num3 = list[2].toDouble(&ok3);
                if (ok1 && ok2 && ok3)
                {
                    // 处理 "1000,20,30" 类型的字符串
                    emit Signal_run_data(num1, num2, num3);
                }
            }

        }
    }
}

//QByteArray buffer;
void SerialHandler::read_data()
{
    QByteArray data;
#ifndef CANBUS
    data = SerialPortNow->readAll();

    if (!data.isEmpty())
    {
        // qDebug()<<data;
        foreach (char b, data)
        {
            if (b != '\n')
            {
                buffer.append(b);
                // qDebug()<<b;
            }
            else
            {

                QString strNow = QString::fromUtf8(buffer.constData());
                this->prase_message(strNow);
                buffer.clear();
            }
        }
    }
#endif
}

void SerialHandler::read_data_can(QByteArray arry, DeviceType device_type, int num)
{
#ifdef CANBUS
    emit Signal_device_data(device_type,  num, arry);
#endif
}


void SerialHandler::send_directly_move_msg(double speed, bool ifForward)
{
    QString ifForwardStr = ifForward ? "+" : "-";
    QString strToSend = QString("%1%2\r\n")
                            .arg(ifForwardStr)
                            .arg(speed);
    qDebug() <<__FILE__<<__LINE__<< strToSend;
//    this->send_serial_message(strToSend);
}

void SerialHandler::send_directly_stop_move_msg()
{
    QString strToSend = QString("=\r\n");
//    this->send_serial_message(strToSend);
}

bool SerialHandler::send_check_running_msg()
{
    QString strToSend = QString("?\r\n");
//    return this->send_serial_message(strToSend);
}

void SerialHandler::set_location(int x)
{
    //    LOCATION nowLoc ;
    //    nowLoc = inverseTransform(x,y,z);
    //    set_globalLocNow(nowLoc);

//        QString strToSend = QString("{\"t\":%1,\"d\":{\"x\":%2}}\r\n")
//                .arg(M_SETDATA).arg(x);
//        this->send_serial_message(strToSend);
}

void SerialHandler::send_stop_move_msg()
{
    qDebug() <<__FILE__<<__LINE__<< "send_stop_move_msg called"; // 添加调试输出
    QString strToSend = QString("{\"t\":5,\"d\":\"S\"}\r\n");
//    this->send_serial_message(strToSend);
}

void SerialHandler::send_pause_move_msg()
{
    qDebug() <<__FILE__<<__LINE__ << "send_stop_move_msg called"; // 添加调试输出
    QString strToSend = QString("{\"t\":5,\"d\":\"P\"}\r\n");
//    this->send_serial_message(strToSend);
}
void SerialHandler::send_continue_move_msg()
{
    qDebug() <<__FILE__<<__LINE__<< "send_stop_move_msg called"; // 添加调试输出
    QString strToSend = QString("{\"t\":5,\"d\":\"C\"}\r\n");
//    this->send_serial_message(strToSend);
}

