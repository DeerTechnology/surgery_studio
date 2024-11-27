#ifndef CANHANDLER_H
#define CANHANDLER_H

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QVector>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "canbus/heating_analysis.h"
enum CanStreamType {
    Single = 0x00, //0x00
    First = 0x10, //0x10
    Consecutive = 0x20,//0x20
    Flow = 0x30 //0x30
};

struct deviceInfo
{
    DeviceType device_type;
    int device_id;
    int version;
    int count;
    uint Can_id;
    QString message;
    bool statu;
};

typedef enum {
    _START =1,
    _END
}MOVE_STATE;

typedef struct //LocationNow
{
    float x;
    float y;
    float z;
    //LocationNow()
    //{
    //    x = 0;
    //    y = 0;
    //    z = 0;
    //}
}LOCATION;//umBase
extern LOCATION globalLocNow ;
void set_globalLocNow(LOCATION locNow);
LOCATION get_globalLocNow();

typedef struct
{
    QString comName;
    QString comDiscribe;
    QSerialPortInfo infoNow;
}SerialData;
Q_DECLARE_METATYPE(SerialData)

class canHandler:public QObject
{
    Q_OBJECT
public:
    canHandler(QObject *parent = nullptr);
    virtual ~canHandler();
    bool init();
    int send_frame(QString array, DeviceType deviceType, int device_id);
    bool send_check(DeviceType deviceType, int device_id);
    QList<QCanBusFrame> arry_to_frame(QString message, int can_id);
    QByteArray frame_to_arry(QCanBusFrame frame, int *count);
    QCanBusFrame onLine_init(QByteArray arry, int canid);
    QString frame_to_str(QCanBusFrame frame);
    QString add_bytes(bool statu, QString array, QString byte, int tail);
    QCanBusFrame str_to_frame(QByteArray str);
    void online_check();
public slots:
    void read_frame();
    void task_handler();
    void can_erro(QCanBusDevice::CanBusError error);
    void read_data();
signals:
    QByteArray recv_frame(QByteArray arry, DeviceType, int num);
    void error();

private:
    QCanBusDevice *canDevice;
    bool first = true;
    QList<deviceInfo *> devices;
    bool status = false, canRead = false;
    QTimer timer_handler, timer_online;
    QVector<QCanBusFrame> m_frames;
    QSerialPort *SerialPortNow;
};

#endif // CANHANDLER_H
