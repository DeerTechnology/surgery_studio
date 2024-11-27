#ifndef HANDLER_ANALYSIS_H
#define HANDLER_ANALYSIS_H

#include <QObject>
#include "device_analysis.h"

// 定义一个枚举类型DATA_TYPE，用于标识不同的数据类型
typedef enum
{
   S_MOVE_HANDLER,// 发送的移动指令   0
   S_END_HANDLER,//停止
   S_SPEED_HANDLER//速度
}ControlType_HANDLER;

typedef enum{
    CantrolCMD_HANDLER,// 控制命令，长度为1，类型为UINT8
    Axis_HANDLER,//坐标轴 长度为1，类型为UINT8   值为 (1：x      2：y     3：z)
    Direction_HANDLER, //方向  长度为1，类型为UINT8   值为 (0：负   1：正)
    Speed_HANDLER//加减速  长度为1，类型为UINT8    值为 (0：减速    1：加速)
}HandlerDataName;



struct HandlerDataInfo {
    HandlerDataName name;
    uint8_t length;           // 传输长度
    DeviceDataType data_type;
    DataValue value;     // 实际数据值
};

class handler_analysis : public QObject
{
    Q_OBJECT
public:
    explicit handler_analysis(QObject *parent = nullptr);
    QByteArray send_data_init(void *dataInfos, GlobalDataType type);
    QList<HandlerDataInfo> recv_data_init(QByteArray array);

signals:

public slots:
};

#endif // HANDLER_ANALYSIS_H
