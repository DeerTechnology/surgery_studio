#include "ui_modules/surgery.h"
#include "ui_surgery.h"
#include "main.h"
surgery::surgery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::surgery)
{
    ui->setupUi(this);

    init_ui();
    ui->widget_DD->setProperty("nav", "DD");
    connect(&timer, &QTimer::timeout, this, &surgery::timer_task);
    timer.start(CHECK_TIME);
}

surgery::~surgery()
{
    delete ui;
}

void surgery::init_ui()
{
    //设置边框
    ui->widget_info->setProperty("form", "studio");
    ui->widget_temp1->setProperty("form", "studio");
    ui->widget_temp2->setProperty("form", "studio");
//    ui->widget_temps->setProperty("form", "studio");
    ui->widget_DD->setProperty("form", "studio");
    ui->widget_lightValue->setProperty("form", "studio");
    ui->widget_lzValue->setProperty("form", "studio");

    //设置按钮样式
    ui->widget_temp1->setProperty("nav", "temps");
    ui->widget_temp2->setProperty("nav", "temps");
    ui->widget_lightValue->setProperty("nav", "body");
    ui->widget_lzValue->setProperty("nav", "body");
    ui->toolButton_3->setProperty("from","min");
    ui->toolButton_5->setProperty("from","min");
    ui->toolButton_7->setProperty("from","min");
    ui->toolButton_8->setProperty("from","min");
    ui->toolButton_6->setProperty("from","max");
    ui->toolButton_9->setProperty("from","max");
}

//设备状态改变
void surgery::device_statu(DeviceType device_type ,bool statu)
{
    if(statu)
    {
        switch (device_type)
        {
            case SyringePump:
                qDebug()<<__FILE__<<__LINE__<< " 注射泵";
                ui->label_syringe->setText("在线");
                break;
            case Electronic:
                break;
            case HeatingPad:
                qDebug()<<__FILE__<<__LINE__<< " 加热垫";
                ui->label_temp1->setText("在线");
                ui->label_temp2->setText("在线");
                //更新下位机时间戳
                break;
            case LightDrill:
                qDebug()<<__FILE__<<__LINE__<< " 冷光源颅钻";
                ui->label_lz->setText("在线");
                ui->label_light->setText("在线");
                break;
        }
    }
    else
    {
        switch (device_type)
        {
            case HeatingPad:
                ui->label_temp1->setText("下线");
                ui->label_temp2->setText("下线");
                break;
            case SyringePump:
                ui->label_syringe->setText("下线");
                break;
            case Electronic:
                ui->label_DD->setText("下线");
                break;
            case LightDrill:
                ui->label_lz->setText("下线");
                ui->label_light->setText("下线");
                break;
            case Handler:
                break;
        }
    }

}

//加热垫数据 使用
void surgery::recv_data_heating(QList<HeatingPadDataInfo> dataInfos)
{
    int num = 0; //通道数
    foreach(HeatingPadDataInfo info, dataInfos)
    {
        switch (info.name)
        {
            case TargetTemp:        //温度
                if(num == 1)
                {
                    ui->label_tempValue1->setText(QString("%1").arg(info.value.f32_val));
                }
                else
                {
                    ui->label_tempValue2->setText(QString("%1").arg(info.value.f32_val));
                }
                break;
            case ChannelNum:        //通道数
                num = info.value.u8_val;
                break;
            case PageID:            //导出状态变化

                break;
        }
    }
}

