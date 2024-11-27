

#ifndef CUSTOM_SCROLLAREA_H
#define CUSTOM_SCROLLAREA_H

#include <QObject>
#include <QScrollArea>
#include <QPoint>

class Custom_ScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    Custom_ScrollArea(QWidget* parent = NULL);
    ~Custom_ScrollArea();

protected:
    bool eventFilter(QObject *obj,QEvent *evt);

private:
    bool mMoveStart;
    bool mContinuousMove;
    QPoint mMousePoint;
};

#endif // MYSCROLLAREA_H
