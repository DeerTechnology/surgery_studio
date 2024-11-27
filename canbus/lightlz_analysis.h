#ifndef LIGHTLZ_ANALYSIS_H
#define LIGHTLZ_ANALYSIS_H

#include <QObject>

#include "device_analysis.h"
typedef enum
{
    M_START_LIGHT,    // 开启冷光源
    M_END_LIGHT,      //关闭冷光源
    M_START_LZ,    // 开启卢钻
    M_END_LZ,      //关闭卢钻
    M_UPDATE_LIGHT,    // 更新冷光源强度
    M_UPDATE_LZ,    // 更新卢钻源强度
}LLControlType;

typedef enum{
    CantrolCMD_LL,        //  控制命令，长度为1，类型为UINT8，值为ControlType，
    LightValue_LL,        //  光照强度，长度为1，类型为UINT8，值为 1 - 100，
    Speed_LL ,       // 卢钻速度，长度为1，类型为UINT8，值为 1 - 100，
    Color_LL,             //色温，长度为1，类型为UINT8，值为0(白) 1(黄)
    Direction_LL         //转向，长度为1，类型为UINT8，值为0(顺时针) 1(逆时针)
}LLDataName;



struct LLDataInfo {
   LLDataName name;
   uint8_t length;           // 传输长度
   DeviceDataType data_type;
   DataValue value;     // 实际数据值
};

class LightLz_analysis : public QObject
{
    Q_OBJECT
public:
    explicit LightLz_analysis(QObject *parent = nullptr);


    QByteArray send_data_init(void *dataInfos, GlobalDataType type);

    QByteArray send_Timestamp_init();

    QList<LLDataInfo> recv_data_init(QByteArray array);

signals:

public slots:
};

#endif // LIGHTLZ_ANALYSIS_H
