#include "needleselection.h"
#include "ui_needleselection.h"
#include <QFile>
#include <qdebug.h>
#include <QStyleOption>
#include <QPainter>
#include <QBitmap>
NeedleSelection::NeedleSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NeedleSelection)
{
    ui->setupUi(this);
    this->init_ui();
    this->init_logic();
}

void NeedleSelection::init_ui()
{
//    this->setProperty("form", true);
    ui->widgetLeft->setProperty("nav", "left");
    ui->page_SGE->setProperty("nav","SEG");
    ui->page_Hamilton->setProperty("nav","SEG");
    ui->page_custom->setProperty("nav","custom");
    QList<QPushButton *> btns = ui->widgetLeft->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(btnClick()));
    }

//    btns = ui->page_custom->findChildren<QPushButton *>();
//    foreach (QPushButton *btn, btns) {
//        btn->setCheckable(true);
//    }

    QList<QPushButton *> btnHam = ui->page_Hamilton->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btnHam) {
        connect(btn, SIGNAL(clicked()), this, SLOT(hamiltonBtnClick()));
        //btn->setMinimumSize(QSize(100,70));
        btn->setCheckable(true);
    }
    QList<QPushButton *> btnSGE = ui->page_SGE->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btnSGE) {
        connect(btn, SIGNAL(clicked()), this, SLOT(sgeBtnClick()));
        //btn->setMinimumSize(QSize(100,100));
        btn->setCheckable(true);
    }
    ui->pushButton->click();


    this->init_current_style();

}

void NeedleSelection::init_logic()
{
    NeedlePropertyNow = new NeedleProperty();
}

void NeedleSelection::init_current_style()
{
    QFile qFile(":/qss/blue.css");
    qFile.open(QFile::ReadOnly); // 读取qss文件，设置样式
    if (qFile.isOpen())
    {
        QString qss = qFile.readAll();
        this->setStyleSheet(qss);
    }
    qFile.close();


}


void NeedleSelection::show_needle_property(NeedlePropertyStruct needleProperty)
{
    NeedlePropertyStruct *currentNeedlePropertyStruct = &needleProperty;
    QString name = NeedlePropertyNow->name;
    QString volumeSuffix = "ul";
    double volumeData = currentNeedlePropertyStruct->maxVolume;
    if(currentNeedlePropertyStruct->maxVolume>1000)
    {
        volumeSuffix = "ml";
        volumeData = volumeData/1000;
    }
    else if(currentNeedlePropertyStruct->maxVolume<1)
    {
        volumeSuffix = "nl";
        volumeData = volumeData*1000;
    }
    QString volume = QString::asprintf("%.0f",volumeData) + volumeSuffix;
    
    QString innerDiameter = QString::asprintf("%.3fmm",currentNeedlePropertyStruct->inner_diameter);

    QString maxVolumeSuffix = "uL/min";
    double maxVolumeSpeed = currentNeedlePropertyStruct->maxVspeed;
    if(currentNeedlePropertyStruct->maxVspeed>=1000)
    {
        maxVolumeSuffix = "mL/min";
        maxVolumeSpeed = maxVolumeSpeed/1000;
    }
    else if(currentNeedlePropertyStruct->maxVspeed<1)
    {
        maxVolumeSuffix = "nL/min";
        maxVolumeSpeed = maxVolumeSpeed*1000;
    }
    QString maxVolumeSpeeds = QString::asprintf("%.3f",maxVolumeSpeed) + maxVolumeSuffix;

    double minVolumeSpeed = currentNeedlePropertyStruct->minVspeed;
    QString minVolumeSuffix = "uL/min";
    if(currentNeedlePropertyStruct->minVspeed>=1000)
    {
        minVolumeSuffix = "mL/min";
        minVolumeSpeed = minVolumeSpeed/1000;
    }
    else if(currentNeedlePropertyStruct->minVspeed<1)
    {
        minVolumeSuffix = "nL/min";
        minVolumeSpeed = minVolumeSpeed*1000;
    }
    QString minVolumeSpeeds = QString::asprintf("%.3f",minVolumeSpeed) + minVolumeSuffix;

//    ui->label_type->setText(name);
//    ui->label_volume->setText(volume);
//    ui->label_diameter->setText(innerDiameter);
//    ui->label_max->setText(maxVolumeSpeeds);
//    ui->label_min->setText(minVolumeSpeeds);
    qDebug()<<"*********";
    qDebug()<<"volume"<<currentNeedlePropertyStruct->maxVolume;
    qDebug()<<"maxspeed"<<currentNeedlePropertyStruct->maxVspeed;
    qDebug()<<"minspeed"<<currentNeedlePropertyStruct->minVspeed;
    qDebug()<<"stepVolume"<<currentNeedlePropertyStruct->stepVolume;
    qDebug()<<"*********";

    emit Signal_needle_select_confirm(globalNeedleProperty,NeedlePropertyNow->name);
    hide();
    //qDebug()<<"stepVolume"<<needleProperty->stepVolume;
}

