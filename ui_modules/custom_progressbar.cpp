#include "ui_modules/custom_progressbar.h"
#include <QDebug>
#include <QMouseEvent>
custom_progressbar::custom_progressbar(QWidget *parent):QProgressBar (parent)
{

}

custom_progressbar::~custom_progressbar()
{

}

bool custom_progressbar::isPerssed()
{
    return isPress;
}

void custom_progressbar::mousePressEvent(QMouseEvent *even)
{
    isPress = true;
}

void custom_progressbar::mouseMoveEvent(QMouseEvent *even)
{
    if(isPress)
    {
        setValue(even->pos().x() * this->maximum() / size().width());
    }
}

void custom_progressbar::mouseReleaseEvent(QMouseEvent *event)
{
    isPress = false;
}
