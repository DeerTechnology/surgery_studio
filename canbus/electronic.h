#ifndef ELECTRONIC_H
#define ELECTRONIC_H

#include <QObject>
#include "device_analysis.h"

// 定义一个枚举类型DATA_TYPE，用于标识不同的数据类型
typedef enum
{
 M_START_EL, // 移动指令    0
 M_END_EL, // 停止指令    1
 M_SET_EL,//设置坐标轴
 M_RESET_EL,//重置所有坐标轴
 M_UPTATE_EL,//更新速度
 S_AXIS_EL//返回坐标数据 5
}ControlType_EL;

typedef enum{
 CantrolCMD_EL,//  控制命令，长度为1，类型为UINT8，值为 ControlType_EL
 Axis_EL,//坐标轴，长度为1，类型为UINT8，值为 (1：x      2：y     3：z)
 Direction_EL,//方向, 长度为1，类型为UINT8，值为 (0：负     1：正)
 Speed_EL,//速度, 长度为4，类型为FLOAT，值为 ()
 X_EL,//X轴坐标值
 Y_EL,//Y轴坐标值
 Z_EL//Z轴坐标值
}ELDataName;


struct ELDataInfo {
    ELDataName name;
    uint8_t length;           // 传输长度
    DeviceDataType data_type;
    DataValue value;     // 实际数据值
};

class electronic : public QObject
{
    Q_OBJECT
public:
    explicit electronic(QObject *parent = nullptr);
    QByteArray send_data_init(void *dataInfos, GlobalDataType type);
    QList<ELDataInfo> recv_data_init(QByteArray array);

signals:

public slots:
};

#endif // ELECTRONIC_H