//检测冷光源 颅钻是否更新数据
void surgery::timer_task()
{
    if(ui->progressBar->isPerssed())
    {
        QList<LLDataInfo> *infos = new QList<LLDataInfo>;
        LLDataInfo info1;
        info1.name = CantrolCMD_LL;
        info1.length = 1;
        info1.data_type = M_UINT8;
        info1.value.u8_val = M_UPDATE_LIGHT;
        infos->append(info1);
        LLDataInfo info2;
        info2.name = LightValue_LL;
        info2.length = 1;
        info2.data_type = M_UINT8;
        info2.value.u8_val = ui->progressBar->value();
        infos->append(info2);
        LLDataInfo info3;
        info3.name = Color_LL;
        info3.length = 1;
        info3.data_type = M_UINT8;
        info3.value.u8_val = 0;
        infos->append(info3);
        emit Signal_messge_surgery(infos, MasterControl, LightDrill);
    }

    if(ui->progressBar_2->isPerssed())
    {
        QList<LLDataInfo> *infos = new QList<LLDataInfo>;
        LLDataInfo info1;
        info1.name = CantrolCMD_LL;
        info1.length = 1;
        info1.data_type = M_UINT8;
        info1.value.u8_val = M_UPDATE_LZ;
        infos->append(info1);
        LLDataInfo info2;
        info2.name = LightValue_LL;
        info2.length = 1;
        info2.data_type = M_UINT8;
        info2.value.u8_val = ui->progressBar_2->value();
        infos->append(info2);
        LLDataInfo info3;
        info3.name = Color_LL;
        info3.length = 1;
        info3.data_type = M_UINT8;
        info3.value.u8_val = 0;
        infos->append(info3);
        emit Signal_messge_surgery(infos, MasterControl, LightDrill);
    }
}

//切换页面
void surgery::on_toolButton_4_clicked()
{
    emit Signal_change_page();
}


//冷光源开关
void surgery::on_toolButton_clicked()
{
    QList<LLDataInfo> *infos = new QList<LLDataInfo>;
    if(ui->toolButton->text() == "ON")
    {
        ui->toolButton->setText("OFF");
        LLDataInfo info1;
        info1.name = CantrolCMD_LL;
        info1.length = 1;
        info1.data_type = M_UINT8;
        info1.value.u8_val = 0;
        infos->append(info1);
        LLDataInfo info2;
        info2.name = LightValue_LL;
        info2.length = 1;
        info2.data_type = M_UINT8;
        info2.value.u8_val = 50;
        infos->append(info2);
        LLDataInfo info3;
        info3.name = Color_LL;
        info3.length = 1;
        info3.data_type = M_UINT8;
        info3.value.u8_val = 0;
        infos->append(info3);

    }
    else if(ui->toolButton->text() == "OFF")
    {
        ui->toolButton->setText("ON");
        LLDataInfo info1;
        info1.name = CantrolCMD_LL;
        info1.length = 1;
        info1.data_type = M_UINT8;
        info1.value.u8_val = 1;
        infos->append(info1);
    }
    emit Signal_messge_surgery(infos, MasterControl, LightDrill);
}

//颅钻开关
void surgery::on_toolButton_2_clicked()
{
    QList<LLDataInfo> *infos = new QList<LLDataInfo>;
    if(ui->toolButton_2->text() == "ON")
    {
        ui->toolButton_2->setText("OFF");
        LLDataInfo info1;
        info1.name = CantrolCMD_LL;
        info1.length = 1;
        info1.data_type = M_UINT8;
        info1.value.u8_val = 2;
        infos->append(info1);
        LLDataInfo info2;
        info2.name = LightValue_LL;
        info2.length = 1;
        info2.data_type = M_UINT8;
        info2.value.u8_val = 50;
        infos->append(info2);
        LLDataInfo info3;
        info3.name = Color_LL;
        info3.length = 1;
        info3.data_type = M_UINT8;
        info3.value.u8_val = 0;
        infos->append(info3);
    }
    else if(ui->toolButton_2->text() == "OFF")
    {
        ui->toolButton_2->setText("ON");
        LLDataInfo info1;
        info1.name = CantrolCMD_LL;
        info1.length = 1;
        info1.data_type = M_UINT8;
        info1.value.u8_val = 3;
        infos->append(info1);
    }
    emit Signal_messge_surgery(infos, MasterControl, LightDrill);
}


