#ifndef HEATING_ANALYSIS_H
#define HEATING_ANALYSIS_H

#include <QJsonObject>
#include "canbus/device_analysis.h"
typedef enum{

    M_START,
    M_STOP,
    M_STORAGE,
    M_EXPORT,
    M_TIME,
    M_TEST,
    M_CALI, //温度校准
    S_UPDATETEMP,//更新当前温度
    S_UPDATEBUTTONSTATE,//更新屏幕按钮状态
    S_SWITCHPAGE//切换提示页面
}Cantrol_HeatingPad;

typedef enum{
    CantrolCMD_HeatingPad,	//  控制命令，长度为1，类型为UINT8，值为DATA_TYPE，
    ChannelNum,   //通道号， 长度为1，类型为UINT8，值为0,1,2,。。。。。（这个合成项目只有1个通道，默认值为1）
    TargetTemp, //目标温度, 长度为4，类型为FLOAT32，值例如36.87
    ifges,		//是否存储，长度为1，类型为UINT8，值为0和1，0 - 停止存储，1 - 开始存储
    PageID,		//页面ID，长度为1，类型为UINT8，值为0-（数据导出成功），1-（数据导出中），2-（U盘未出入）
    TimeStamp //时间戳  长度8, 类型为M_UINT64 ()
}HeatingPadDataName;



struct HeatingPadDataInfo {
    HeatingPadDataName name;
    uint8_t length;           // 传输长度
    DeviceDataType data_type;
    DataValue value;     // 实际数据值
};

class HeatingPad_analysis : public QObject
{
    Q_OBJECT
public:
    explicit HeatingPad_analysis(QObject *parent = nullptr);

    QByteArray send_data_init(void *dataInfos, GlobalDataType type);

    QByteArray send_Timestamp_init();

    QList<HeatingPadDataInfo> recv_data_init(QByteArray array);

signals:

public slots:

private:

};

#endif // DEVICE_ANALYSIS_H
