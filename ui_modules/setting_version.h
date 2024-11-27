#ifndef SETTING_VERSION_H
#define SETTING_VERSION_H

#include <QDialog>

namespace Ui {
class setting_version;
}

class setting_version : public QDialog
{
    Q_OBJECT

public:
    explicit setting_version(QWidget *parent = nullptr);
    ~setting_version();
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_confirm_2_clicked();

    void on_pushButton_cancel_2_clicked();

private:
    Ui::setting_version *ui;
};

#endif // SETTING_VERSION_H