//加热垫1 - 开始按钮
void surgery::on_toolButton_3_clicked()
{
    QList<HeatingPadDataInfo> *infos = new QList<HeatingPadDataInfo>;
    if(!heating_running[0] )
    {
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = M_START;
        info1.length = 1;
        info1.data_type = M_UINT8;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 1;
        info2.length = 1;
        info2.data_type = M_UINT8;
        infos->append(info2);

        HeatingPadDataInfo info3;
        info3.name = TargetTemp;
        info3.value.f32_val = ui->lineEdit->text().toFloat();
        info3.length = 4;
        info3.data_type = M_FLOAT32;
        infos->append(info3);

        ui->toolButton_3->setIcon(QIcon(":/icons/qrcs/icons/stop.png"));
        ui->toolButton_5->setIcon(QIcon(":/icons/qrcs/icons/Database_true.png"));

        heating_running[0] = true;
    }
    else
    {
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = 1;
        info1.length = 1;
        info1.data_type = M_UINT8;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 1;
        info2.length = 1;
        info2.data_type = M_UINT8;
        infos->append(info2);

        ui->toolButton_3->setIcon(QIcon(":/icons/qrcs/icons/start.png"));
        ui->toolButton_5->setIcon(QIcon(":/icons/qrcs/icons/Database_false.png"));
        heating_running[0] = false;
        heating_storage[0] = false;
    }

    emit Signal_messge_surgery(infos, MasterControl, HeatingPad);

}

//加热垫2 - 开始按钮
void surgery::on_toolButton_7_clicked()
{
    QList<HeatingPadDataInfo> *infos = new QList<HeatingPadDataInfo>;
    if(!heating_running[1])
    {
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = M_START;
        info1.length = 1;
        info1.data_type = M_BOOL;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 2;
        info2.length = 1;
        info2.data_type = M_BOOL;
        infos->append(info2);


        HeatingPadDataInfo info3;
        info3.name = TargetTemp;
        info3.value.f32_val = ui->lineEdit_2->text().toFloat();
        info3.length = 4;
        info3.data_type = M_FLOAT32;
        infos->append(info3);

        ui->toolButton_7->setIcon(QIcon(":/icons/qrcs/icons/stop.png"));
        ui->toolButton_8->setIcon(QIcon(":/icons/qrcs/icons/Database_true.png"));

        heating_running[1] = true;
    }
    else
    {
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = M_STOP;
        info1.length = 1;
        info1.data_type = M_UINT8;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 2;
        info2.length = 1;
        info2.data_type = M_UINT8;
        infos->append(info2);

        ui->toolButton_7->setIcon(QIcon(":/icons/qrcs/icons/start.png"));
        ui->toolButton_8->setIcon(QIcon(":/icons/qrcs/icons/Database_false.png"));
        heating_running[1] = false;
        heating_storage[1] = false;
    }
    emit Signal_messge_surgery(infos, MasterControl, HeatingPad);
}

//加入热垫1 - 存储按钮
void surgery::on_toolButton_5_clicked()
{
    QList<HeatingPadDataInfo> *infos = new QList<HeatingPadDataInfo>;
    if(!heating_storage[0])
    {
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = M_STORAGE;
        info1.length = 1;
        info1.data_type = M_UINT8;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 1;
        info2.length = 1;
        info2.data_type = M_UINT8;
        infos->append(info2);


        HeatingPadDataInfo info3;
        info3.name = ifges;
        info3.value.u8_val = 1;
        info3.length = 1;
        info3.data_type = M_UINT8;
        infos->append(info3);

        ui->toolButton_5->setIcon(QIcon(":/icons/qrcs/icons/Database_stop.png"));
        ui->toolButton_6->setIcon(QIcon(":/icons/qrcs/icons/export.png"));
        heating_storage[0] = true;
    }
    else
    {
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = M_STORAGE;
        info1.length = 1;
        info1.data_type = M_UINT8;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 1;
        info2.length = 1;
        info2.data_type = M_UINT8;
        infos->append(info2);


        HeatingPadDataInfo info3;
        info3.name = ifges;
        info3.value.u8_val = 0;
        info3.length = 1;
        info3.data_type = M_UINT8;
        infos->append(info3);

        ui->toolButton_5->setIcon(QIcon(":/icons/qrcs/icons/Database_true.png"));
        heating_storage[0] = false;
    }
    emit Signal_messge_surgery(infos, MasterControl, HeatingPad);
}

