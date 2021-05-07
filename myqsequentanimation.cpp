#include "widget.h"
#include "ui_widget.h"
#include "MyGraphicsView.h"
#include <ellipseobject.h>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QPoint>
#include <QQueue>
#include <iostream>
#include <QStack>
#include <QThread>
#include <myqsequentanimation.h>
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
#include <widget.h>
#include <QThread>

MyQsequentAnimation::MyQsequentAnimation(Widget* uii, int cur_task) {
   this->widg = uii;
   this->cur_task = cur_task;
}

MyQsequentAnimation::~MyQsequentAnimation() {
    qDebug() << "  ended  ";
    if(cur_task == 0){
        cur_task++;
        widg->animationGroup = new MyQsequentAnimation(widg, cur_task);
        qDebug() << "points  " <<  widg->first_sel.first << "   " << widg->first_sel.second << "   " << widg->second_sel.first << "    " <<  widg->second_sel.second << " curtsk  " << cur_task;
        auto way =  widg->get_way(widg->first_sel, widg->second_sel);
        if(way[0] == QPair<int, int>{-1, -1}){
            widg->balls[widg->first_sel.first][widg->first_sel.second]->setVisible(false);
            widg->ui->ballLabel->setPixmap(*(widg->cancelCross));
            widg->ui->ballLabel->setVisible(true);
            widg->animationGroup->clear();
            QPropertyAnimation* leftTopToRightTop = new QPropertyAnimation(widg->ui->ballLabel, "geometry");
            leftTopToRightTop->setDuration(300); // Длительность анимации
            leftTopToRightTop->setEndValue(widg->ui->ballLabel->geometry()); // Конечная позиция и геометрия анимации
            widg->animationGroup->addAnimation(leftTopToRightTop);
            widg->no_way = true;
        } else {
            for(auto u: way){
                qDebug() << u.first << "  " << u.second;
            }
            if ( widg->ball_colors[widg->first_sel.first][widg->first_sel.second] == "red"){
                widg->ui->ballLabel->setPixmap(*(widg->red_ball));
            } else if ( widg->ball_colors[widg->first_sel.first][widg->first_sel.second] == "blue"){
                widg->ui->ballLabel->setPixmap(*(widg->blue_ball));
            } else if ( widg->ball_colors[widg->first_sel.first][widg->first_sel.second] == "green"){
                widg->ui->ballLabel->setPixmap(*(widg->green_ball));
            } else if ( widg->ball_colors[widg->first_sel.first][widg->first_sel.second] == "yellow"){
                widg->ui->ballLabel->setPixmap(*(widg->yellow_ball));
            } else if ( widg->ball_colors[widg->first_sel.first][widg->first_sel.second] == "red_kill"){
                widg->ui->ballLabel->setPixmap(*(widg->red_killP));
            } else if ( widg->ball_colors[widg->first_sel.first][widg->first_sel.second] == "blue_kill"){
                widg->ui->ballLabel->setPixmap(*(widg->blue_killP));
            } else if ( widg->ball_colors[widg->first_sel.first][widg->first_sel.second] == "green_kill"){
                widg->ui->ballLabel->setPixmap(*(widg->green_killP));
            } else if ( widg->ball_colors[widg->first_sel.first][widg->first_sel.second] == "yellow_kill"){
                widg->ui->ballLabel->setPixmap(*(widg->yellow_killP));
            } else if ( widg->ball_colors[widg->first_sel.first][widg->first_sel.second] == "joker"){
                widg->ui->ballLabel->setPixmap(*(widg->jokerP));
            }
            widg->ui->ballLabel->setVisible(true);
            widg->balls[widg->first_sel.first][widg->first_sel.second]->setVisible(false);
        }
        widg->animationGroup->start(QAnimationGroup::DeleteWhenStopped);

    } else if(cur_task == 1){
        if(widg->no_way){
            widg->ui->ballLabel->setVisible(false);
            widg->balls[widg->first_sel.first][widg->first_sel.second]->setVisible(true);
            widg->first_sel = widg->second_sel = {-1, -1};
            cur_task = 0;
            widg->done = true;
            widg->no_way = false;
            return;
        }

        widg->slotCreateBall(widg->second_sel.first, widg->second_sel.second, widg->ball_colors[widg->first_sel.first][widg->first_sel.second]);
        widg->slotDeleteBall(widg->first_sel.first, widg->first_sel.second);
        widg->balls[widg->second_sel.first][widg->second_sel.second]->setVisible(true);
        widg->first_sel = widg->second_sel = {-1, -1};
        cur_task = 0;
        widg->ui->ballLabel->setVisible(false);
        int res = widg->collapseBalls();
        widg->count+=  res;
        qDebug() << "  COUNT F  " << res;
        widg->ui->lcdNumber->display(widg->count);
        qDebug() << "  seqPartkjlbkfjhljakfbhablkjfjkb  " << res;
        if(res && widg->scene->items().size()){
            widg->done = true;
            return;
        }
        for(int cnt = 0; cnt < widg->CNT_TO_GEN; ++cnt){
            widg->gen_ball();
            //qDebug() <<"generated   " <<  gen_res.first << "  " << gen_res.second << "\n";
        }
        qDebug() << "SIZEEEE " << widg->scene->items().size();
        widg->done = true;

    }


}
