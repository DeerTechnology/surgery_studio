#ifndef CUSTOM_LINEEDIT_H
#define CUSTOM_LINEEDIT_H
#include <QLineEdit>
#include <QMouseEvent>
#include "numkeyboard.h"
class Custom_LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit Custom_LineEdit(QWidget *parent = 0);
    ~Custom_LineEdit();
    void set_num_key();

    void limit_hour();
    void limit_min();
    void set_limit(QString reNow);
    void set_limit_range_vitrual(float max,float min);//用于虚拟键盘
    void set_only_int();
    void allow_negative();
private:
    // NumKeyboard *numKeyWidget;
    NumKeyboard *numKeyWidget;
    bool ifUseKey = 0;
protected:
    //添加并重写鼠标点击事件函数
    virtual void mousePressEvent(QMouseEvent *e);
signals:
    //点击信号函数
    void virtualKeyConfirm(QString);
    void clicked();
public slots:
    void slot_open_key();
    void slot_key_confirm(QString);
};

#endif // CUSTOM_LINEEDIT_H
