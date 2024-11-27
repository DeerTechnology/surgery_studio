#ifndef NEEDLEPROPERTY_H
#define NEEDLEPROPERTY_H
#include <QString>
#include <qobject.h>
#define StepPerMM 12800 
#define PI 3.1415926

#define MAX_SPEED 12800 //step/s //
#define MIN_SPEED 1 //step/s // 0.0003125mm/s
const double hamilton_inner_diameter[17] = {0.104,0.145,0.206,0.325/*5uL*/,0.34,0.49,0.73,1.03,1.46,2.30,3.26/*500uL*/,4.61,7.29,10.3,14.57,23.03,32.54};
const QString ser [17]  =               {"7000.5","7001","7002","7105","75",    "701","702","705","710","725","750"  , "1001","1002","1005","1010","1025","1050"};
const double hamilton_volume[17] =          {0.5,1,2,5/*0.325*/,5,        10,25,50,100,250,500,1000,2500,5000,10000,25000,50000};//ul
const double SGE_inner_diameter[8] = {0.1,0.15,0.2,0.35,0.5,0.802,1.102,1.598};//{0.343,0.485,0.728,1.03,1.457};
const double SGE_volume[8] = {0.5,1,2,5,10,25,50,100};//ul
typedef enum
{
    _NL = 1,
    _UL,
    _ML
}VolumeUnit;

typedef enum
{
    _MS = 1,
    _S,
    _M,
}TimeUnit;

typedef struct VolumeSpeed
{
    double speed;
    VolumeUnit unit;
    TimeUnit timeUnit;
}VolumeSpeed;

typedef struct NeedlePropertyStruct
{
    double maxVolume;//uL
    double inner_diameter;//mm
    double maxVspeed;//uL/min
    double minVspeed;//uL/min
    double stepVolume;//每步代表的体积uL
}NeedlePropertyStruct;
Q_DECLARE_METATYPE(NeedlePropertyStruct)


typedef enum
{
    _Hamilton = 1,
    _SGE ,
    _Custom
}NeedleType;

class NeedleProperty
{
public:
    NeedleProperty();
    QString name;
    NeedlePropertyStruct get_needle_property(NeedleType needleType,int index,QString name);
    NeedlePropertyStruct get_needle_property(NeedleType needleType,double diameter,double volume,QString name);
};

#endif // NEEDLEPROPERTY_H
