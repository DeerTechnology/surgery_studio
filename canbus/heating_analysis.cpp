#include "canbus/heating_analysis.h"
#include <QDebug>
#include <QDateTime>
HeatingPad_analysis::HeatingPad_analysis(QObject *parent) : QObject(parent)
{

}

//发送数据初始化
QByteArray HeatingPad_analysis::send_data_init(void * dataInfos, GlobalDataType type)
{
    QList<HeatingPadDataInfo> * datainfos = static_cast<QList<HeatingPadDataInfo> *>(dataInfos);
    //命令开头
    QByteArray res = "FEFE";
    switch (type)
    {
        case GlobalDataType::MasterControl:
            res += "00";
            break;
        case GlobalDataType::SlaveControl:
            res += "01";
            break;
        case GlobalDataType::MasterControl_Fast:
            res += "04";
            break;
        case GlobalDataType::SlaveControl_Fast:
            res += "05";
            break;
    }

    //写入数量
    int size = datainfos->size();
    QString size_str = QString::number(size, 16);
    size = size_str.size();
    for (int i =0; i < 2 - size; i++)
    {
        size_str = "0" + size_str;
    }
    res += size_str;

    //写入类型 和 值
    QString name_str, val_str;
    float_uint f_to_i;
    foreach(HeatingPadDataInfo data, *datainfos)
    {
        QString name = QString::number(data.name, 16);
        int len = name.size();
        for (int i =0; i < 2 - len; i++)
        {
            name = "0" + name;
        }
        name_str += name;

        QString val;
        switch (data.data_type)
        {
            case M_UINT8:
                val = QString::number(data.value.u8_val, 16);
                break;
            case M_INT8:
                val = QString::number(data.value.i8_val, 16);
                break;
            case M_UINT16:
                val = QString::number(data.value.u16_val, 16);
                break;
            case M_INT16:
                val = QString::number(data.value.i16_val, 16);
                break;
            case M_UINT32:
                val = QString::number(data.value.u32_val, 16);
                break;
            case M_UINT64:
                val = QString::number(data.value.u64_val, 16);
                break;
            case M_INT64:
                val = QString::number(data.value.i64_val, 16);
                break;
            case M_INT32:
                val = QString::number(data.value.i32_val, 16);
                break;
            case M_FLOAT32:
                f_to_i.f = data.value.f32_val;
                val = QString::number(f_to_i.i, 16);
                break;
            case M_BOOL:
                val = QString::number(data.value.bool_val, 16);
                break;
            case M_STRING:  //废弃
//                val = QString::number(data.value., 16);
                break;
        }
        len = val.size();
        for (int i =0; i < data.length * 2 - len; i++)
        {
            val = "0" + val;
        }
        val_str += val;
    }
    res += name_str + val_str + "FFFF";

    return res;
}

//初始化时间戳数据
QByteArray HeatingPad_analysis::send_Timestamp_init()
{
    QList<HeatingPadDataInfo> *dataInfos = new QList<HeatingPadDataInfo>;
    HeatingPadDataInfo info1;
    info1.name = CantrolCMD_HeatingPad;
    info1.data_type = M_UINT8;
    info1.length = 1;
    info1.value.u8_val = M_TIME;
    dataInfos->append(info1);

    HeatingPadDataInfo info2;
    info2.name = TimeStamp;
    info2.data_type = M_UINT64;
    info2.length = 8;
    info2.value.u64_val = parseTimestamp();
    dataInfos->append(info2);


    return send_data_init(static_cast<void *>(dataInfos), MasterControl);
}


//接收数据初始化
QList<HeatingPadDataInfo> HeatingPad_analysis::recv_data_init(QByteArray array)
{
    QList<HeatingPadDataInfo> infos;
    int num = array.at(1);
    int len = 0;
    float_uint value;
    for (int i = 0; i < num; i++)
    {
        HeatingPadDataInfo dataInfo;
        dataInfo.name = HeatingPadDataName(array.at(2 + i));

        switch (dataInfo.name)
        {
            case CantrolCMD_HeatingPad://
                dataInfo.data_type = M_BOOL;
                dataInfo.length = 1;
                dataInfo.value.bool_val = array.at(2 + num + len);
                len += dataInfo.length;
                break;
            case ChannelNum:
                dataInfo.data_type = M_BOOL;
                dataInfo.length = 1;
                dataInfo.value.bool_val = array.at(2 + num + len);
                len += dataInfo.length;
                break;
            case TargetTemp: //
                dataInfo.data_type = M_FLOAT32;
                dataInfo.length = 4;
                dataInfo.value.f32_val = *(float *)reversed(array.mid(2 + num + len, 4)).constData();
                len += dataInfo.length;
                break;
            case ifges:		//
                dataInfo.data_type = M_BOOL;
                dataInfo.length = 1;
                len += dataInfo.length;
                dataInfo.value.bool_val = array.at(2 + num + len);
                break;
            case PageID:
                dataInfo.data_type = M_BOOL;
                dataInfo.length = 1;
                len += dataInfo.length;
                dataInfo.value.bool_val = array.at(2 + num + len);
                break;//
        }
        infos.append(dataInfo);
    }
    return infos;
}


QByteArray reversed(QByteArray array)
{
    QByteArray res = "";
    for (int i = array.size() - 1; i >=0; i--)
    {
        res.append(array.at(i));
    }
    return res;
}


uint64_t parseTimestamp() {
    QDateTime dateTime = QDateTime::currentDateTime();
    uint64_t year = dateTime.date().year() * 10000000000000ULL;
    uint64_t month = dateTime.date().month() * 100000000000ULL;
    uint64_t day = dateTime.date().day() * 1000000000ULL;
    uint64_t hour = dateTime.time().hour() * 10000000ULL;
    uint64_t minute = dateTime.time().minute() * 100000ULL;
    uint64_t second = dateTime.time().second() * 1000ULL;
    uint64_t millisecond = dateTime.time().msec();
    return year + month + day + hour + minute + second + millisecond;
}

void formatTimestamp(uint64_t timestamp, char* buffer, size_t bufferSize) {
    uint64_t year = timestamp / 10000000000000ULL;
    timestamp %= 10000000000000ULL;
    uint64_t month = timestamp / 100000000000ULL;
    timestamp %= 100000000000ULL;
    uint64_t day = timestamp / 1000000000ULL;
    timestamp %= 1000000000ULL;
    uint64_t hour = timestamp / 10000000ULL;
    timestamp %= 10000000ULL;
    uint64_t minute = timestamp / 100000ULL;
    timestamp %= 100000ULL;
    uint64_t second = timestamp / 1000ULL;
    timestamp %= 1000ULL;
    uint64_t millisecond = timestamp;
    snprintf(buffer, bufferSize, "%04llu%02llu%02llu%02llu%02llu%02llu%03llu",
             year, month, day, hour, minute, second, millisecond);
}
