#include "ui_modules/syringe_pump.h"
#include "ui_syringe_pump.h"
#include "quiwidget.h"
#include <QMessageBox>
syringe_pump::syringe_pump(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::syringe_pump)
{
    ui->setupUi(this);
    this->init_ui();
    this->init_logic();
    this->resize(1024, 600);
}

syringe_pump::~syringe_pump()
{
    delete ui;
}

void syringe_pump::init_ui()
{
    this->max = false;
    this->location = this->geometry();
    this->setProperty("form", true);
    ui->widget_setting->setProperty("form","pad");
    ui->widget_runningData->setProperty("form","pad");
    ui->widget_info->setProperty("form","pad");

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    ui->widget_info->setProperty("color", "black");
    ui->widget_runningData->setProperty("color", "black");
    ui->widget_runningData_2->setProperty("color", "black");
    ui->widget->setProperty("color", "black");
    ui->cyclePage->setProperty("color", "black");
    ui->customPage->setProperty("color", "black");

    needleSelectionWidget = new NeedleSelection(nullptr);

    // 设置表格的表头
    ui->widget_runningData_2->setProperty("form", "pad");

    // 设置整个表格不可编辑
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    // 设置选择方式为整列选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setColumnWidth(0, 60);
    ui->tableWidget->setColumnWidth(1, 80);
    ui->tableWidget->setColumnWidth(2, 100);
    ui->tableWidget->setColumnWidth(3, 110);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setVisible(true);



    ui->label_leftCycle->setProperty("type", "data");
    ui->label_leftTime_2->setProperty("type", "data");
    ui->label_leftTime_3->setProperty("type", "data");
    ui->label_already_2->setProperty("type", "data");
    ui->label_already_3->setProperty("type", "data");QHeaderView *header = ui->tableWidget->horizontalHeader();
    this->init_current_style();
    // 设置字体
    QFont font = header->font(); // 获取当前字体作为基础
    font.setPointSize(7);
    header->setFont(font);

    addTaskPageWidget = new AddTaskPage(this);
    ui->stackedWidget->setCurrentWidget(ui->cyclePage);



}
void syringe_pump::set_limit()
{
    //    ui->lineEdit->set_limit_range_vitrual(6,0.1);
}

void syringe_pump::set_serial(SerialHandler *SerialHandler)
{
    globalSerialHandler = SerialHandler;
    globalTaskHandler->serialHandler = SerialHandler;
}

void syringe_pump::init_current_style()
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

void syringe_pump::init_logic()
{
    connect(needleSelectionWidget, SIGNAL(Signal_needle_select_confirm(NeedlePropertyStruct, QString)), this, SLOT(slot_needle_select_confirm(NeedlePropertyStruct, QString)));
    connect(addTaskPageWidget, SIGNAL(Signal_add_task(TaskType, double, double, double)), this, SLOT(slot_add_task(TaskType, double, double, double)));
    globalTaskHandler = new TaskHandler();
    connect (globalTaskHandler,SIGNAL(Signal_stop_all_task()),this,SLOT(slot_stop_all()));
    connect (globalTaskHandler,SIGNAL(Signal_change_infinited()),this,SLOT(slot_change_infinited()));
    connect (globalTaskHandler,&TaskHandler::Signal_send_message,this,&syringe_pump::slot_send_message);
    connect (globalTaskHandler,&TaskHandler::Signal_send_cmd,this,&syringe_pump::slot_send_cmd);
}


