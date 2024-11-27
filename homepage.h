#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QDialog>
#include "pages/needleselection.h"
#include "pages/addtaskpage.h"
#include "pages/select_worker.h"
#include "modules/serialhandler.h"
#include "modules/needleproperty.h"
#include "modules/taskhandler.h"
#include "ui_modules/setting_version.h"
#include "ui_modules/warning.h"
#include <QStandardItemModel>
#include <QJsonObject>
#include "canbus/heating_analysis.h"
#include "canbus/syringepump_analysis.h"
#include "canbus/lightlz_analysis.h"
QT_BEGIN_NAMESPACE
namespace Ui { class HomePage; }
QT_END_NAMESPACE

struct Device
{
    int num = -1;    //设备号
    bool online = false; //在线状态
    bool running = false;
    int heartbeat = 12;
};



class HomePage : public QDialog
{
    Q_OBJECT

public:
    HomePage(QDialog *parent = nullptr);
    ~HomePage();
    bool connectState = 0;
    void init_ui();
    void init_logic();
    void init_current_style();
    void stop_all();
    void run_ui_change(bool state);
    void set_limit();
    bool send_Timestamp(DeviceType device_type, int num);
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

private slots:
    void slot_change_page();
    void slot_change_connect_state(bool state);
    void slot_messge_syringe(QList<SyringePumpDataInfo> infos, GlobalDataType data_type);
    void slot_messge_surgery(void *infos, GlobalDataType data_type, DeviceType device_type);
    void slot_device_data(DeviceType device_type, int num, QByteArray arry);

    void heart_check();

    void on_pushButton_closeWin_clicked();

    void on_pushButton_maxWin_clicked();

    void on_pushButton_minWin_clicked();





//    void on_pushButton_connect_auto_clicked();


    void on_pushButton_4_clicked();

private:
    Ui::HomePage *ui;
    bool max;
    QRect location;
    select_worker m_worker;
    setting_version setting;
    QStandardItemModel *model;
    QTimer heartTimer;
    int times = 10;
    QString pre_time;
    warning warner;
    canHandler globalCan;
    Device heatingPad, electronic, lightDrill, syringe, handler;
    HeatingPad_analysis heatingPad_analysis;
    SyringePump_analysis syringePump_analysis;
    LightLz_analysis lightDrill_analysis;
};
#endif // HOMEPAGE_H
