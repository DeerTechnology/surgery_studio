#ifndef CUSTOM_PROGRESSBAR_H
#define CUSTOM_PROGRESSBAR_H

#include <QObject>
#include <QWidget>
#include <QProgressBar>

class custom_progressbar : public QProgressBar
{
public:
    custom_progressbar(QWidget * parent);
    ~custom_progressbar();
    bool isPerssed();
    void mousePressEvent(QMouseEvent *even);
    void mouseMoveEvent(QMouseEvent *even);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    bool isPress = 0;
};

#endif // CUSTOM_PROGRESSBAR_H