void syringe_pump::slot_add_task(TaskType taskType, double volume, double speed, double pauseTime)
{
    qDebug() << "slot_add_task";
    // qDebug()<<"volume:"<<volume;
    // qDebug()<<"speed:"<<speed;
    // qDebug()<<"pauseTime:"<<pauseTime;
    TaskUnit task;
    task.taskType = taskType;
    task.needleData.volume = volume;
    task.needleData.speed = speed;
    task.needleProperty = globalNeedleProperty;
    task.needleData.pauseTime = pauseTime;
    CustomPageTaskList.append(task);
    // ui->tableWidget
    int currentIndex = ui->tableWidget->rowCount(); // 获取当前行数作为索引

    ui->tableWidget->insertRow(currentIndex);       // 在最后一行之后插入新行
    // // 设置任务类型
    QString typeStr;
    if (taskType == TaskType_Inject)
    {
        typeStr = "注射";
    }
    else if (taskType == TaskType_Extract)
    {
        typeStr = "抽取";
    }
    else if (taskType == TaskType_Wait)
    {
        typeStr = "等待";
    }


    QTableWidgetItem *numItem = new QTableWidgetItem(QString("%1").arg(currentIndex + 1));
    numItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(currentIndex, 0, numItem);
    QTableWidgetItem *typeItem = new QTableWidgetItem(typeStr);
    typeItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(currentIndex, 1, typeItem);

    // 设置体积
    double volumeData = volume;
    QString volumeSubfix = "uL";
    if (volume > 1000)
    {
        volumeData = volume / 1000;
        volumeSubfix = "mL";
    }
    else if (volume < 1)
    {
        volumeData = volume * 1000;
        volumeSubfix = "nL";
    }
    QTableWidgetItem *volumeItem = new QTableWidgetItem(QString::asprintf("%.2f", volumeData) + volumeSubfix);
    ui->tableWidget->setItem(currentIndex, 2, volumeItem);
    volumeItem->setTextAlignment(Qt::AlignCenter);

    // 设置速度
    double speedData = speed;
    QString speedSubfix = "uL/min";
    if (speed > 1000)
    {
        speedData = speed / 1000;
        speedSubfix = "mL/min";
    }
    else if (speed < 1)
    {
        speedData = speed * 1000;
        speedSubfix = "nL/min";
    }
    QTableWidgetItem *speedItem = new QTableWidgetItem(QString::asprintf("%.2f", speedData) + speedSubfix);
    ui->tableWidget->setItem(currentIndex, 3, speedItem);
    speedItem->setTextAlignment(Qt::AlignCenter);

    // 设置暂停时间
    QTableWidgetItem *pauseTimeItem = new QTableWidgetItem(QString::number(pauseTime) + "S");
    ui->tableWidget->setItem(currentIndex, 4, pauseTimeItem);
    pauseTimeItem->setTextAlignment(Qt::AlignCenter);
}


void syringe_pump::on_toolButton_clicked() // 添加
{

    if (!hasNeedle)
    {
        warner.setText("请先选择针管");
        warner.exec();
        return;
    }
    QWidget widget(this);
    widget.setStyleSheet("background-color: rgba(255, 255, 255, 0.5);");
    widget.setGeometry(this->rect());
    widget.show();
    addTaskPageWidget->exec();
    widget.close();
}

void syringe_pump::on_toolButton_2_clicked() // 删除选中
{
    int indexNow = ui->tableWidget->currentIndex().row();
//    QList<QTableWidgetItem *> items = ui->tableWidget->selectedItems();
//    qDebug()<<items;
    if (indexNow == -1)
    {
        warner.setText("请先选择要删除的任务");
        warner.exec();
    }
    else
    {
        CustomPageTaskList.removeAt(indexNow);
        ui->tableWidget->removeRow(indexNow);
    }

    int rows = ui->tableWidget->rowCount(); // 获取当前行数作为索引
    for(int i = 0; i < rows; i ++)
    {
        ui->tableWidget->item(i,0)->setText(QString("%1").arg(i + 1));
    }
}

void syringe_pump::on_toolButton_3_clicked() // 清空
{
    CustomPageTaskList.clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}


