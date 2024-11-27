#include "ui_modules/warning.h"
#include "ui_warning.h"
#include <QStyleOption>
#include <QPainter>
#include <QBitmap>
warning::warning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::warning)
{
    ui->setupUi(this);
}

warning::~warning()
{
    delete ui;
}

void warning::setText(QString text)
{
    ui->label_text->setText(text);
}

void warning::paintEvent(QPaintEvent *event)
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

void warning::on_pushButton_clicked()
{
    hide();
}
