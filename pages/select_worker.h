#ifndef SELECT_WORKER_H
#define SELECT_WORKER_H

#include <QDialog>

namespace Ui {
class select_worker;
}

class select_worker : public QDialog
{
    Q_OBJECT

public:
    explicit select_worker(QWidget *parent = nullptr);
    ~select_worker();
    QString get_worker();

    void paintEvent(QPaintEvent *event);
private slots:
    void btn_clicked();

private:
    Ui::select_worker *ui;
    QString worker = "";
};

#endif // SELECT_WORKER_H