//针头参数解析
void syringe_pump::slot_needle_select_confirm(NeedlePropertyStruct needleProperty, QString name)
{
    qDebug() << "slot_needle_select_confirm";
    qDebug() << "name:" << name;
    hasNeedle = true;
    addTaskPageWidget->hasNeedle = true;
    addTaskPageWidget->needleProperty.maxVolume = needleProperty.maxVolume;
    addTaskPageWidget->needleProperty.inner_diameter = needleProperty.inner_diameter;
    addTaskPageWidget->needleProperty.maxVspeed = needleProperty.maxVspeed;
    addTaskPageWidget->needleProperty.minVspeed = needleProperty.minVspeed;
    addTaskPageWidget->needleProperty.stepVolume = needleProperty.stepVolume;

    globalNeedleProperty.maxVolume = needleProperty.maxVolume;
    globalNeedleProperty.inner_diameter = needleProperty.inner_diameter;
    globalNeedleProperty.maxVspeed = needleProperty.maxVspeed;
    globalNeedleProperty.minVspeed = needleProperty.minVspeed;
    globalNeedleProperty.stepVolume = needleProperty.stepVolume;
    NeedlePropertyStruct *currentNeedlePropertyStruct = &needleProperty;
    QString volumeSuffix = "ul";
    double volumeData = currentNeedlePropertyStruct->maxVolume;
    if (currentNeedlePropertyStruct->maxVolume > 1000)
    {
        volumeSuffix = "ml";
        volumeData = volumeData / 1000;
    }
    else if (currentNeedlePropertyStruct->maxVolume < 1)
    {
        volumeSuffix = "nl";
        volumeData = volumeData * 1000;
    }
    QString volume = QString::asprintf("%.3f", volumeData) + volumeSuffix;

    QString innerDiameter = QString::asprintf("%.3fmm", currentNeedlePropertyStruct->inner_diameter);

    QString maxVolumeSuffix = "uL/min";
    double maxVolumeSpeed = currentNeedlePropertyStruct->maxVspeed;
    if (currentNeedlePropertyStruct->maxVspeed >= 1000)
    {
        maxVolumeSuffix = "mL/min";
        maxVolumeSpeed = maxVolumeSpeed / 1000;
    }
    else if (currentNeedlePropertyStruct->maxVspeed < 1)
    {
        maxVolumeSuffix = "nL/min";
        maxVolumeSpeed = maxVolumeSpeed * 1000;
    }
    QString maxVolumeSpeeds = QString::asprintf("%.3f", maxVolumeSpeed) + maxVolumeSuffix;

    double minVolumeSpeed = currentNeedlePropertyStruct->minVspeed;
    QString minVolumeSuffix = "uL/min";
    if (currentNeedlePropertyStruct->minVspeed >= 1000)
    {
        minVolumeSuffix = "mL/min";
        minVolumeSpeed = minVolumeSpeed / 1000;
    }
    else if (currentNeedlePropertyStruct->minVspeed < 1)
    {
        minVolumeSuffix = "nL/min";
        minVolumeSpeed = minVolumeSpeed * 1000;
    }
    QString minVolumeSpeeds = QString::asprintf("%.3f", minVolumeSpeed) + minVolumeSuffix;

    ui->label_type->setText(name);
    ui->label_min->setText(minVolumeSpeeds);
    ui->label_max->setText(maxVolumeSpeeds);
    ui->label_diameter->setText(innerDiameter);
    ui->label_volume->setText(volume);
    ui->toolButton_seg->setText(name.mid(0, name.indexOf('-')));
}


//更新所有状态
void syringe_pump::slot_stop_all()
{
    QString strNow = "--";
    run_ui_change(true);
    isrunning = false;
    QIcon iconNow = QIcon(":/icons/qrcs/icons/start.png");
    ui->toolButton_stopAll->setIcon(iconNow);
    iconNow = QIcon(":/icons/qrcs/icons/puase_false.png");
    ui->toolButton_pause->setIcon(iconNow);
    ui->toolButton_stopAll->setText("开始");
    ui->toolButton_pause->setText("暂停");

    ui->label_already_2->setText(strNow);
    ui->label_leftTime_2->setText(strNow);
    ui->label_already_3->setText(strNow);
    ui->label_leftTime_3->setText(strNow);
    ui->label_leftCycle->setText(strNow);
    ui->widget_bar->setValue(0);
    ui->widget_bar_2->setValue(0);
}

