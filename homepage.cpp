#include "homepage.h"
#include "ui_homepage.h"
#include "quiwidget.h"
#include "main.h"
#include <QCanBusFrame>
#include <QMessageBox>
#include <QString>
#include <QJsonDocument>
HomePage::HomePage(QDialog *parent)
    : QDialog(parent), ui(new Ui::HomePage)
{
    ui->setupUi(this);
    this->init_ui();
    this->init_logic();
    this->resize(1024, 600);

//    char buff[100];
//    memset(buff,0 , 100);
//    formatTimestamp(parseTimestamp(), buff, 100);
//    qDebug()<<__FILE__<<__LINE__<<QString::number(25, 16).toUtf8();
//    qDebug()<<__FILE__<<__LINE__<<QCanBusFrame().toString();
//    send_Timestamp(HeatingPad, 1);

    QByteArray array = "050600010204050609000586b400002f27000053c36fc5163f00005242aaaaaaaaaa";
    array = QByteArray::fromHex(array);
    syringePump_analysis.recv_data_init(array);
}

void HomePage::init_ui()
{
    this->max = false;
    this->location = this->geometry();
    this->setProperty("form", true);

    ui->label_version->setText(__VERSION_MASTER);
//    setting.setParent(this);
#ifdef __FULLSCREEN
    this->setProperty("canMove", false);
    setSizeGripEnabled(false);
//    ui->pushButton_minWin->setHidden(true);
//    ui->pushButton_maxWin->setHidden(true);
//    ui->pushButton_closeWin->setHidden(true);
    ui->widgetMenu->hide();
    this->setGeometry(QGuiApplication::primaryScreen()->geometry());
#else
    this->setProperty("canMove", true); // 是否可以移动
    ui->pushButton_4->hide();
#endif
#ifdef __SUBWINDOW
    ui->widgetTitle->setHidden(true);
#endif
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    IconHelper::Instance()->setIcon(ui->pushButton_minWin, QChar(0xF068));
    IconHelper::Instance()->setIcon(ui->pushButton_maxWin, QChar(0xF067));
    IconHelper::Instance()->setIcon(ui->pushButton_closeWin, QChar(0xF00d));
    ui->widgetTitle->setProperty("form", "title");
    ui->widgetTitle->setProperty("nav", "top");
    ui->widgetTop->setProperty("nav", "top");
    this->setWindowTitle(ui->label_title->text());

    needleSelectionWidget = new NeedleSelection(nullptr);


    // 设置整个表格不可编辑
    this->init_current_style();
    addTaskPageWidget = new AddTaskPage(this);

}
void HomePage::set_limit()
{
    //    ui->lineEdit->set_limit_range_vitrual(6,0.1);
}

//发送时间戳给下位机
bool HomePage::send_Timestamp(DeviceType device_type, int num)
{
    QByteArray array;
    array = heatingPad_analysis.send_Timestamp_init();
    qDebug()<<__FILE__<<__LINE__<<array;
    globalSerialHandler->send_serial_message(array, device_type, num);
}
void HomePage::init_current_style()
{
    QFile qFile(":/qss/blue.css");
    qFile.open(QFile::ReadOnly); // 读取qss文件，设置样式
    if (qFile.isOpen())
    {
        QString qss = qFile.readAll();
        this->setStyleSheet(qss);
    }
    qFile.close();
}

void HomePage::init_logic()
{
    globalTaskHandler = new TaskHandler();
    globalSerialHandler = new SerialHandler();
    globalTaskHandler->serialHandler = globalSerialHandler;
    connect(globalSerialHandler, SIGNAL(Signal_change_connect_state(bool)), this, SLOT(slot_change_connect_state(bool)));
    connect(ui->page, SIGNAL(Signal_change_page()), this, SLOT(slot_change_page()));
    connect(ui->page_2, SIGNAL(Signal_change_page()), this, SLOT(slot_change_page()));
    connect(ui->page, &syringe_pump::Signal_messge_syringe, this, &HomePage::slot_messge_syringe);
    connect(ui->page_2, &surgery::Signal_messge_surgery, this,&HomePage::slot_messge_surgery);
    connect(globalSerialHandler, &SerialHandler::Signal_device_data, this, &HomePage::slot_device_data);
    connect(&heartTimer, &QTimer::timeout, this, &HomePage::heart_check);

    heartTimer.start(1000);
}


HomePage::~HomePage()
{
    delete ui;
}


void HomePage::on_pushButton_closeWin_clicked()
{
    close();
}

void HomePage::on_pushButton_maxWin_clicked()
{
    if (this->max)
    {
        this->setGeometry(location);
        this->setProperty("canMove", true);
    }
    else
    {
        location = this->geometry();
        this->setGeometry(QGuiApplication::primaryScreen()->geometry());
        this->setProperty("canMove", false);
    }
    // emit ui->toolButton_changeMode->clicked();
    max = !max;
}

void HomePage::on_pushButton_minWin_clicked()
{
    showMinimized();
}

//外部链接接口
void HomePage::connect_serial()
{
//    globalSerialHandler->auto_connect_serial();
}

