#include "pages/select_worker.h"
#include "ui_select_worker.h"
#include <QStyleOption>
#include <QPainter>
#include <QBitmap>
select_worker::select_worker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::select_worker)
{
    ui->setupUi(this);

    QList<QPushButton*> btns = this->findChildren<QPushButton*>();

    foreach(QPushButton *btn, btns)
    {
        btn->setCheckable(true);
        connect(btn,&QPushButton::clicked,this,&select_worker::btn_clicked);
    }
}

select_worker::~select_worker()
{
    delete ui;
}

QString select_worker::get_worker()
{
    return worker;
}

void select_worker::paintEvent(QPaintEvent *event)
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

void select_worker::btn_clicked()
{
    QPushButton *btn = static_cast<QPushButton *>(sender());
    btn->setChecked(true);
    worker = btn->text();
    this->hide();
}