//主动暂停
void syringe_pump::stop_all()
{
    QString strNow = "--";
    run_ui_change(true);
    isrunning = false;

    QIcon iconNow = QIcon(":/icons/qrcs/icons/start.png");
    ui->toolButton_stopAll->setIcon(iconNow);
    iconNow = QIcon(":/icons/qrcs/icons/puase_false.png");
    ui->toolButton_pause->setIcon(iconNow);
    ui->toolButton_stopAll->setText("开始");
    ui->toolButton_pause->setText("暂停");

    ui->label_already_2->setText(strNow);
    ui->label_leftTime_2->setText(strNow);
    ui->label_already_3->setText(strNow);
    ui->label_leftTime_3->setText(strNow);

    ui->label_leftCycle->setText(strNow);

    ui->widget_bar->setValue(0);
    ui->widget_bar_2->setValue(0);
    globalTaskHandler->stopTask();

    QList<SyringePumpDataInfo> infos;

    SyringePumpDataInfo info1;
    info1.name = CantrolCMD_Syringe;
    info1.length = 1;
    info1.data_type = M_UINT8;
    info1.value.u8_val = M_END_syringe;
    infos.append(info1);
    emit Signal_messge_syringe(infos, MasterControl);
}

void syringe_pump::run_data_init(int32_t totalStepNow, int32_t restStepNow, int32_t speedNow)
{
    int32_t totalStep = totalStepNow;
    int32_t restStep = restStepNow;
    int32_t speed = speedNow;
    if(speed == 0 ||totalStep ==0)
    {
        qDebug()<<__FILE__<<__LINE__<<"数据错误 --- speed："<<speed<<"总步数："<<totalStep <<"剩余步数："<<restStep;
        return ;
    }
    if (globalTaskHandler->ifRunning)
    {
        if(globalTaskHandler->runningType == RunningType_Custom && globalTaskHandler->ifCustomPause == true)//暂停
        {
            ui->label_already_3->setText("--");
            ui->label_leftTime_3->setText(QString::asprintf("%.2f", globalTaskHandler->currentCustomPauseIndex));
            ui->label_alreadyUnits_3->setText("uL");
            double percentNow = (globalTaskHandler->currentCustomPauseIndexSet - globalTaskHandler->currentCustomPauseIndex)
                    * 100 / globalTaskHandler->currentCustomPauseIndexSet;
            ui->widget_bar->setValue(percentNow);
            return;
        }
        double stepPeruL = globalNeedleProperty.stepVolume; // 每步代表的体积
        int percent = (totalStep - restStep) * 100 / totalStep;
//        qDebug()<<__FILE__
        double restTime = restStep / speed;
        double alreadyVolume = (totalStep - restStep) * stepPeruL;
        QString volumesuffix = "uL";
        if (abs(alreadyVolume) > 1000)
        {
            alreadyVolume = alreadyVolume / 1000;
            volumesuffix = "mL";
        }
        else if (abs(alreadyVolume) < 1)
        {
            alreadyVolume = alreadyVolume * 1000;
            volumesuffix = "nL";
        }
        if (globalTaskHandler->runningType == RunningType_Single)
        {
            ui->label_already_2->setText(QString::asprintf("%.2f", alreadyVolume));
            ui->label_leftTime_2->setText(QString::asprintf("%.2f", restTime));
            ui->label_alreadyUnits_2->setText(volumesuffix);

            ui->widget_bar->setValue(percent);
        }
        else if (globalTaskHandler->runningType == RunningType_Loop)
        {
            ui->label_already_2->setText(QString::asprintf("%.2f", alreadyVolume));
            ui->label_leftTime_2->setText(QString::asprintf("%.2f", restTime));
            ui->label_alreadyUnits_2->setText(volumesuffix);
            ui->widget_bar->setValue(percent);
        }
        else if (globalTaskHandler->runningType == RunningType_Custom)
        {
            if(ui->label_already_3->text() == "--")//上一个任务是暂停
            {
                clear_custom_running_info_and_set(globalTaskHandler->currentTaskIndex);
            }
            ui->label_already_3->setText(QString::asprintf("%.2f", alreadyVolume));
            ui->label_leftTime_3->setText(QString::asprintf("%.2f", restTime));
            ui->label_alreadyUnits_3->setText(volumesuffix);
            ui->widget_bar_2->setValue(percent);

        }
    }
}


