#include "canbus/electronic.h"

electronic::electronic(QObject *parent) : QObject(parent)
{

}

QByteArray electronic::send_data_init(void *dataInfos, GlobalDataType type)
{
    QList<ELDataInfo> * datainfos = static_cast<QList<ELDataInfo> *>(dataInfos);
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
    foreach(ELDataInfo data, *datainfos)
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

QList<ELDataInfo> electronic::recv_data_init(QByteArray array)
{
    QList<ELDataInfo> infos;
    int num = array.at(1);
    int len = 0;
    for (int i = 0; i < num; i++)
    {
        ELDataInfo dataInfo;
        dataInfo.name = ELDataName(array.at(2 + i));

        switch (dataInfo.name)
        {
            case CantrolCMD_EL://
                dataInfo.data_type = M_UINT8;
                dataInfo.length = 1;
                dataInfo.value.u8_val = array.at(2 + num + len);
                len += dataInfo.length;
                break;
            case Axis_EL:
                dataInfo.data_type = M_UINT8;
                dataInfo.length = 1;
                dataInfo.value.u8_val = array.at(2 + num + len);
                len += dataInfo.length;
                break;
            case Direction_EL: //
                dataInfo.data_type = M_UINT8;
                dataInfo.length = 1;
                dataInfo.value.u8_val = array.at(2 + num + len);
                len += dataInfo.length;
                break;
            case Speed_EL: //
                dataInfo.data_type = M_FLOAT32;
                dataInfo.length = 4;
                dataInfo.value.f32_val = array.at(2 + num + len);
                len += dataInfo.length;
                break;
        }
        infos.append(dataInfo);
    }
    return infos;
}
