#ifndef SYRINGEPUMP_ANALYSIS_H
#define SYRINGEPUMP_ANALYSIS_H

#include <QObject>
#include "canbus/device_analysis.h"
typedef enum{
     M_MOVE_syringe,    // 发送的移动指令   0
     S_MOVE_syringe,      //对移动指令的回应
     M_END_syringe,        //停止
     M_STOP_syringe,        //暂停
     M_CONTINUE_syringe,//继续                 4
     M_SETDATA_syringe,  // 发送的当前位置指令
     S_SETDATA_syringe,  // 对当前位置指令的回应
     M_SIMPLEMOVE_syringe, // 简单移动指    7
     M_GET_syringe,//获取数据
     S_GET_syringe,//返回数据
}Cantrol_SyringePump;

enum SyringePumpDataName
{
    CantrolCMD_Syringe,        //  控制命令，长度为1，类型为UINT8，值为ControlType，
    AlldistanceToGo_syringe,//当前任务需走步数,总步数，长度为4，类型为INT32，
    Speed_syringe,        //移动速度，长度为4，类型为INT32,值为
    IfM_syringe,//移动标志，长度为1，类型为UINT8，
    DistanceToGo_syringe,//剩余步数,长度为4，类型为INT32
    Current_syringe ,//电流，长度为4，类型为float
    Temperature_syringe,//温度，长度为4，类型为float
    Directtion_syringe, //方向（运动模式），长度为1，类型为UINT8，（0-下，注射；1-上，抽取）
    run_statu_syringe,// uint8  1 代表开始运行(MV)  2代表运行结束(ED)
    SP_data_syringe, // uint8   0：数据返回  1 停止移动  2：打开debug  3：关闭debug
    IfR_syringe,        //获取是否为相对移动的标志，长度为1，类型为BOOL
};



 struct SyringePumpDataInfo {
    SyringePumpDataName name;
    uint8_t length;           // 传输长度
    DeviceDataType data_type;
    DataValue value;     // 实际数据值
};



class SyringePump_analysis : public QObject
{
    Q_OBJECT
public:
    explicit SyringePump_analysis(QObject *parent = nullptr);

    QByteArray send_data_init(QList<SyringePumpDataInfo> dataInfos, GlobalDataType type);

    QByteArray send_Timestamp_init();

    QList<SyringePumpDataInfo> recv_data_init(QByteArray array);

signals:

public slots:
};

#endif // SYRINGEPUMP_ANALYSIS_H
