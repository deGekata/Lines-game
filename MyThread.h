ifndef MYTHREAD_H
#define MYHREAD_H

#include <QThread>
#include "widget.h"
#include "MyGraphicsView.h"
#include <QSequentialAnimationGroup>
#include <QAnimationGroup>
#include <QPropertyAnimation>
class MyThread : public QThread
{
public:
    explicit MyThread(QString threadName);

    // Переопределяем метод run(), в котором будет
    // располагаться выполняемый код
    void run(QSequentialAnimationGroup*& animation);
private:
    QString name;   // Имя потока
};

#endif // EXAMPLETHREADS_H
