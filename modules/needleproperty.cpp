#include "needleproperty.h"
#include "math.h"
#include <QDebug>
NeedleProperty::NeedleProperty()
{

}

NeedlePropertyStruct NeedleProperty::get_needle_property(NeedleType needleType,int index,QString name)
{
    NeedlePropertyStruct needleProperty;
    if(needleType == _Hamilton)
    {
        QString nameNow = "Hamilton-"+name;
        this->name = nameNow;
        needleProperty.inner_diameter = hamilton_inner_diameter[index];
        needleProperty.maxVolume = hamilton_volume[index];
        needleProperty.maxVspeed = (double)MAX_SPEED/(double)StepPerMM * PI * pow((needleProperty.inner_diameter)/2,2) * 60 ;
        qDebug()<<"max speed"<< MAX_SPEED << "/" << StepPerMM << "*" << PI << "*" << "(" << needleProperty.inner_diameter <<"/2)^2" << "*" << 60 << "=" << needleProperty.maxVspeed;
        needleProperty.minVspeed = (double)MIN_SPEED/(double)StepPerMM * PI * pow((needleProperty.inner_diameter)/2,2) * 60 ;
        qDebug()<<"min speed "<< MIN_SPEED << "/" << StepPerMM << "*" << PI << "*" << "(" << needleProperty.inner_diameter <<"/2)^2" << "*" << 60 << "=" << needleProperty.minVspeed;

        needleProperty.stepVolume =1/( StepPerMM/(PI * pow(needleProperty.inner_diameter/2,2)) );
    }
    else if(needleType == _SGE)
    {
        QString nameNow = "SGE-"+name;
        this->name = nameNow;
        needleProperty.inner_diameter = SGE_inner_diameter[index];
        needleProperty.maxVolume = SGE_volume[index];
        needleProperty.maxVspeed = (double)MAX_SPEED/(double)StepPerMM * PI * pow((needleProperty.inner_diameter)/2,2) * 60 ;
        qDebug()<<"max speed"<< MAX_SPEED << "/" << StepPerMM << "*" << PI << "*" << "(" << needleProperty.inner_diameter <<"/2)^2" << "*" << 60 << "=" << needleProperty.maxVspeed;
        needleProperty.minVspeed = (double)MIN_SPEED/(double)StepPerMM * PI * pow((needleProperty.inner_diameter)/2,2) * 60 ;
        qDebug()<<"min speed "<< MIN_SPEED << "/" << StepPerMM << "*" << PI << "*" << "(" << needleProperty.inner_diameter <<"/2)^2" << "*" << 60 << "=" << needleProperty.minVspeed;
        needleProperty.stepVolume = 1/( StepPerMM/(PI * pow(needleProperty.inner_diameter/2,2)));
    }

    return needleProperty;
}

NeedlePropertyStruct NeedleProperty::get_needle_property(NeedleType needleType,double diameter,double volume,QString name)
{

    NeedlePropertyStruct needleProperty;
    QString nameNow = name+"-"+QString::asprintf("%.2f",volume);
    this->name = nameNow;
    needleProperty.inner_diameter = diameter;
    needleProperty.maxVolume = volume;
    needleProperty.maxVspeed = (double)MAX_SPEED/(double)StepPerMM * PI * pow((needleProperty.inner_diameter)/2, 2) * 60 ;
    needleProperty.minVspeed = (double)MIN_SPEED/(double)StepPerMM * PI * pow((needleProperty.inner_diameter)/2, 2) * 60 ;
    needleProperty.stepVolume = 1/( StepPerMM/(PI * pow(needleProperty.inner_diameter/2,2)) );
    return needleProperty;
}