//切换页面
void HomePage::slot_change_page()
{
    if(ui->stackedWidget->currentWidget() == ui->page)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_2);
    }
    else
    {
        ui->stackedWidget->setCurrentWidget(ui->page);
    }
}

//连接状态 改变图标
void HomePage::slot_change_connect_state(bool state)
{

    if (state)
    {
        connectState = true;
    }
    else
    {
        connectState = false;
    }
}

//注射泵发送指令
void HomePage::slot_messge_syringe(QList<SyringePumpDataInfo> infos, GlobalDataType data_type)
{

    if(infos.size() < 1)
    {
        return ;
    }
    if(!syringe.online)
    {
        warner.setText("注射泵设备不在线！");
        warner.exec();
        return ;
    }
    QByteArray arry =  syringePump_analysis.send_data_init(infos, data_type);
//    qDebug()<<__FILE__<<__LINE__<<arry;

    int res = globalSerialHandler->send_serial_message(arry, SyringePump, syringe.num);
    if(res == 0)
    {
//        warner.setText("发送成功!");
//        warner.exec();
    }
    else
    {
        warner.setText(QString("发送失败！%1 -- %2 --").arg(QString(arry)).arg(syringe.num));
        warner.exec();
    }
}

//手术台发射指令
void HomePage::slot_messge_surgery(void *infos, GlobalDataType data_type, DeviceType device_type)
{
    QByteArray arry;
    switch (device_type)
    {
        case HeatingPad:
            arry = heatingPad_analysis.send_data_init(infos, data_type);
            break;
        case LightDrill:
            arry = lightDrill_analysis.send_data_init(infos, data_type);
            break;
    }
    qDebug()<<__FILE__<<__LINE__<<arry;
    int num = 0;
    switch (device_type)
    {

    case HeatingPad:
        num = heatingPad.num;
        break;

    case LightDrill:
        num = lightDrill.num;
        break;

    }

    int res = globalSerialHandler->send_serial_message(arry, device_type, num);
    if(res == 0)
    {
//        warner.setText("发送成功!" + QString(" 设备类型：%1，设备号：%2, 数据：").arg(device_type).arg(num)+ arry);
//        warner.exec();
    }
    else
    {
        warner.setText(QString("发送失败！%1--%2").arg(QString(arry)).arg(res));
        warner.exec();
    }
}


//接受下位机数据
void HomePage::slot_device_data(DeviceType device_type, int num, QByteArray array)
{
//    warner.setText(array + QString(" - %1 - %2 -").arg(device_type).arg(num));
//    warner.exec();

    if(array.size() < 10 )
    {
        qDebug()<<__FILE__<<__LINE__<<"数据错误";
        return ;
    }
    array = array.mid(4, array.size() - 8);
    array = QByteArray::fromHex(array);
    qDebug()<<__FILE__<<__LINE__<<array.toHex();
    if((array.at(0) &0xFF) == SlaveOnlineCheck)
    {
        ui->page_2->device_statu(device_type, true);
        switch (device_type)
        {
            case HeatingPad:    //加热垫上线
                heatingPad.num = num;
                heatingPad.online = true;
                heatingPad.heartbeat = 12;
                break;
            case SyringePump:   //注射泵上线
                syringe.num = num;
                syringe.online = true;
                syringe.heartbeat = 12;
                break;
            case LightDrill:          //数显上线
                lightDrill.num = num;
                lightDrill.online = true;
                lightDrill.heartbeat = 12;
                break;
        }
    }
    else
    {
        switch (device_type)
        {
            case HeatingPad:
                ui->page_2->recv_data_heating(heatingPad_analysis.recv_data_init(array));
                heatingPad.heartbeat = 12;
                break;
            case SyringePump:
                ui->page->recv_data(syringePump_analysis.recv_data_init(array), DeviceDataType(array.at(0)));
                syringe.heartbeat = 12;
                break;
            case Electronic:
                break;
            case LightDrill:
                qDebug()<<__FILE__<<__LINE__<<"冷光源颅钻："<<array.toHex();
                lightDrill.heartbeat = 12;
                break;
            case Handler:
                break;
        }
    }
}

void HomePage::heart_check()
{
    times --;
    if(syringe.heartbeat  > 0)
    {
        syringe.heartbeat --;
    }
    else
    {
        ui->page_2->device_statu(SyringePump, false);
        syringe.online = false;
    }

    if(heatingPad.heartbeat  > 0)
    {
        heatingPad.heartbeat --;
    }
    else
    {
        ui->page_2->device_statu(HeatingPad, false);
        heatingPad.online = false;
    }

    if(lightDrill.heartbeat  > 0)
    {
        lightDrill.heartbeat --;
    }
    else
    {
        ui->page_2->device_statu(LightDrill, false);
        lightDrill.online = false;
    }
    if(times <= 0)
    {
        times = 10;
        globalSerialHandler->send_online();
    }
}


//设置按钮
void HomePage::on_pushButton_4_clicked()
{
    QWidget widget(this);
    widget.setStyleSheet("background-color: rgba(255, 255, 255, 0.5);");
    widget.setGeometry(this->rect());
    widget.show();
    setting.exec();
    widget.close();
}
