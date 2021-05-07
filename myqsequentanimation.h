#ifndef MYQSEQUENTANIMATION_H
#define MYQSEQUENTANIMATION_H
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QAnimationGroup>
#include <QDebug>
#include <ui_widget.h>
#include "widget.h"
class MyQsequentAnimation: public QSequentialAnimationGroup  {
public:
    MyQsequentAnimation(Widget* uii, int cur_task);
    ~MyQsequentAnimation();
    Widget* widg;
    int cur_task;

};

#endif // MYQSEQUENTANIMATION_H
