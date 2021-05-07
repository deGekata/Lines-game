#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSequentialAnimationGroup>
#include <QAnimationGroup>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QShortcut>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <ball.h>
#include "CustomQGraphicsScene.h"
#include <random>

const int pole_size = 14;

namespace Ui { class Widget; }

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    int count = 0;
    int CNT_TO_GEN = 2;
    int CNT_TO_COLLAPSE = 4;
    bool done = true;
    bool no_way = false;
    std::mt19937 mersenne;
    QPair<int, int> gen_ball();
    inline qreal round(qreal, int);
    QVector<QPair<int, int>> get_way(QPair<int, int>, QPair<int, int>);
    QSequentialAnimationGroup* animationGroup;
    QPixmap* blue_ball, * red_ball, * yellow_ball, *green_ball, *cancelCross, *jokerP, *green_killP, *red_killP, *blue_killP, *yellow_killP;
    QImage* joker, *green_kill, *red_kill, *blue_kill, *yellow_kill;
    QPair<int,int> first_sel = {-1, -1}, second_sel = {-1, -1};
    int collapseBalls(int prev = 0);
    QVector<QVector<QGraphicsItem *>> balls;
    QVector<QVector<QString>> ball_colors;
    Ui::Widget *ui;
    CustomQGraphicsScene *scene;

public slots:
    bool slotDeleteBall(int, int, bool = false);
    void slotCreateBall(int, int ,QString, QImage* = NULL);
    void on_pushButton_clicked();
    void mousePressEvent(QMouseEvent *event);

private slots:

private:



};
#endif // WIDGET_H
