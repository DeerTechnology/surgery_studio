#ifndef CIRLCEPROGRESSBAR_H
#define CIRLCEPROGRESSBAR_H

#include <QWidget>

namespace Ui {
class cirlceprogressBar;
}

class cirlceprogressBar : public QWidget
{
    Q_OBJECT

public:
    explicit cirlceprogressBar(QWidget *parent = nullptr);
    ~cirlceprogressBar();

    void setValue(int value);

    void paintEvent(QPaintEvent *event);

private:
    Ui::cirlceprogressBar *ui;

    double m_value= 0;
    double m_min = 0;
    double m_max = 100;
};

#endif // CIRLCEPROGRESSBAR_H
