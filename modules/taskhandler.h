#ifndef TASKHANDLER_H
#define TASKHANDLER_H
#include <QObject>
#include <QJsonArray>
#include "needleproperty.h"
#include "serialhandler.h"
#include <QTimer>
typedef enum
{
    TaskType_Inject = 1,
    TaskType_Extract,
    TaskType_Wait,
}TaskType;
typedef enum 
{
    RunningType_Single = 1,
    RunningType_Loop,
    RunningType_Custom
}RunningType;
typedef struct TaskNeedleData
{
    double volume;//uL
    double speed;//uL/min
    double pauseTime;//s
}TaskNeedleData;

typedef struct TaskUnit
{
    TaskType taskType;
    NeedlePropertyStruct needleProperty;
    TaskNeedleData needleData;
}TaskUnit;


class TaskHandler : public QObject
{
    Q_OBJECT

public:

    explicit TaskHandler(QObject *parent = nullptr);
    bool ifRunning = false;
    bool ifPause = false;
    bool ifInfinite = false;
    int currentTaskIndex = 0;
    int m_num = 1;
    bool ifCustomPause = false;
    double currentCustomPauseIndexSet = 0;
    double currentCustomPauseIndex = 0;
    RunningType runningType;
    QTimer* timerNow;
    QTimer* timerCustomPause;
    QList<TaskUnit> taskList;
    SerialHandler *serialHandler;
    void addTask(TaskUnit task);
    void removeTask(int index);
    void clearTask();
    void startTask();
    void pauseTask();
    void continueTask();
    void nextTask();
    void stopTask();
    void slot_nextTask();
    void timer_check_state();
    void setInfinite(int num);
    //void stop_all(); // Callback function

signals:
    void Signal_stop_all_task();
    void Signal_change_infinited();
    void Signal_send_message(int volumeStep, int speedStep, bool statu = true);
    void Signal_send_cmd(int cmd);
private slots:
    void slot_timer_check_state();
    void slot_timer_custompause_check();

};

#endif // TASKHANDLER_H