void NeedleSelection::hamiltonBtnClick()
{
    QPushButton *b = static_cast<QPushButton *>(sender());
    if(pre_btn)
    {
        pre_btn->setChecked(false);
    }
    pre_btn = b;
    b->setChecked(true);
    QString name = b->text();
    QList<QPushButton *> btns = ui->page_Hamilton->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        
        if (btn == b) {
            int ind = 0;
            if(name == "0.5uL (7000.5")
            {
                ind = 0;
            }
            else if(name == "1uL (7001")
            {
                ind = 1;
            }
            else if(name == "2uL(7002")
            {
                ind = 2;
            }
            else if(name == "5uL(7105")
            {
                ind = 3;
            }
            else if(name == "5uL(75")
            {
                ind = 4;
            }
            else if(name == "10uL(701")
            {
                ind = 5;
            }
            else if(name == "25uL(702")
            {
                ind = 6;
            }
            else if(name == "50uL(705")
            {
                ind = 7;
            }
            else if(name == "100uL(710")
            {
                ind = 8;
            }
            else if(name == "250uL(725")
            {
                ind = 9;
            }
            else if(name == "500uL(750")
            {
                ind = 10;
            }
            else if(name == "1mL(1001")
            {
                ind = 11;
            }
            else if(name == "2.5mL(1002")
            {
                ind = 12;
            }
            else if(name == "5mL(1005")
            {
                ind = 13;
            }
            else if(name == "10mL(1010")
            {
                ind = 14;
            }
            else if(name == "25mL(1025")
            {
                ind = 15;
            }
            else if(name == "50mL(1050")
            {
                ind = 16;
            }
            
            
            NeedleType currentType = _Hamilton;
            NeedlePropertyStruct needlePRopertyStructNow = NeedlePropertyNow->get_needle_property(currentType,ind,name);
            globalNeedleProperty.maxVolume = needlePRopertyStructNow.maxVolume;
            globalNeedleProperty.inner_diameter = needlePRopertyStructNow.inner_diameter;
            globalNeedleProperty.maxVspeed = needlePRopertyStructNow.maxVspeed;
            globalNeedleProperty.minVspeed = needlePRopertyStructNow.minVspeed;
            globalNeedleProperty.stepVolume = needlePRopertyStructNow.stepVolume;

            update();
            this->show_needle_property(needlePRopertyStructNow);
            break;
        }
    }
}

void NeedleSelection::sgeBtnClick()
{
    QPushButton *b = static_cast<QPushButton *>(sender()) ;
    if(pre_btn)
    {
        qDebug()<<__FILE__<<__LINE__<<"完成取消";
        pre_btn->setChecked(false);
    }
    pre_btn = b;
    b->setChecked(true);
    QString name = b->text().mid(0,b->text().indexOf('-'));
    qDebug()<<__FILE__<<__LINE__<<"change page";
    QList<QPushButton *> btns = ui->page_SGE->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {

        if (btn == b) {
            int ind = 0;
            if(name == "0.5uL")
            {
                ind = 0;
            }
            else if(name == "1uL")
            {
                ind = 1;
            }
            else if(name == "2uL")
            {
                ind = 2;
            }
            else if(name == "5uL")
            {
                ind = 3;
            }
            else if(name == "10uL")
            {
                ind = 4;
            }
            else if(name == "25uL")
            {
                ind = 5;
            }
            else if(name == "50uL")
            {
                ind = 6;
            }
            else if(name == "100uL")
            {
                ind = 7;
            }
            
            NeedleType currentType = _SGE;
            NeedlePropertyStruct  needlePRopertyStructNow = NeedlePropertyNow->get_needle_property(currentType,ind,name);
            
            globalNeedleProperty.maxVolume = needlePRopertyStructNow.maxVolume;
            globalNeedleProperty.inner_diameter = needlePRopertyStructNow.inner_diameter;
            globalNeedleProperty.maxVspeed = needlePRopertyStructNow.maxVspeed;
            globalNeedleProperty.minVspeed = needlePRopertyStructNow.minVspeed;
            globalNeedleProperty.stepVolume = needlePRopertyStructNow.stepVolume;

            update();
            this->show_needle_property(needlePRopertyStructNow);
            break;
        }
    }
}

void NeedleSelection::btnClick()
{
    QPushButton *b = (QPushButton *)sender();
    QString name = b->text();
    qDebug()<<__FILE__<<__LINE__<<"change page";
    QList<QPushButton *> btns = ui->widgetLeft->findChildren<QPushButton *>();
    unsigned char currentBtnInd = 0;
    foreach (QPushButton *btn, btns) {

        if (btn == b) {
            ui->stackedWidget->setCurrentIndex(currentBtnInd);
            btn->setChecked(true);
            
            //ui->stackedWidget->setCurrentIndex(currentBtnInd);
        } else {
            btn->setChecked(false);
        }
        currentBtnInd+=1;
    }
}

NeedleSelection::~NeedleSelection()
{
    delete ui;
}

void NeedleSelection::paintEvent(QPaintEvent *event)
{
    //绘制样式
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//绘制样式

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter painter(&bmp);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    //设置边框为圆角15px
    painter.drawRoundedRect(bmp.rect(), 15, 15);
    setMask(bmp);
    QDialog::paintEvent(event);
}

void NeedleSelection::on_pushButton_cal_clicked()
{
    double diameter = ui->lineEdit_diameter->text().toDouble();
    double volume = ui->lineEdit_volume->text().toDouble();
    if(pre_btn)
    {
        pre_btn->setChecked(false);
        pre_btn = nullptr;
    }
    if(ui->radioButton_mL->isChecked())
    {
        volume = volume*1000;
    }
    
    NeedleType currentType = _Custom;
    NeedlePropertyStruct needlePRopertyStructNow = NeedlePropertyNow->get_needle_property(currentType,diameter,volume,"Custom");
    globalNeedleProperty.maxVolume = needlePRopertyStructNow.maxVolume;
    globalNeedleProperty.inner_diameter = needlePRopertyStructNow.inner_diameter;
    globalNeedleProperty.maxVspeed = needlePRopertyStructNow.maxVspeed;
    globalNeedleProperty.minVspeed = needlePRopertyStructNow.minVspeed;
    globalNeedleProperty.stepVolume = needlePRopertyStructNow.stepVolume;
    this->show_needle_property(needlePRopertyStructNow);
}

