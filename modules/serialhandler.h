#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H
#include <QObject>
#include <QCoreApplication>
#include "ui_modules/warning.h"
#include "canbus/canhandler.h"
#define NAME_DEVICE "SP"
#define CANBUS
#define BAUD 115200




class SerialHandler:public QObject
{
    Q_OBJECT
public:
    explicit SerialHandler(QObject *parent = nullptr);

#ifdef CANBUS
    canHandler *canHandlerNow;
#else
    QSerialPort *SerialPortNow;
#endif
    QByteArray  serialRecvBuffer;
    warning waring;
    bool connectState = false;
    bool runningState = false;

    void delaymsec(int msec);
    int send_serial_message(QString strToSend, DeviceType deviceType, int num);
    int auto_connect_serial();
    void send_check_msg();
    void set_connect_state(bool state);
    void init_logic();
    void prase_message(QString strNow);
    //发送移动
    void send_move_msg(int steps, double speed, int8_t ifRelative);
    //发送停止
    void send_stop_move_msg();
    void send_pause_move_msg();
    void send_continue_move_msg();
    //设置当前位置
    void set_location(int x);
    //直接移动
    void send_directly_move_msg(double speed,bool ifForward);
    void send_directly_stop_move_msg();
    //查询状态
    bool send_check_running_msg();

    void close_serial();
    void S_MOVE_handler(QJsonObject jObject);
    void send_online();

signals:
    void Signal_update_location(float x);
    void Signal_change_connect_state(bool state);
//    void Signal_run_state_change(MOVE_STATE state);
    void Signal_run_data(int totalStep,int restStep,double speed);
    void Signal_device_data(DeviceType device_type, int num, QByteArray arry);
private slots:
    void serial_error(QSerialPort::SerialPortError error);
    void read_data();
    void read_data_can(QByteArray arry, DeviceType device_type, int num);
};

#endif // SERIALHANDLER_H
