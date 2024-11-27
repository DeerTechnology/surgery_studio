#include "ui_modules/tablewidget_radius.h"
#include "ui_tablewidget_radius.h"
#include <QStyleOption>
#include <QPainter>
#include <QBitmap>
tableWidget_radius::tableWidget_radius(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tableWidget_radius)
{
    ui->setupUi(this);
}

tableWidget_radius::~tableWidget_radius()
{
    delete ui;
}

void tableWidget_radius::paintEvent(QPaintEvent *event)
{
    //绘制样式
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//绘制样式

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter painter(&bmp);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    //设置边框为圆角20px
    painter.drawRoundedRect(bmp.rect(), 20, 20);
    setMask(bmp);
    QWidget::paintEvent(event);
}
