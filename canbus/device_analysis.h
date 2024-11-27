#ifndef DEVICE_ANALYSIS_H
#define DEVICE_ANALYSIS_H

#include <QObject>

union float_uint
{
    float f;
    uint32_t i;
};

//CAN帧 数据类型
enum GlobalDataType {//后续可以用于数据优先级制定，比如高实时性任务识别到后用低位ID
//格式会区分SDO和PDO以增加CheckBack
    MasterControl, //0x00 control data from master
    SlaveControl, //0x01 control data from slave
    MasterControl_Name,//0x02 control data from master
    SlaveControl_Name,//0x03 control data from slave
    MasterControl_Fast, //0x04 不用接收确认
    SlaveControl_Fast, //0x05 不用接收确认
    MasterOnlineCheck, //0x06 master check
    SlaveOnlineCheck, //0x07 slave check
    DataInit, //0x08 data init //用于控制器向KEY发送初始化
    MasterCheck,
    SlaveCheck
};

//KEY 类型
enum KeyType{
    Can_Uart_Master,//0x00
    Can_Uart_Slave//0x01
};

//所有设备类型
enum DeviceType {
    SyringePump, //0x00     注射泵
    Electronic, //0x01      电动定位仪
    HeatingPad, //0x02      加热垫
    LightDrill, //0x03      冷光源加颅钻
    Handler     //0x04      手柄
};

//错误码
enum ErrorCode {
    None, //0x00
    VersionMismatch, //0x01
    DataInitError,//0x02
    UnknownDevice,//0x02
    SlaveCheckError,//0x03
    DataCRCError,//0x04
};

enum DeviceDataType {
    M_UINT8,
    M_INT8,
    M_UINT16,
    M_INT16,
    M_UINT32,
    M_UINT64,
    M_INT64,
    M_INT32,
    M_FLOAT32,
    M_BOOL,
    M_STRING,//不建议
};

typedef union DataValue {
    uint8_t u8_val;
    int8_t i8_val;
    uint16_t u16_val;
    int16_t i16_val;
    uint32_t u32_val;
    int32_t i32_val;
    uint64_t u64_val;
    int64_t i64_val;
    float f32_val;
    bool bool_val;
    char str[4];  // 为字符串预留固定空间 根据项目定 可以用Define 去定义
} DataValue;//4字节


uint64_t parseTimestamp();
void formatTimestamp(uint64_t timestamp, char* buffer, size_t bufferSize);
QByteArray reversed(QByteArray array);
#endif // DEVICE_ANALYSIS_H