//改变自定义任务状态
void syringe_pump::clear_custom_running_info_and_set(int index)
{
    int columnToClear = 4;      // 要清空的列索引
    QString specificData = "√"; // 要填写的特定数据
    if(index >= ui->tableWidget->rowCount())
    {
        QTableWidgetItem *preItem = ui->tableWidget->item(ui->tableWidget->rowCount() - 1, columnToClear);
//        qDebug()<<__FILE__<<__LINE__<<pre_time;
        preItem->setText(pre_time);
        return ;
    }
    if (index > 0)
    {
        QTableWidgetItem *preItem = ui->tableWidget->item(index - 1, columnToClear);
        qDebug()<<__FILE__<<__LINE__<<pre_time;
        preItem->setText(pre_time);
    }
    QTableWidgetItem *newItem = ui->tableWidget->item(index, columnToClear);
    pre_time = newItem->text();
    qDebug()<<__FILE__<<__LINE__<<pre_time;
    newItem->setText(specificData);
}

void syringe_pump::connect_serial()
{
    globalSerialHandler->auto_connect_serial();
}

//接收下位机数据
void syringe_pump::recv_data(QList<SyringePumpDataInfo> infos, DeviceDataType cmd)
{
    if(infos.size() < 1)
    {
        return ;
    }
    if(cmd == 5)
    {
        SyringePumpDataInfo info = infos.first();
        if(info.name == CantrolCMD_Syringe)
        {
            switch (info.value.u8_val)
            {
                case S_GET_syringe:
                    run_data_init(infos.at(1).value.i32_val, infos.at(3).value.i32_val, infos.at(2).value.i32_val);
                    qDebug()<<__FILE__<<__LINE__<<"总量："<<infos.at(1).value.i32_val<<"速度："<<infos.at(2).value.i32_val<<"剩余："<<infos.at(3).value.i32_val;
                    break;
                case S_MOVE_syringe:
                    if(infos.at(1).value.u8_val == 2)
                    {
                        if(isrunning)
                        {
                            if(globalTaskHandler->ifPause)
                            {
                                qDebug()<<"pause";
                                return;
                            }
                            if (globalTaskHandler->ifRunning)
                            {
                                if (globalTaskHandler->runningType == RunningType_Single)
                                {
                                    ui->widget_bar->setValue(100);
                                }
                                else if (globalTaskHandler->runningType == RunningType_Custom)
                                {
                                    clear_custom_running_info_and_set(globalTaskHandler->currentTaskIndex+1);
                                    if(globalTaskHandler->currentTaskIndex+1 >= globalTaskHandler->taskList.size())
                                    {
                                        stop_all();

                                        ui->widget_bar_2->setValue(100);
                                        return;
                                    }
                                }

                                globalTaskHandler->nextTask();
                            }
                        }
                    }
                    else
                    {
                        qDebug()<<__FILE__<<__LINE__<<"开始"<<infos.at(1).value.u8_val;
                    }

                    break;
            }
        }
    }

}

void syringe_pump::slot_run_state_change(MOVE_STATE state)
{
    if (state == _START)
    {
    }
    else if (state == _END)
    {
        if(globalTaskHandler->ifPause)
        {
            qDebug()<<"pause";
            return;
        }
        if (globalTaskHandler->ifRunning)
        {
            if (globalTaskHandler->runningType == RunningType_Single)
            {
                ui->widget_bar->setValue(100);
            }
            else if (globalTaskHandler->runningType == RunningType_Custom)
            {
                clear_custom_running_info_and_set(globalTaskHandler->currentTaskIndex+1);
                if(globalTaskHandler->currentTaskIndex+1 >= globalTaskHandler->taskList.size())
                {
                    stop_all();

                    ui->widget_bar_2->setValue(100);
                    return;
                }
            }

            globalTaskHandler->nextTask();
        }
    }
}

void syringe_pump::run_ui_change(bool state)
{
    ui->toolButton_worker->setEnabled(state);
    ui->toolButton_seg->setEnabled(state);
    ui->toolButton_manualInjection->setEnabled(state);
    ui->toolButton_manualExtraction->setEnabled(state);
    ui->toolButton->setEnabled(state);
    ui->toolButton_2->setEnabled(state);
    ui->toolButton_3->setEnabled(state);
}


