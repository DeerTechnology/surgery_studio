#ifndef SURGERY_H
#define SURGERY_H

#include <QWidget>
#include <QJsonObject>
#include "canbus/canhandler.h"
#include "canbus/heating_analysis.h"
#include "canbus/lightlz_analysis.h"
namespace Ui {
class surgery;
}

class surgery : public QWidget
{
    Q_OBJECT

public:
    explicit surgery(QWidget *parent = nullptr);
    ~surgery();
    void init_ui();

    void device_statu(DeviceType device_type, bool statu);
    void recv_data_heating(QList<HeatingPadDataInfo> dataInfos);
signals:
    void Signal_change_page();
    void Signal_messge_surgery(void * infos, GlobalDataType data_type,DeviceType deviceType);

private slots:
    void timer_task();
    void on_toolButton_4_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();


    void on_toolButton_7_clicked();

    void on_toolButton_5_clicked();

    void on_toolButton_8_clicked();

    void on_toolButton_6_clicked();

    void on_toolButton_9_clicked();

    void on_pushButton_X_clicked();

private:
    Ui::surgery *ui;

    bool heating_running[2] = {0,0}, heating_storage[2] = {0,0};
    QTimer timer;

};

#endif // SURGERY_H
