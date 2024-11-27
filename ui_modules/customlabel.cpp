// customlabel.cpp

#include "customlabel.h"
#include <QPainter>
#include <QPointF>
#include <QDebug>

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent), m_scaleFactor(1.0), m_isDragging(false)
{
    setScaledContents(false);
    setAlignment(Qt::AlignCenter);
    setMouseTracking(true);
}

void CustomLabel::paintNeedle(QPointF point)
{
    m_paintNeedle = true;
    m_mark = point;
    m_hasMark = true;
    update();
}
void CustomLabel::setImage(const QImage &image)
{
    m_image = image;
    m_imageRect = QRect(QPoint(0, 0), m_image.size());
    // this->zoomIn();
    m_moveX = m_imageRect.left() * m_scaleFactor;
    m_moveY = m_imageRect.top() * m_scaleFactor; // 实时更新偏移量
    update();
}

void CustomLabel::zoomOut()
{
    m_scaleFactor *= 1.1;
    update();
}

void CustomLabel::zoomIn()
{
    QSize scaledSize = m_image.size() * m_scaleFactor;
    QSize labelSize = size();
    // qDebug() << labelSize;
    // qDebug() << scaledSize;

    if (labelSize.width() < scaledSize.width() * 1.05 || labelSize.height() < scaledSize.height() * 1.05)
    {
        m_scaleFactor /= 1.1;
    }
    update();
}

void CustomLabel::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = true;
        m_dragStartPosition = event->pos();
        m_dragLastPosition = m_dragStartPosition;

        // 将点击坐标转换为图像坐标
        // QPointF imagePoint = mapToImage(event->pos());

        // 输出图像的当前尺寸和原始尺寸
        // qDebug() << "Current Image Size:" << m_imageRect.size() * m_scaleFactor;
        // qDebug() << "Original Image Size:" << m_imageRect.size();

        // 输出图像的位置
        // int imagex = m_imageRect.left();
        // int imagey = m_imageRect.top();
        // qDebug() << "Image Position:" << mapToParent(m_imageRect.topLeft());

        // 计算点击点在原始图像中的坐标
        // int x = static_cast<int>((imagePoint.x() - m_imageRect.left()) / m_scaleFactor);
        // int y = static_cast<int>((imagePoint.y() - m_imageRect.top()) / m_scaleFactor);
        // qDebug() << "Clicked Pixel Coordinate: (" << x << ", " << y << ")";

        if (ifOpenShowNeedle)
        {
            return;
        }
        m_mark = event->pos();
        // qDebug() <<  m_mark;
        m_hasMark = true;
    }
}

void CustomLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging)
    {
        QPointF delta = event->pos() - m_dragLastPosition;
        m_dragLastPosition = event->pos();

        // 根据拖动的距离调整图像的位置
        int dx = delta.x() / m_scaleFactor;
        int dy = delta.y() / m_scaleFactor;

        // 计算图像移动后的位置
        QRectF imageRect = m_imageRect.translated(-dx, -dy);
        QSize scaledSize = m_image.size() * m_scaleFactor;
        QSize labelSize = size();
        if (labelSize.width() > scaledSize.width() * 1.0 && labelSize.height() > scaledSize.height() * 1.0)
        {
            // qDebug()<<"small view";
            this->setText("^_^");
            int x = (labelSize.width() - scaledSize.width());
            int y = (labelSize.height() - scaledSize.height()) / 2;
            // 获取 QLabel 的边界矩形
            QRectF labelRect = rect();
            // qDebug()<<labelRect.bottom();
            // qDebug()<<imageRect.top()+scaledSize.height();
            // qDebug()<<imageRect.bottom() * m_scaleFactor + y;
            // qDebug()<< x;
            //  将图像的位置限制在 QLabel 内部
            if (imageRect.left() > labelRect.left())
            {
                dx += imageRect.left() - labelRect.left();
            }
            if (imageRect.top() > labelRect.top())
            {
                dy += imageRect.top() - labelRect.top();
            }
            int xNow = imageRect.right() * m_scaleFactor + x;
            if (xNow < labelRect.right())
            {
                dx -= labelRect.right() - xNow;
            }
            if (imageRect.bottom() < labelRect.bottom())
            {
                dy -= labelRect.bottom() - imageRect.bottom();
            }
        }
        else
        {
            this->setText("");
        }

        // 更新图像位置
        m_imageRect.translate(-dx, -dy);
        m_moveX = imageRect.left() * m_scaleFactor;
        m_moveY = imageRect.top() * m_scaleFactor;
        // qDebug()<<imageRect.top()*m_scaleFactor;
        // qDebug()<<imageRect.left()*m_scaleFactor;
        update();
    }

    m_hasMark = false;
}
void CustomLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = false;
        update();
    }
}

