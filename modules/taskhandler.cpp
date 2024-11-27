#include "taskhandler.h"
#include <QTimer>
#include <QDebug>
#include <QTime>
TaskHandler::TaskHandler(QObject *parent)
    : QObject{parent}
{
    timerNow = new QTimer(this);
    timerCustomPause = new QTimer(this);
    connect(timerCustomPause, &QTimer::timeout, this, &TaskHandler::slot_timer_custompause_check);
    connect(timerNow, &QTimer::timeout, this, &TaskHandler::slot_timer_check_state);
}
void TaskHandler::addTask(TaskUnit task)
{
    taskList.append(task);
}

void TaskHandler::removeTask(int index)
{
    taskList.removeAt(index);
}

void TaskHandler::timer_check_state()
{
    timerNow->start(1000);
}

void TaskHandler::setInfinite(int num)
{
    m_num = num;
}

void TaskHandler::clearTask()
{
    taskList.clear();
}

void TaskHandler::startTask()
{
    
    ifRunning = true;
    if(taskList.size() > 0)
    {
        TaskUnit task = taskList.at(0);
        //taskList.removeAt(0);
        double volume = task.needleData.volume;
        double speed = task.needleData.speed;
        double stepVolumeNow = task.needleProperty.stepVolume;
        int volumeStep = volume / stepVolumeNow;
        int speedStep = speed / stepVolumeNow / 60;
        double pauseTime = task.needleData.pauseTime;
        QTime dieTime = QTime::currentTime().addMSecs(pauseTime * 1000);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        switch (task.taskType) {
        case TaskType_Inject:
            //inject
//            serialHandler->send_move_msg(volumeStep, speedStep, true);
            emit Signal_send_message(volumeStep, speedStep);
            break;
        case TaskType_Extract:
            //extract
//            serialHandler->send_move_msg(volumeStep, speedStep, false);
            emit Signal_send_message(-1 * volumeStep, speedStep);
            break;
        case TaskType_Wait:
            //wait
            ifCustomPause = true;
            currentCustomPauseIndexSet = task.needleData.pauseTime;
            currentCustomPauseIndex = task.needleData.pauseTime;
            timerCustomPause->start(100);
            //QTimer::singleShot(task.needleData.pauseTime * 1000, [this]() {
            //    this->nextTask();
            //});
            break;
        default:
            break;
        }
    }
    else
    {
        stopTask();
        return ;
    }
    timer_check_state();
}

void TaskHandler::slot_nextTask()
{
    nextTask();
}

void TaskHandler::nextTask()
{   
    qDebug() <<__FILE__<<__LINE__<< "nextTask";
    currentTaskIndex++;
    if(currentTaskIndex >= taskList.size())
    {
        stopTask();
    }
    else
    {

        TaskUnit task = taskList.at(currentTaskIndex);
        double volume = task.needleData.volume;
        double speed = task.needleData.speed;
        double stepVolumeNow = task.needleProperty.stepVolume;
        int volumeStep = volume/stepVolumeNow;
        double speedStep = speed/stepVolumeNow/60;
        double pauseTime = task.needleData.pauseTime;
        QTime dieTime = QTime::currentTime().addMSecs(pauseTime * 1000);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        switch (task.taskType)
        {
        case TaskType_Inject:
            //inject
//            serialHandler->send_move_msg(volumeStep,speedStep, true);

            emit Signal_send_message(volumeStep, speedStep);
            break;
        case TaskType_Extract:
            //extract
//            serialHandler->send_move_msg(volumeStep,speedStep, false);

            emit Signal_send_message( -1 * volumeStep, speedStep);
            break;
        case TaskType_Wait:
            //wait
            ifCustomPause = true;
            currentCustomPauseIndexSet = task.needleData.pauseTime;
            currentCustomPauseIndex = task.needleData.pauseTime;
            timerCustomPause->start(100);
            break;
        default:
            break;
        }
    }
}

void TaskHandler::pauseTask()
{
    ifPause = true;
//    serialHandler->send_pause_move_msg();
    if(ifCustomPause)
    {
        timerCustomPause->stop();
    }
}

void TaskHandler::continueTask()
{
    ifPause = false;
//    serialHandler->send_continue_move_msg();
    if(ifCustomPause)
    {
        timerCustomPause->start(100);
    }
}

void TaskHandler::stopTask()
{
    if(ifInfinite)
    {
        qDebug()<<__FILE__<<__LINE__<<"循环中";
        m_num--;
        if(m_num == 0)
        {
            qDebug()<<__FILE__<<__LINE__<< "stopTask";
            timerNow->stop();
            currentTaskIndex = 0;
            ifInfinite = false;
            ifRunning = false;
            ifPause = false;
            currentCustomPauseIndex = 0;
            currentCustomPauseIndexSet = 0;
            ifCustomPause = false;
            taskList.clear();
            serialHandler->send_stop_move_msg();
            emit Signal_stop_all_task();
            return ;
        }
        currentTaskIndex = -1;
        nextTask();
        emit Signal_change_infinited();
    }
    else
    {
        qDebug() <<__FILE__<<__LINE__<< "stopTask";
        timerNow->stop();
        currentTaskIndex = 0;
        ifInfinite = false;
        ifRunning = false;
        ifPause = false;
        currentCustomPauseIndex = 0;
        currentCustomPauseIndexSet = 0;
        ifCustomPause = false;
        taskList.clear();
//        serialHandler->send_stop_move_msg();
        emit Signal_stop_all_task();
    }
}


void TaskHandler::slot_timer_custompause_check()
{
    currentCustomPauseIndex -= 0.1;
    if(currentCustomPauseIndex <= 0.000001)
    {
        timerCustomPause->stop();
        currentCustomPauseIndex = 0;
        currentCustomPauseIndexSet = 0;
        ifCustomPause = false;
        nextTask();
    }
}


void TaskHandler::slot_timer_check_state()
{

    if(ifRunning)
    {
        if(ifPause)
        {
            //pause
        }
        else
        {
//            serialHandler->send_check_running_msg();
            emit Signal_send_cmd(1);
            //continue
        }
    }
    else
    {
        //stop
        stopTask();
    }
}
