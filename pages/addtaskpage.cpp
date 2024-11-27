#include "addtaskpage.h"
#include "ui_addtaskpage.h"
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#include <QBitmap>
AddTaskPage::AddTaskPage(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::AddTaskPage)
{
    
    ui->setupUi(this);
    this->init_ui();
    this->init_logic();
}

AddTaskPage::~AddTaskPage()
{
    delete ui;
}

void AddTaskPage::init_ui()
{

    this->setProperty("color", "white");
    this->setProperty("nav", "SEG");
    ui->widgetTitle->setProperty("form", "title");
    QList<QPushButton *> btns = this->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns)
    {
        btn->installEventFilter(this);
    }
    this->setWindowTitle(tr("添加任务"));
}

void AddTaskPage::init_logic()
{
}


bool AddTaskPage::check_data(double volume, double speed)
{
    qDebug() << " volume : "<<volume<<"  speed:"<<speed;
    if(speed > needleProperty.maxVspeed || speed < needleProperty.minVspeed)
    {
        return false;
    }
    if(volume > needleProperty.maxVolume || volume < 0.000000001)
    {
        return false;
    }
    return true;
}


void AddTaskPage::paintEvent(QPaintEvent *event)
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

bool AddTaskPage::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QPushButton *btn = qobject_cast<QPushButton *>(watched);
        if (btn)
        {
            // 在这里处理按钮点击事件
        
            if (btn == ui->pushButton_confirm)
            {
                // 处理确认按钮点击事件
                
                double volume = ui->lineEdit_volume->text().toDouble();
                double speed = ui->lineEdit_speed->text().toDouble();
                if(ui->radioButton_volumemL->isChecked())
                {
                    volume = volume*1000;
                }
                else if(ui->radioButton_volumenL->isChecked())
                {
                    volume = volume/1000;
                }
                
                if(ui->radioButton_speedmL->isChecked())
                {
                    speed = speed*1000;
                }
                else if(ui->radioButton_speednL->isChecked())
                {
                    speed = speed/1000;
                }
                if(check_data(volume,speed))
                {
                    double pauseTime = ui->lineEdit_pauseTime->text().toDouble();
                    if(ui->toolButton->text() == "注射")
                        emit Signal_add_task(TaskType_Inject,volume,speed, pauseTime);
                    else
                        emit Signal_add_task(TaskType_Extract,volume,speed, pauseTime);
                }
                else
                {
//                    QMessageBox::warning(this, tr("警告"), tr("数据不合法"));
                    warner.setText("数据不合法");
                    warner.exec();
                    return true ;
                }
            }
            else if (btn == ui->pushButton_cancel)
            {
                // 处理取消按钮点击事件
            }

            // 处理完按钮点击事件后关闭窗口
            this->hide();

            return true; // 如果你处理了事件，返回true
        }
    }

    // 如果你没有处理事件，调用基类的eventFilter方法
    return QDialog::eventFilter(watched, event);
}


//切换任务模式
void AddTaskPage::on_toolButton_clicked()
{

    if(ui->toolButton->text() == "注射")
    {
        ui->toolButton->setText("抽取");
    }
    else {

        ui->toolButton->setText("注射");
    }
}