//暂停继续按钮
void syringe_pump::on_toolButton_pause_clicked()
{
    //暂停
    QHeaderView *header = ui->tableWidget->horizontalHeader();
    // 设置字体
    QFont font = header->font(); // 获取当前字体作为基础
qDebug()<<__FILE__<<__LINE__<<font;
    QList<SyringePumpDataInfo> infos;
    if (globalTaskHandler->ifRunning && !globalTaskHandler->ifPause)
    {
        SyringePumpDataInfo info1;
        info1.name = CantrolCMD_Syringe;
        info1.length = 1;
        info1.data_type = M_UINT8;
        info1.value.u8_val = M_STOP_syringe;
        infos.append(info1);
        globalTaskHandler->pauseTask();
        QIcon iconNow = QIcon(":/icons/qrcs/icons/start.png");
        ui->toolButton_pause->setIcon(iconNow);
        ui->toolButton_pause->setText("继续");
    }

    //继续
    else if (globalTaskHandler->ifPause && globalTaskHandler->ifRunning)
    {
        SyringePumpDataInfo info1;
        info1.name = CantrolCMD_Syringe;
        info1.length = 1;
        info1.data_type = M_UINT8;
        info1.value.u8_val = M_CONTINUE_syringe;
        infos.append(info1);
        globalTaskHandler->continueTask();
        QIcon iconNow = QIcon(":/icons/qrcs/icons/puase.png");
        ui->toolButton_pause->setIcon(iconNow);
        ui->toolButton_pause->setText("暂停");
    }
    emit Signal_messge_syringe(infos, MasterControl_Fast);
}


//简单注射 按钮按下
void syringe_pump::on_toolButton_manualExtraction_pressed()
{
    uint speedNow = StepPerMM * 3;
    QList<SyringePumpDataInfo> infos;

    SyringePumpDataInfo info1;
    info1.name = CantrolCMD_Syringe;
    info1.length = 1;
    info1.data_type = M_UINT8;
    info1.value.u8_val = M_SIMPLEMOVE_syringe;
    infos.append(info1);

    SyringePumpDataInfo info2;
    info2.name = Speed_syringe;
    info2.length = 4;
    info2.data_type = M_UINT32;
    info2.value.u32_val = speedNow;
    infos.append(info2);

    SyringePumpDataInfo info3;
    info3.name = Directtion_syringe;
    info3.length = 1;
    info3.data_type = M_UINT8;
    info3.value.u8_val = 1;
    infos.append(info3);

//    SyringePumpDataInfo info4;
//    info4.name = IfR_syringe;
//    info4.length = 1;
//    info4.data_type = M_BOOL;
//    info4.value.u8_val = 1;
//    infos.append(info4);

    emit Signal_messge_syringe(infos, MasterControl_Fast);

}


//简单注射 按钮松开
void syringe_pump::on_toolButton_manualExtraction_released()
{
    QList<SyringePumpDataInfo> infos;

    SyringePumpDataInfo info1;
    info1.name = CantrolCMD_Syringe;
    info1.length = 1;
    info1.data_type = M_UINT8;
    info1.value.u8_val = M_END_syringe;
    infos.append(info1);

    emit Signal_messge_syringe(infos, MasterControl_Fast);
}

void syringe_pump::on_toolButton_manualInjection_pressed()
{
    uint speedNow = StepPerMM * 3;
    QList<SyringePumpDataInfo> infos;

    SyringePumpDataInfo info1;
    info1.name = CantrolCMD_Syringe;
    info1.length = 1;
    info1.data_type = M_UINT8;
    info1.value.u8_val = M_SIMPLEMOVE_syringe;
    infos.append(info1);

    SyringePumpDataInfo info2;
    info2.name = Speed_syringe;
    info2.length = 4;
    info2.data_type = M_UINT32;
    info2.value.u32_val = speedNow;
    infos.append(info2);

    SyringePumpDataInfo info3;
    info3.name = Directtion_syringe;
    info3.length = 1;
    info3.data_type = M_UINT8;
    info3.value.u8_val = 0;
    infos.append(info3);

//    SyringePumpDataInfo info4;
//    info4.name = IfR_syringe;
//    info4.length = 1;
//    info4.data_type = M_BOOL;
//    info4.value.u8_val = 1;
//    infos.append(info4);

    emit Signal_messge_syringe(infos, MasterControl_Fast);
}

