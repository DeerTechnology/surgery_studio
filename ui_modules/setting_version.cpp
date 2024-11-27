#include "ui_modules/setting_version.h"
#include "ui_setting_version.h"
#include <QStyleOption>
#include <QPainter>
#include <QBitmap>
#include "main.h"
setting_version::setting_version(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setting_version)
{
    ui->setupUi(this);
    setProperty("nav", "setting");
    ui->label_master->setText(__VERSION_MASTER);
    ui->label_slave->setText(__VERSION_SLAVE);
}

setting_version::~setting_version()
{
    delete ui;
}

void setting_version::paintEvent(QPaintEvent *event)
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
    //设置边框为圆角15px
    painter.drawRoundedRect(bmp.rect(), 15, 15);
    setMask(bmp);
    QDialog::paintEvent(event);
}

void setting_version::on_pushButton_confirm_2_clicked()
{
    hide();
}

void setting_version::on_pushButton_cancel_2_clicked()
{
    hide();
}
