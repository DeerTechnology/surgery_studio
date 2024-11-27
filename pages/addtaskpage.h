#ifndef ADDTASKPAGE_H
#define ADDTASKPAGE_H

#include <QDialog>
#include <QEvent>
#include "../modules/taskhandler.h"
#include "../modules/needleproperty.h"
#include "ui_modules/warning.h"
namespace Ui {
class AddTaskPage;
}

class AddTaskPage : public QDialog
{
    Q_OBJECT

public:
    warning warner;
    explicit AddTaskPage(QWidget *parent = nullptr);
    ~AddTaskPage();
    void init_ui();
    void init_logic();
    bool hasNeedle = false;
    NeedlePropertyStruct needleProperty;
    bool check_data(double volume ,double speed );
    void paintEvent(QPaintEvent *event) override;
private slots:


    void on_toolButton_clicked();

private:
    Ui::AddTaskPage *ui;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void Signal_add_task(TaskType taskType,double volume ,double speed ,double pauseTime);
};

#endif // ADDTASKPAGE_H
