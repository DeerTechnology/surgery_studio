#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QImage>
#include <QPointF>
#include <QMouseEvent>

class CustomLabel : public QLabel
{
    Q_OBJECT

public:
    explicit CustomLabel(QWidget *parent = nullptr);
    void setImage(const QImage& image);
    void zoomIn();
    void zoomOut();
    qreal m_scaleFactor;
    bool ifOpenShowNeedle = false;
    void paintNeedle(QPointF point);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPointF m_mark;
        bool m_hasMark;

    QPointF mapToImage(const QPoint& pos);
    QImage m_image;

    bool m_isDragging;

    QPointF m_dragStartPosition;
    QPointF m_dragLastPosition;
    QRect m_imageRect;
    int m_moveX = 0;
    int m_moveY = 0;
    bool m_paintNeedle = false;

signals:
    void Signal_selectPoint(float x, float y );//对于orignal pic 的像素点
};

#endif // CUSTOMLABEL_H