void CustomLabel::wheelEvent(QWheelEvent *event)
{

    if (event->angleDelta().y() > 0)
        zoomOut();
    else
        zoomIn();
    m_moveX = m_imageRect.left() * m_scaleFactor;
    m_moveY = m_imageRect.top() * m_scaleFactor; // 实时更新偏移量

    m_hasMark = false;
    update();
}

void CustomLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    update();
}

void CustomLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    if (m_image.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 根据当前的缩放级别和 QLabel 的大小计算缩放后的图像大小
    QSize scaledSize = m_image.size() * m_scaleFactor;
    QSize labelSize = size();
    // 计算图像在 QLabel 中的位置
    int x = (labelSize.width() - scaledSize.width()) / 2;
    int y = (labelSize.height() - scaledSize.height()) / 2;
    // if(x>0) x = 0;
    // if(y>0) y = 0;
    QRect imageRect(QPoint(x, y), scaledSize);
    // qDebug()<<QPoint(x, y);
    int leftPicX = x - m_moveX;
    int topPicY = y - m_moveY;
    // qDebug()<<QPoint(leftPicX, topPicY);
    // qDebug()<<QPoint(x+scaledSize.width(), y+scaledSize.height());
    //  绘制图像
    painter.drawImage(imageRect, m_image, m_imageRect);
    // painter.setPen(Qt::red);
    // painter.drawRect(rect());

    if (m_hasMark && !m_isDragging)
    {
        if (m_paintNeedle)
        {
            painter.setPen(QPen(Qt::red, 3));
            int markSize = 10;
            int markHalfSize = markSize / 2;
            //int x = m_mark.x();
            //int y = m_mark.y();
            
            // painter.setPen(Qt::red);
            // painter.drawEllipse(m_mark, 5, 5);
            int paintX = (float)(m_mark.x() - leftPicX) / m_scaleFactor;
            int paintY = (float)(m_mark.y() - topPicY) / m_scaleFactor;
            int x = m_mark.x()*m_scaleFactor + leftPicX;
            int y = m_mark.y()*m_scaleFactor + topPicY;
            // qDebug()<<"current point ";
            painter.drawLine(x - markSize, y - markSize, x , y );
            painter.drawLine(x , y , x + markSize, y - markSize);
            painter.drawLine(x , y , x , y - 4* markSize);
            qDebug() << "Clicked Pixel Coordinate: (" << x << ", " << y << ")";
            qDebug() << m_scaleFactor;
        }
        else
        {
            painter.setPen(QPen(Qt::red, 2));
            int markSize = 10;
            int markHalfSize = markSize / 2;
            int x = m_mark.x();
            int y = m_mark.y();
            painter.drawLine(x - markHalfSize, y - markHalfSize, x + markHalfSize, y + markHalfSize);
            painter.drawLine(x - markHalfSize, y + markHalfSize, x + markHalfSize, y - markHalfSize);
            // painter.setPen(Qt::red);
            // painter.drawEllipse(m_mark, 5, 5);
            float paintX = (float)(m_mark.x() - leftPicX) / m_scaleFactor;
            float paintY = (float)(m_mark.y() - topPicY) / m_scaleFactor;
            // qDebug()<<"current point ";
            qDebug() << "Clicked Pixel Coordinate: (" << paintX << ", " << paintY << ")";
            qDebug() << m_scaleFactor;
            emit Signal_selectPoint(paintX, paintY);
        }
    }
}
QPointF CustomLabel::mapToImage(const QPoint &pos)
{
    QSize labelSize = size();
    QSize scaledSize = m_image.size() * m_scaleFactor;

    int x = (labelSize.width() - scaledSize.width()) / 2;
    int y = (labelSize.height() - scaledSize.height()) / 2;

    QPointF imagePoint = QPointF(pos.x() - x, pos.y() - y) / m_scaleFactor;

    return imagePoint;
}
