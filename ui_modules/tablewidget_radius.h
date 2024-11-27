#ifndef TABLEWIDGET_RADIUS_H
#define TABLEWIDGET_RADIUS_H

#include <QWidget>

namespace Ui {
class tableWidget_radius;
}

class tableWidget_radius : public QWidget
{
    Q_OBJECT

public:
    explicit tableWidget_radius(QWidget *parent = nullptr);
    ~tableWidget_radius();
    void paintEvent(QPaintEvent *event);

private:
    Ui::tableWidget_radius *ui;
};

#endif // TABLEWIDGET_RADIUS_H
