#include "custom_lineedit.h"
#include "QRegularExpressionValidator"
#include "main.h"
Custom_LineEdit::Custom_LineEdit(QWidget *parent) : QLineEdit(parent)
{
    // num only
    // QRegExp exp("([0-9]{0,3}[\\.][0-9]{1,1})");
    // QRegExp regx("[a-zA-Z0-9\-_)(#]{1,25}");
    // QRegExp exp("[0-9\\.-]+$");

    // QIntValidator *intVal = new QIntValidator();
    // intVal->setRange(10, 23);
    // this->setValidator(intVal);
#ifdef __TOUCHSCREEN
    this->set_num_key();

#endif
}

Custom_LineEdit::~Custom_LineEdit()
{

}

void Custom_LineEdit::mousePressEvent(QMouseEvent *e)
{
    // 如果单击了就触发clicked信号
    if (e->button() == Qt::LeftButton)
    {
        // 触发clicked信号
        emit clicked();
        if (ifUseKey)
        {
            this->slot_open_key();
        }
        if (!this->isReadOnly())
        {
            // this->setText("");
            this->selectAll();//可能不需要全选
        }
    }

    // 将该事件传给父类处理
    //  QLineEdit::mousePressEvent(e);
}
void Custom_LineEdit::set_only_int()
{
    if (ifUseKey)
    {
        numKeyWidget->key_set_only_int();
    }
}
void Custom_LineEdit::allow_negative()
{
    if (ifUseKey)
    {
        numKeyWidget->key_allow_negative();
    }
}
void Custom_LineEdit::limit_hour()
{
    this->set_only_int();
    QRegExp rx("^([0-9]|1[0-9]|2[0-3])$");
    QValidator *Validator = new QRegExpValidator(rx);
    this->setValidator(Validator);
    if (ifUseKey)
    {
        numKeyWidget->SetLimit("^([0-9]|1[0-9]|2[0-3])$");
    }
}
void Custom_LineEdit::limit_min()
{
    this->set_only_int();
    QRegExp rx("^([0-9]|[0-5][0-9])$");
    QValidator *Validator = new QRegExpValidator(rx);
    this->setValidator(Validator);
    if (ifUseKey)
    {
        numKeyWidget->SetLimit("^([0-9]|[0-5][0-9])$");
    }
}
void Custom_LineEdit::set_limit_range_vitrual(float max, float min) // 虚拟键盘用
{
    if (ifUseKey == 1)
    {
        this->numKeyWidget->SetLimitRange(max, min);
    }
}

void Custom_LineEdit::set_limit(QString reNow)
{
    QRegExp rx(reNow);
    QValidator *Validator = new QRegExpValidator(rx);
    this->setValidator(Validator);
    if (ifUseKey)
    {
        numKeyWidget->SetLimit(reNow);
    }
}
void Custom_LineEdit::set_num_key()
{
    numKeyWidget = new NumKeyboard;
    void (NumKeyboard::*dealsignalNumKeyboardSure1)(QString) = &NumKeyboard::signalNumKeyboardSure;
    QObject::connect(numKeyWidget, dealsignalNumKeyboardSure1, this, &Custom_LineEdit::slot_key_confirm);
    ifUseKey = 1;
}
void Custom_LineEdit::slot_open_key()
{
    //qDebug() << "clicked";
    // globalKeyWakeStruct.currentWidget = enum_lineEdit;
    // globalKeyWakeStruct.currentItemPointer = (POINTER)(ui->lineEdit);
    // this->keyWidget->show();
    //this->numKeyWidget->show();
    //this->numKeyWidget->activateWindow();
    // this->numKeyWidget->SetLineText(this->text());
    //this->numKeyWidget->SetLineText("");
    this->numKeyWidget->exec();
    // ui->spinBox->setValue(100);
}
void Custom_LineEdit::slot_key_confirm(QString strNow)
{
    qDebug() << strNow;
    this->setText(strNow);
    emit virtualKeyConfirm(strNow);
    // ui->spinBox.current
}
