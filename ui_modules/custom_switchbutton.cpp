#include "custom_switchbutton.h"
#include <QPainter>
#include <QPainterPath>




Custom_SwitchButton::Custom_SwitchButton(QWidget *parent) : QWidget(parent)
{
    m_space = 2;
    m_radius = 5;
    m_checked = false;
    m_showText = true;
    m_showText = false;
    m_animation = true;

    m_bgColorOn = QColor(21, 156, 119);
    m_bgColorOff = QColor(111, 122, 126);

    m_sliderColorOn = QColor(255, 255, 255);
    m_sliderColorOff = QColor(255, 255, 255);

    m_textColor = QColor(255, 255, 255);
    m_textOn = "";
    m_textOff = "";
    m_step = 0;
    m_startX = 0;
    m_endX = 0;

    m_timer = new QTimer(this);
    m_timer->setInterval(30);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateValue()));
}


void Custom_SwitchButton::drawBackGround(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    QColor bgColor = m_checked ? m_bgColorOn : m_bgColorOff;
    if (isEnabled()) {
        bgColor.setAlpha(60);
    }

    painter->setBrush(bgColor);

    QRect rect(0, 0, width(), height());
    int side = qMin(width(), height());

    //左侧半圆
    QPainterPath path1;
    path1.addEllipse(rect.x(), rect.y(), side, side);

    //右侧半圆
    QPainterPath path2;
    path2.addEllipse(rect.width() - side, rect.y(), side, side);

    //中间的矩形
    QPainterPath path3;
    path3.addRect(rect.x() + side / 2, rect.y(), rect.width() - side, height());

    QPainterPath path = path1 + path2 + path3;
    painter->drawPath(path);

    //绘制文本

    //滑块半径
    int sliderWidth = qMin(height(), width()) - m_space * 2 - 5;
    if (m_checked){
        QRect textRect(0, 0, width() - sliderWidth, height());
        painter->setPen(QPen(m_textColor));
        painter->drawText(textRect, Qt::AlignCenter, m_textOn);
    } else {
        QRect textRect(sliderWidth, 0, width() - sliderWidth, height());
        painter->setPen(QPen(m_textColor));
        painter->drawText(textRect, Qt::AlignCenter, m_textOff);
    }

    painter->restore();
}

void Custom_SwitchButton::drawSlider(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    QColor color = m_checked ? m_sliderColorOn : m_sliderColorOff;

    painter->setBrush(QBrush(color));

    int sliderWidth = qMin(width(), height()) - m_space * 2;
    QRect rect(m_space + m_startX, m_space, sliderWidth, sliderWidth);
    painter->drawEllipse(rect);

    painter->restore();
}

void Custom_SwitchButton::paintEvent(QPaintEvent *ev)
{
    //启用反锯齿
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制背景
    drawBackGround(&painter);

    //绘制滑块
    drawSlider(&painter);
}

void Custom_SwitchButton::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)

    m_checked = !m_checked;
    this->status = m_checked;
    emit statusChanged(m_checked);

    //计算步长
    m_step = width() / 10;

    //计算滑块X轴终点坐标
    if (m_checked) {
        m_endX = width() - height();
    } else {
        m_endX = 0;
    }

    //判断是否使用动画
    if (m_animation) {
        m_timer->start();
    } else{
        m_startX = m_endX;
        update();
    }
}

void Custom_SwitchButton::updateValue()
{
    if (m_checked) {
        if (m_startX < m_endX) {
            m_startX += m_step;
        } else {
            m_startX = m_endX;
            m_timer->stop();
        }
    } else {
        if (m_startX > m_endX) {
            m_startX -= m_step;
        } else {
            m_startX = m_endX;
            m_timer->stop();
        }
    }

    update();
}

int Custom_SwitchButton::space() const
{
    return m_space;
}

int Custom_SwitchButton::radius() const
{
    return m_radius;
}

bool Custom_SwitchButton::checked() const
{
    return m_checked;
}

bool Custom_SwitchButton::showText() const
{
    return m_showText;
}

bool Custom_SwitchButton::showCircel() const
{
    return m_showCircle;
}

bool Custom_SwitchButton::animation() const
{
    return m_animation;
}

QColor Custom_SwitchButton::bgColorOn() const
{
    return m_bgColorOn;
}

QColor Custom_SwitchButton::bgColorOff() const
{
    return m_bgColorOff;
}

QColor Custom_SwitchButton::sliderColorOn() const
{
    return m_sliderColorOn;
}

QColor Custom_SwitchButton::sliderColorOff() const
{
    return m_sliderColorOff;
}

QColor Custom_SwitchButton::textColor() const
{
    return m_textColor;
}

QString Custom_SwitchButton::textOn() const
{
    return m_textOn;
}

QString Custom_SwitchButton::textOff() const
{
    return m_textOff;
}

int Custom_SwitchButton::step() const
{
    return m_step;
}

int Custom_SwitchButton::startX() const
{
    return m_startX;
}

int Custom_SwitchButton::endX() const
{
    return m_endX;
}

void Custom_SwitchButton::setSpace(int space)
{
    if (m_space != space) {
        m_space = space;
        update();
    }
}

void Custom_SwitchButton::setRadius(int radius)
{
    if (m_radius != radius) {
        m_radius = radius;
        update();
    }
}