void syringe_pump::on_toolButton_manualInjection_released()
{
    QList<SyringePumpDataInfo> infos;
    SyringePumpDataInfo info1;
    info1.name = CantrolCMD_Syringe;
    info1.length = 1;
    info1.data_type = M_UINT8;
    info1.value.u8_val = M_END_syringe;
    infos.append(info1);

    emit Signal_messge_syringe(infos, MasterControl_Fast);
}


//开始  结束
void syringe_pump::on_toolButton_stopAll_clicked()
{
    if(!isrunning)
    {
        if(ui->toolButton_worker->text() == "自定义")
        {
            if (CustomPageTaskList.length() == 0)
            {
                warner.setText("请先添加任务");
                warner.exec();
                return;
            }
            if (!globalTaskHandler->ifRunning && !globalTaskHandler->ifPause)
            {
                globalTaskHandler->clearTask();
                for (int i = 0; i < CustomPageTaskList.length(); i++)
                {
                    globalTaskHandler->addTask(CustomPageTaskList.at(i));
                }
                globalTaskHandler->runningType = RunningType_Custom;
            }
        }
        else
        {
            if (!globalTaskHandler->ifRunning && !globalTaskHandler->ifPause)
            {
                globalTaskHandler->clearTask();
                double volume = ui->lineEdit_volume_2->text().toDouble();
                if (ui->radioButton_volumemL_2->isChecked())
                {
                    volume *= 1000;
                }
                else if (ui->radioButton_volumenL_2->isChecked())
                {
                    volume /= 1000;
                }
                if (volume > globalNeedleProperty.maxVolume)
                {
                    warner.setText("体积超出针管容量");
                    warner.exec();
                    return;
                }
                double speed = ui->lineEdit_speed_2->text().toDouble();
                if (ui->radioButton_speedmL_2->isChecked())
                {
                    speed *= 1000;
                }
                else if (ui->radioButton_speednL_2->isChecked())
                {
                    speed /= 1000;
                }
                if (speed > globalNeedleProperty.maxVspeed)
                {
                    warner.setText("速度超出针管最大速度");
                    warner.exec();
                    return;
                }
                else if (speed < globalNeedleProperty.minVspeed)
                {
                    warner.setText("速度低于针管最小速度");
                    warner.exec();
                    return;
                }
                TaskUnit task;
                task.needleData.pauseTime = 0;
                task.needleData.speed = speed;
                task.needleData.volume = volume;
                task.needleProperty = globalNeedleProperty;
                int num = ui->lineEdit_cycleTime_2->text().toInt();
                if(num <= 0) return;
                if(ui->toolButton_worker->text() == "注射抽取")
                {
                        task.taskType = TaskType_Inject;
                        globalTaskHandler->addTask(task);
                        task.taskType = TaskType_Extract;
                        globalTaskHandler->addTask(task);

                }
                else if(ui->toolButton_worker->text() == "抽取注射")
                {
                        task.taskType = TaskType_Extract;
                        globalTaskHandler->addTask(task);
                        task.taskType = TaskType_Inject;
                        globalTaskHandler->addTask(task);

                }
                else if(ui->toolButton_worker->text() == "抽取")
                {
                        task.taskType = TaskType_Extract;
                        globalTaskHandler->addTask(task);
                }
                else
                {
                        task.taskType = TaskType_Inject;
                        globalTaskHandler->addTask(task);
                }
                globalTaskHandler->ifInfinite = true;
                if(ui->radioButton_infinite->isChecked())
                {
                    globalTaskHandler->setInfinite(-1);
                    ui->label_leftCycle->setText("MAX");
                }
                else
                {
                    ui->label_leftCycle->setText(QString("%1").arg(num - 1));
                    globalTaskHandler->setInfinite(num);
                }
                globalTaskHandler->runningType = RunningType_Single;
            }
        }
        isrunning = true;
        run_ui_change(false);
        QIcon iconNow = QIcon(":/icons/qrcs/icons/stop.png");
        ui->toolButton_stopAll->setIcon(iconNow);
        iconNow = QIcon(":/icons/qrcs/icons/puase.png");
        ui->toolButton_pause->setIcon(iconNow);
        ui->toolButton_stopAll->setText("结束");
        globalTaskHandler->startTask();
    }

    else
    {
        globalTaskHandler->ifInfinite = false;
        stop_all();
        if(ui->toolButton_worker->text() == "自定义")
        {
            QTableWidgetItem *preItem = ui->tableWidget->item(globalTaskHandler->currentTaskIndex, 4);
            qDebug()<<__FILE__<<__LINE__<<preItem<<pre_time;
            preItem->setText(pre_time);
        }

    }
}