//加入热垫2 - 存储按钮
void surgery::on_toolButton_8_clicked()
{
    QList<HeatingPadDataInfo> *infos = new QList<HeatingPadDataInfo>;
    if(!heating_storage[1])
    {
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = M_STORAGE;
        info1.length = 1;
        info1.data_type = M_UINT8;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 2;
        info2.length = 1;
        info2.data_type = M_UINT8;
        infos->append(info2);


        HeatingPadDataInfo info3;
        info3.name = ifges;
        info3.value.u8_val = 1;
        info3.length = 1;
        info3.data_type = M_UINT8;
        infos->append(info3);

        ui->toolButton_8->setIcon(QIcon(":/icons/qrcs/icons/Database_stop.png"));
        ui->toolButton_9->setIcon(QIcon(":/icons/qrcs/icons/export.png"));
        heating_storage[1] = true;
    }
    else
    {
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = M_STORAGE;
        info1.length = 1;
        info1.data_type = M_UINT8;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 2;
        info2.length = 1;
        info2.data_type = M_UINT8;
        infos->append(info2);


        HeatingPadDataInfo info3;
        info3.name = ifges;
        info3.value.u8_val = 0;
        info3.length = 1;
        info3.data_type = M_UINT8;
        infos->append(info3);

        ui->toolButton_8->setIcon(QIcon(":/icons/qrcs/icons/Database_true.png"));
        heating_storage[1] = false;
    }
    emit Signal_messge_surgery(infos, MasterControl, HeatingPad);
}

//加入热垫1 - 导出按钮
void surgery::on_toolButton_6_clicked()
{
    QList<HeatingPadDataInfo> *infos = new QList<HeatingPadDataInfo>;
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = M_EXPORT;
        info1.length = 1;
        info1.data_type = M_UINT8;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 1;
        info2.length = 1;
        info2.data_type = M_UINT8;
        infos->append(info2);

        ui->toolButton_3->setIcon(QIcon(":/icons/qrcs/icons/start.png"));
        ui->toolButton_5->setIcon(QIcon(":/icons/qrcs/icons/Database_false.png"));
        heating_running[0] = false;
        heating_storage[0] = false;
        emit Signal_messge_surgery(infos, MasterControl, HeatingPad);
}

void surgery::on_toolButton_9_clicked()
{
    QList<HeatingPadDataInfo> *infos = new QList<HeatingPadDataInfo>;
        HeatingPadDataInfo info1;
        info1.name = CantrolCMD_HeatingPad;
        info1.value.u8_val = M_EXPORT;
        info1.length = 1;
        info1.data_type = M_UINT8;
        infos->append(info1);

        HeatingPadDataInfo info2;
        info2.name = ChannelNum;
        info2.value.u8_val = 2;
        info2.length = 1;
        info2.data_type = M_UINT8;
        infos->append(info2);

        ui->toolButton_7->setIcon(QIcon(":/icons/qrcs/icons/start.png"));
        ui->toolButton_8->setIcon(QIcon(":/icons/qrcs/icons/Database_false.png"));
        heating_running[1] = false;
        heating_storage[1] = false;

        emit Signal_messge_surgery(infos, MasterControl, HeatingPad);
}

void surgery::on_pushButton_X_clicked()
{
//        QList<HeatingPadDataInfo> infos;
//        HeatingPadDataInfo info1;
//        info1.name = CantrolCMD_HeatingPad;
//        info1.value.u8_val = M_EXPORT;
//        info1.length = 1;
//        info1.data_type = M_UINT8;
//        infos.append(info1);

//        HeatingPadDataInfo info2;
//        info2.name = ChannelNum;
//        info2.value.u8_val = 2;
//        info2.length = 1;
//        info2.data_type = M_UINT8;
//        infos.append(info2);
//        emit Signal_messge_surgery(infos, MasterControl, Dial);
}
