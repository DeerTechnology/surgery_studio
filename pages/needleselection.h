#ifndef NEEDLESELECTION_H
#define NEEDLESELECTION_H

#include <QDialog>
#include "../modules/needleproperty.h"
namespace Ui {
class NeedleSelection;
}

class NeedleSelection : public QDialog
{
    Q_OBJECT

public:
    explicit NeedleSelection(QWidget *parent = nullptr);
    ~NeedleSelection();

    void paintEvent(QPaintEvent *event);
    void init_ui();
    void init_logic();
    void init_current_style();
    void show_needle_property(NeedlePropertyStruct needleProperty);
    NeedlePropertyStruct globalNeedleProperty ;
    
    NeedleProperty * NeedlePropertyNow;
signals:
    //void Signal_needle_select_confirm(double maxVolume,double inner_diameter,double maxVspeed,double minVspeed,double stepVolume,QString name, QString minSpeed,QString maxSpeed,QString innerDiameter ,QString Volume);
    void Signal_needle_select_confirm(NeedlePropertyStruct needleProperty,QString name);
private slots:
    void btnClick();
    void hamiltonBtnClick();
    void sgeBtnClick();

    void on_pushButton_cal_clicked();

private:
    Ui::NeedleSelection *ui;
    QPushButton *pre_btn = nullptr;
};

#endif // NEEDLESELECTION_H