//连接状态 改变图标
void syringe_pump::slot_change_connect_state(bool state)
{

}

void syringe_pump::slot_change_infinited()
{
    int num = ui->label_leftCycle->text().toInt();
    if( num > 0)
    {
        ui->label_leftCycle->setText(QString("%1").arg(num - 1));
    }
}

//注射泵任务参数
void syringe_pump::slot_send_message(int volumeStep, int speedStep, bool statu)
{
    QList<SyringePumpDataInfo> infos;

    SyringePumpDataInfo info1;
    info1.name = CantrolCMD_Syringe;
    info1.length = 1;
    info1.data_type = M_UINT8;
    info1.value.u8_val = M_MOVE_syringe;
    infos.append(info1);

    SyringePumpDataInfo info2;
    info2.name = AlldistanceToGo_syringe;
    info2.length = 4;
    info2.data_type = M_UINT32;
    info2.value.u32_val = volumeStep;
    infos.append(info2);

    SyringePumpDataInfo info3;
    info3.name = Speed_syringe;
    info3.length = 4;
    info3.data_type = M_INT32;
    info3.value.i32_val = speedStep;
    infos.append(info3);

    SyringePumpDataInfo info4;
    info4.name = IfM_syringe;
    info4.length = 1;
    info4.data_type = M_BOOL;
    info4.value.u8_val = statu;
    infos.append(info4);

    emit Signal_messge_syringe(infos, MasterControl_Fast);
}

//注射泵命令
void syringe_pump::slot_send_cmd(int cmd)
{
    if(cmd == 1)
    {
        QList<SyringePumpDataInfo> infos;
        SyringePumpDataInfo info1;
        info1.name = CantrolCMD_Syringe;
        info1.length = 1;
        info1.data_type = M_UINT8;
        info1.value.u8_val = M_GET_syringe;
        infos.append(info1);
        emit Signal_messge_syringe(infos, MasterControl_Fast);
    }
    else if(cmd == 2)
    {

    }
    else if(cmd == 3)
    {

    }
    else if(cmd == 4)
    {

    }
}

//选择工作模式
void syringe_pump::on_toolButton_worker_clicked()
{
    QWidget widget(this);
    widget.setStyleSheet("background-color: rgba(255, 255, 255, 0.5);");
    widget.setGeometry(this->rect());
    widget.show();
    m_worker.exec();
    widget.close();
    QString work = m_worker.get_worker();

    ui->toolButton_worker->setText(work);

    if(work == "自定义")
    {
        ui->stackedWidget->setCurrentWidget(ui->customPage);
    }
    else
    {
        ui->stackedWidget->setCurrentWidget(ui->cyclePage);

    }
}

//针头选择
void syringe_pump::on_toolButton_seg_clicked()
{
    QWidget widget(this);
    widget.setStyleSheet("background-color: rgba(255, 255, 255, 0.5);");
    widget.setGeometry(this->rect());
    widget.show();
    needleSelectionWidget->exec();
    widget.close();
}


void syringe_pump::on_toolButton_4_clicked()
{
    emit Signal_change_page();
}

