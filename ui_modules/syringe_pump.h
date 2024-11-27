#ifndef SYRINGE_PUMP_H
#define SYRINGE_PUMP_H

#include <QWidget>
#include "pages/needleselection.h"
#include "pages/addtaskpage.h"
#include "pages/select_worker.h"
#include "modules/serialhandler.h"
#include "modules/needleproperty.h"
#include "modules/taskhandler.h"
#include "ui_modules/warning.h"
#include <QStandardItemModel>
#include <QJsonObject>
#include "canbus/syringepump_analysis.h"

namespace Ui {
class syringe_pump;
}

//typedef struct
//{
//    QString comName;
//    QString comDiscribe;
//    QSerialPortInfo infoNow;
//}SerialData;
//Q_DECLARE_METATYPE(SerialData)

class syringe_pump : public QWidget
{
    Q_OBJECT

public:
    explicit syringe_pump(QWidget *parent = nullptr);
    ~syringe_pump();

    bool isrunning = 0;
    void init_ui();
    void init_logic();
    void init_current_style();
    void stop_all();
    void run_ui_change(bool state);
    void set_limit();
    void set_serial(SerialHandler *SerialHandler);
    bool hasNeedle = false;
    NeedleSelection *needleSelectionWidget;
    AddTaskPage *addTaskPageWidget;
    NeedlePropertyStruct globalNeedleProperty;
    TaskHandler * globalTaskHandler;
    QList<TaskUnit> CustomPageTaskList;
    QList<SerialData> globalSerialList;
    SerialHandler * globalSerialHandler;
    void clear_custom_running_info_and_set(int index);
    void connect_serial();
    void recv_data(QList<SyringePumpDataInfo> infos, DeviceDataType cmd);
    void run_data_init(int32_t totalStep, int32_t restStep, int32_t speed);

private slots:
    void slot_stop_all();
    void slot_run_state_change(MOVE_STATE state);
    void slot_add_task(TaskType taskType,double volume ,double speed ,double pauseTime);
    void slot_needle_select_confirm(NeedlePropertyStruct needleProperty,QString name);
    void slot_change_connect_state(bool state);
    void slot_change_infinited();
    void slot_send_message( int volumeStep, int speedStep, bool statu);
    void slot_send_cmd(int cmd);

    void on_toolButton_clicked();

    void on_toolButton_pause_clicked();

    void on_toolButton_manualExtraction_pressed();

    void on_toolButton_manualExtraction_released();

    void on_toolButton_manualInjection_pressed();

    void on_toolButton_manualInjection_released();

    void on_toolButton_stopAll_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_worker_clicked();

    void on_toolButton_seg_clicked();

    void on_toolButton_4_clicked();

signals:
    void Signal_change_page();
    void Signal_messge_syringe(QList<SyringePumpDataInfo> infos, GlobalDataType device_type);


private:
    Ui::syringe_pump *ui;
    bool max;
    QRect location;
    select_worker m_worker;
    QStandardItemModel *model;
    QTimer timer;
    QString pre_time;
    warning warner;
    SyringePump_analysis syringePump_analysis;
};

#endif // SYRINGE_PUMP_H