void Custom_SwitchButton::setChecked(bool checked)
{
    if (m_checked != checked) {
        m_checked = checked;
        if(checked==true)
        {
            m_startX = width() - height();
        }
        else
        {
            m_startX = 0;
        }
        update();
    }
}

void Custom_SwitchButton::setShowText(bool show)
{
    if (m_showText != show) {
        m_showText = show;
        update();
    }
}

void Custom_SwitchButton::setShowCircle(bool show)
{
    if (m_showCircle != show) {
        m_showCircle = show;
        update();
    }
}

void Custom_SwitchButton::setAnimation(bool ok)
{
    if (m_animation != ok) {
        m_animation = ok;
        update();
    }
}

void Custom_SwitchButton::setBgColorOn(const QColor &color)
{
    if (m_bgColorOn != color) {
        m_bgColorOn = color;
        update();
    }
}

void Custom_SwitchButton::setBgColorOff(const QColor &color)
{
    if (m_bgColorOff != color) {
        m_bgColorOff = color;
        update();
    }
}

void Custom_SwitchButton::setSliderColorOn(const QColor &color)
{
    if (m_sliderColorOn != color) {
        m_sliderColorOn = color;
        update();
    }
}

void Custom_SwitchButton::setSliderColorOff(const QColor &color)
{
    if (m_sliderColorOff != color) {
        m_sliderColorOff = color;
        update();
    }
}

void Custom_SwitchButton::setTextColor(const QColor &color)
{
    if (m_textColor != color) {
        m_textColor = color;
        update();
    }
}

void Custom_SwitchButton::setTextOn(const QString &text)
{
    if (m_textOn != text) {
        m_textOn = text;
        update();
    }
}

void Custom_SwitchButton::setTextOff(const QString &text)
{
    if (m_textOff != text) {
        m_textOff = text;
        update();
    }
}

//void SwitchButton::setStep(int step)
//{
//    if (m_step != step) {
//        m_step = step;
//        update();
//    }
//}

//void SwitchButton::setStartX(int startX)
//{

//}

//void SwitchButton::setEndX(int endX)
//{

//}




/*
 * old
 */
Custom_SwitchButton_old::Custom_SwitchButton_old(QWidget *parent) :
    QWidget(parent)
{
    //resize(100,60); // 默认100，60宽高
    m_bActive = false; // 默认未激活
    m_fStep = width() / 15.0;
    m_fCurrent = 2.0f;
    m_nArcRadius = std::min(width(), height()); // 默认半径
    m_nRectWidth = width() - m_nArcRadius;
    m_colorActive = Qt::green;
    m_colorInactive = Qt::red;
    m_colorSlider = Qt::white;
    setCursor(QCursor(Qt::PointingHandCursor));
    connect(&m_timer, &QTimer::timeout, this, &Custom_SwitchButton_old::SwitchAnimation);
}

Custom_SwitchButton_old::~Custom_SwitchButton_old()
{
}

void Custom_SwitchButton_old::SetSize(int nWidth, int nHeight)
{
    resize(nWidth, nHeight);
    m_nArcRadius = std::min(width(), height());
    m_nRectWidth = width() - m_nArcRadius;
    m_fStep = width() / 15.0;
}

void Custom_SwitchButton_old::SetActiveColor(QColor color)
{
    m_colorActive = color;
}

void Custom_SwitchButton_old::SetInactiveColor(QColor color)
{
    m_colorInactive = color;
}

void Custom_SwitchButton_old::SetSliderColor(QColor color)
{
    m_colorSlider = color;
}

void Custom_SwitchButton_old::SetStatus(bool bActive)
{
    if(m_bActive == bActive) return;
    m_bActive = bActive;
    if(m_bActive) m_fCurrent = width() - m_nArcRadius;
    else m_fCurrent = 2.0f;
    update();
}

bool Custom_SwitchButton_old::GetStatus()
{
    return m_bActive;
}

void Custom_SwitchButton_old::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);
    if(m_bActive) p.setBrush(QBrush(m_colorActive));
    else p.setBrush(QBrush(m_colorInactive));

    QPainterPath leftPath;
    leftPath.addEllipse(0, 0, m_nArcRadius, m_nArcRadius);

    QPainterPath middlePath;
    middlePath.addRect(m_nArcRadius / 2, 0, m_nRectWidth, m_nArcRadius);

    QPainterPath rightPath;
    rightPath.addEllipse(m_nRectWidth, 0, m_nArcRadius, m_nArcRadius);

    QPainterPath path = leftPath + middlePath + rightPath;

    p.drawPath(path);

    DrawSlider(&p);

    p.end();
}

void Custom_SwitchButton_old::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_bActive = !m_bActive;
    m_timer.start(15);
}

void Custom_SwitchButton_old::DrawSlider(QPainter *p)
{
    p->setBrush(QBrush(m_colorSlider));
    p->drawEllipse(m_fCurrent, 1, m_nArcRadius - 2, m_nArcRadius - 2);
}

void Custom_SwitchButton_old::SwitchAnimation()
{
    if(!m_bActive)
    {
        m_fCurrent -= m_fStep;
        if(m_fCurrent <= 2.0f)
        {
            m_fCurrent = 2.0f;
            m_timer.stop();
        }
    }
    else
    {
        m_fCurrent += m_fStep;
        if(m_fCurrent >= width() - m_nArcRadius)
        {
            m_fCurrent = width() - m_nArcRadius;
            m_timer.stop();
        }
    }
    update();
}
