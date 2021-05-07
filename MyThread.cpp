#include "MyThread.h"
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "MyGraphicsView.h"

MyThread::MyThread(QString threadName) :
    name(threadName)
{

}

void MyThread::run(QSequentialAnimationGroup*& animation)
{
    animation->start();
}
