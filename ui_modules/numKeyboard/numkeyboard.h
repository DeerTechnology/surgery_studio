/*!
 *  @brief     自定义数字软键盘
 *  @details
 *  @author    褚伟
 *  @version
 *  @date      2021.7.23
 *  @warning
 *  @copyright
 *  @note
 */
#ifndef NUMKEYBOARD_H
#define NUMKEYBOARD_H

#include <QWidget>
#include <QDebug>
#include <QDialog>
namespace Ui {
class NumKeyboard;
}

class NumKeyboard : public QDialog
{
    Q_OBJECT

public:
    explicit NumKeyboard(QDialog *parent = nullptr);
    ~NumKeyboard();
    void InitUiStyle();
    void SetLineText(QString str);
    void SetLimit(QString reNow);
    void SetLimitRange(float max, float min);
    bool ifSetLimitRange = false;
    float rangeUpper,rangeLower;
    void checkNum();
    void key_set_only_int();
    QString GetLineText();
    void key_allow_negative();
private:
    Ui::NumKeyboard *ui;
    QString strContent;
public slots:
    void slotpushButton_0();
    void slotpushButton_1();
    void slotpushButton_2();
    void slotpushButton_3();
    void slotpushButton_4();
    void slotpushButton_5();
    void slotpushButton_6();
    void slotpushButton_7();
    void slotpushButton_8();
    void slotpushButton_9();
    void slotpushButton_cancel();//取消
    void slotpushButton_sure();//确定
    void slotpushButton_dian();//.
    void slotpushButton_back();//退格
    void slotpushButton_del();//清空
    void slotpushButton_minus();
signals:
    void signalNumKeyboardSure(QString);//按下确定按钮
};

#endif // NUMKEYBOARD_H
