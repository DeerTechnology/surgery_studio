#include "ui_modules/cirlceprogressbar.h"
#include "ui_cirlceprogressbar.h"
#include <QPainter>
cirlceprogressBar::cirlceprogressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cirlceprogressBar)
{
    ui->setupUi(this);
}

cirlceprogressBar::~cirlceprogressBar()
{
    delete ui;
}

void cirlceprogressBar::setValue(int value)
{
    m_value = value;
    if(m_value > 100) m_value = 100;
    else if(m_value<0) m_value = 0;
    update();
}

void cirlceprogressBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QColor(QString("#EEEEEE")));
    QRect rect_1 = this->rect().adjusted(10, 10, -10, -10);
    painter.drawEllipse(rect_1);
    painter.setPen( Qt::NoPen);
    painter.setBrush(QColor(QString("#4DFF69")));
    painter.drawPie(rect_1, 180 * 16, int( - m_value / (m_max - m_min) *360 * 16));
    painter.setBrush(QColor(QString("#000000")));
    painter.drawEllipse(this->rect().adjusted(24, 24, -24, -24));
    painter.setPen( QPen(QColor(QString("#EEEEEE"))));
    painter.drawText(QPointF(60, 60),"进度");
    painter.setPen( QPen(QColor(QString("#07C854"))));
    painter.drawText(QPointF(60, 100),QString("%1%").arg(m_value));

    QWidget::paintEvent(event);
}
