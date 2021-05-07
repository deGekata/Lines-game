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
#include <random>
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    //настройка сцены и юайки
    ui->setupUi(this);
    this->setFixedSize(68 * pole_size, pole_size * 68 + 64);
    scene = new CustomQGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->setSceneRect(0, 0 ,68 * pole_size, 68 * pole_size + 68); //Игровые координаты от (-250,-250) до (250,250)
    setLayout(ui->verticalLayout);


    ui->pushButton->setAutoFillBackground(true);
    QPalette pal = ui->pushButton->palette();
    pal.setColor(QPalette::Background, QColor(221, 20, 110));
    ui->pushButton->setPalette(pal);
    ui->lcdNumber->display(count);

    //инициализация и настройка лейблочки + пиксмапов
    blue_ball = new QPixmap(":blue_ball - Copy.png");
    blue_kill = new QImage(":blue_kill.png");
    blue_killP = new QPixmap(":blue_kill.png");

    yellow_ball = new QPixmap(":yellow - Copy.png");
    yellow_kill = new QImage(":yellow_kill.png");
    yellow_killP = new QPixmap(":yellow_kill.png");

    green_ball = new QPixmap(":green_ball - Copy.png");
    green_kill = new QImage(":green_kill.png");
    green_killP = new QPixmap(":green_kill.png");


    red_ball = new QPixmap(":red_ball - Copy.png");
    red_kill = new QImage(":red_kill.png");
    red_killP = new QPixmap(":red_kill.png");

    cancelCross = new QPixmap(":cancelationCross.png");
    joker = new QImage(":joker_ball.png");
    jokerP = new QPixmap(":joker_ball.png");


    ui->ballLabel->resize(64, 64);
    ui->ballLabel->setFixedSize(64, 64);
    ui->ballLabel->setEnabled(true);
    ui->ballLabel->setVisible(false);
    ui->ballLabel->setPixmap(*green_ball);
    ui->ballLabel->setGeometry(100, 100, Ball::rad * 2 + 10, Ball::rad * 2 + 10);
    ui->ballLabel->raise();
    animationGroup = new MyQsequentAnimation(this, 0);

    //зануление массива шаров, установка семечка для генерации
    balls = QVector<QVector<QGraphicsItem *>>(pole_size);
    ball_colors = QVector<QVector<QString>>(pole_size);
    for(int i = 0; i < pole_size; ++i){
        balls[i] = QVector<QGraphicsItem *>(pole_size);
        ball_colors[i] = QVector<QString>(pole_size);
    }
    /*for(int iter = 0; iter < CNT_TO_GEN; ++iter){
        gen_ball();
    }*/
    mersenne.seed(clock());
    scene->update();
}

Widget::~Widget() { delete ui; }
//--------------------------------------------------------------------------------------------
QVector<QPair<int, int>> Widget::get_way(QPair<int, int> first_p, QPair<int, int> second_p){
    //поиск пути по волновому алгоритму Ли


    int** mas = new int*[pole_size];
    for (int i =0; i < pole_size; ++i){
        mas[i] = new int[pole_size];
    }
    //максимизирование поля
    for(int i = 0; i < pole_size; ++i){
        for (int j = 0; j < pole_size; ++j){
            mas[i][j] = 1e3;
        }
    }
    qDebug() << first_p.first << "  fdsfa " << first_p.second;
    QQueue<QPair<int, int>> queue;
    mas[first_p.first][first_p.second] = 0;
    QPair<int, int> cur_piv;
    queue.push_back(first_p);

    //алгоритм ли
    while (!queue.isEmpty()) {
        cur_piv = queue.front();
        queue.pop_front();
        if(cur_piv == second_p) continue;
        if (cur_piv.first > 0){
            if (mas[cur_piv.first - 1][cur_piv.second] > (mas[cur_piv.first][cur_piv.second] + 1) && (balls[cur_piv.first - 1][cur_piv.second] == NULL)){
                mas[cur_piv.first - 1][cur_piv.second] = mas[cur_piv.first][cur_piv.second] + 1;
                queue.push_back(QPair<int, int>(cur_piv.first - 1, cur_piv.second));
            }
        }
        if (cur_piv.first < (pole_size - 1)){
            if(mas[cur_piv.first + 1][cur_piv.second] > (mas[cur_piv.first][cur_piv.second] + 1) && (balls[cur_piv.first + 1][cur_piv.second] == NULL)){
                mas[cur_piv.first + 1][cur_piv.second] = mas[cur_piv.first][cur_piv.second] + 1;
                queue.push_back(QPair<int, int>(cur_piv.first + 1, cur_piv.second));
            }
        }

        if (cur_piv.second > 0){
            if (mas[cur_piv.first ][cur_piv.second - 1] > (mas[cur_piv.first][cur_piv.second] + 1) && (balls[cur_piv.first ][cur_piv.second - 1] == NULL)){
                mas[cur_piv.first ][cur_piv.second - 1] = mas[cur_piv.first][cur_piv.second] + 1;
                queue.push_back(QPair<int, int>(cur_piv.first, cur_piv.second - 1));
            }
        }
        if (cur_piv.second < (pole_size - 1)){
            if(mas[cur_piv.first ][cur_piv.second + 1] > (mas[cur_piv.first][cur_piv.second] + 1) && (balls[cur_piv.first ][cur_piv.second + 1] == NULL)){
                mas[cur_piv.first ][cur_piv.second + 1] = mas[cur_piv.first][cur_piv.second] + 1;
                queue.push_back(QPair<int, int>(cur_piv.first , cur_piv.second + 1));
            }
        }


    }
    for (int i = 0; i < pole_size; ++i){
        for (int j = 0; j < pole_size; ++j){
            std::cout << "\t" << mas[j][i];
        }
        std::cout << std::endl;
    }
    QPair<int, int> piv_to_searh = second_p;
    int way_len = mas[second_p.first][second_p.second];
    QStack<QPair<int,int>> st;
    QVector<QPair<int, int>> ans;

    //нахождение списка пути
    while (way_len > 0){
        st.push_back(piv_to_searh);
        qDebug() << piv_to_searh.first * 68 + 32 << "   " <<  piv_to_searh.second * 68 + 32;
        std::cout << piv_to_searh.first << "   " << piv_to_searh.second;
        //        std::cout << "next dot";
        if (piv_to_searh.first > 0){
            if(mas[piv_to_searh.first - 1][piv_to_searh.second] == (mas[piv_to_searh.first][piv_to_searh.second] - 1)){
                piv_to_searh.first--;
                way_len--;
                continue;
            }
        }
        if (piv_to_searh.first < (pole_size - 1)){
            if(mas[piv_to_searh.first + 1][piv_to_searh.second] == (mas[piv_to_searh.first][piv_to_searh.second] - 1)){

                piv_to_searh.first++;
                way_len--;
                continue;
            }
        }
        if (piv_to_searh.second < (pole_size - 1)){
            if(mas[piv_to_searh.first][piv_to_searh.second + 1] == (mas[piv_to_searh.first][piv_to_searh.second] - 1)){
                piv_to_searh.second++;
                way_len--;
                continue;
            }
        }
        if (piv_to_searh.second > 0){
            if(mas[piv_to_searh.first ][piv_to_searh.second - 1] == (mas[piv_to_searh.first][piv_to_searh.second] - 1)){
                piv_to_searh.second--;
                way_len--;
                continue;

            }
        }
        ans.push_back(QPair<int, int>(-1, -1));
        return  ans;
    }
    if(st.top() == QPair<int, int>{-1, -1}){
        return    QVector<QPair<int, int>>{QPair<int, int>{-1, -1}};
    }

    //переворот пути
    while(!st.isEmpty()){
        QPropertyAnimation* leftTopToRightTop = new QPropertyAnimation(ui->ballLabel, "geometry");
        leftTopToRightTop->setDuration(45);
        leftTopToRightTop->setEndValue(ui->pushButton->geometry().translated((Ball::rad * 2 + 4) * st.top().first + 2,
                                                                             (Ball::rad * 2) + (Ball::rad * 2 + 4) * st.top().second)); // Конечная позиция и геометрия анимации
        animationGroup->addAnimation(leftTopToRightTop);
        ans.push_back(st.top());
        st.pop();
    }

    for(auto i = ans.begin(); i < ans.end(); ++i){
        std::cout << i->first << "   " << i->second << "\n";
    }
    return ans;
}

//--------------------------------------------------------

void Widget::mousePressEvent(QMouseEvent * e) {
    //проверка, работает ли в данный момент анимация
    if(!done){
        return;
    }

    //расчет точки клика
    QPointF point = ui->graphicsView->mapFrom (ui->graphicsView ,e->pos ());
    if((point.x() < 0) || (point.x() > 68 * pole_size) || (point.y() < 62) || (point.y() > pole_size * 68 + 64)){
        qDebug() << "ahahahahha";
        return;
    }
    int pos_x = (point.x() + 67) / 68 -1;
    int pos_y = (point.y() - 64) / 68 ;
    qDebug() << pos_x << "  " << pos_y;
    qDebug() << "x=" << point.x() << " y=" << point.y();

    //определение выбранных шаров
    if(first_sel.first == -1){
        if(balls[pos_x][pos_y] != NULL){
            first_sel.first = pos_x;
            first_sel.second = pos_y;
            qDebug() << "first was selected " << first_sel.first << "  " << first_sel.second;
        } else {
            qDebug() << "created ";
            slotCreateBall(pos_x, pos_y, "blue");
        }
        qDebug() << "it works";
        scene->update();
        return;
    } else if(first_sel.first == pos_x && first_sel.second == pos_y) {
        qDebug() << "first was unselectedfdsafadsfas";
        first_sel = {-1, -1};
        if(balls[pos_x][pos_y] != NULL){

        } else {
            qDebug() << "created ";
            slotCreateBall(pos_x, pos_y, "blue");
        }
        qDebug() << "it works";
        scene->update();
        return;
    } else if(second_sel.first == -1){
        if(balls[pos_x][pos_y] != NULL){
            second_sel = first_sel = {-1, -1};
            return;
        }
        qDebug() << "all selected   from" << first_sel.first << " " << first_sel.second << "   to  " << pos_x << " " << pos_y;
        second_sel = {pos_x, pos_y};
    }


    //Движение лейблочки к первому шару
    animationGroup = new MyQsequentAnimation(this, 0);
    QPropertyAnimation* leftTopToRightTop = new QPropertyAnimation(ui->ballLabel, "geometry");
    leftTopToRightTop->setDuration(10); // Длительность анимации
    leftTopToRightTop->setEndValue(ui->pushButton->geometry().translated((Ball::rad * 2 + 4) * first_sel.first + 2,
                                                                         (Ball::rad * 2) + (Ball::rad * 2 + 4) * first_sel.second)); // Конечная позиция и геометрия анимации
    animationGroup->addAnimation(leftTopToRightTop);
    ui->ballLabel->setPixmap(*red_ball);
    ui->ballLabel->setVisible(false);
    done = false;
    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    qDebug() << "  daraaaw\n";

    //вывод счета
    scene->update();
}



int Widget::collapseBalls(int prev) {
    qDebug() << " PREV  " << prev;
    int max_Tea;
    int max_hor;
    int max_ver;
    int max_dec;
    int max_inc;
    char orient;
    max_Tea = max_ver = max_hor = max_inc = max_dec = 0;
    //определение Т-образных структур
    int cordX, cordY;

    char ans_max = 'n';
    for(int i = 0; i < pole_size; ++i){
        qDebug() << "\t" << ball_colors[0][i] << "\t" << ball_colors[1][i] << "\t" << ball_colors[2][i] << "\t" << ball_colors[3][i] << "\t" << ball_colors[4][i] << "\t" << ball_colors[5][i]
                    << "\t" << ball_colors[6][i] << "\t" << ball_colors[7][i] << "\t" << ball_colors[8][i] << "\t" << ball_colors[9][i];
    }
    for(int i = 0; i < pole_size; ++i){
        for(int j = 0; j < pole_size; ++j){
            if(balls[i][j] == NULL) continue;
            QString cur_color = ball_colors[i][j];
            char ans = 'n';
            int tmp = 0;
            int up_balls, down_balls, left_balls, rht_balls;
            up_balls = down_balls = left_balls = rht_balls = 0;
            int up = j - 1, low = j + 1, rht = i + 1, lht = i - 1;


            //идет вверх
            while(up > -1){
                if(balls[i][up] != NULL){
                    if(ball_colors[i][up][0] == cur_color[0] || ball_colors[i][up][0] == 'j'){
                        --up;
                        ++up_balls;
                        //                        qDebug() << "up";
                        continue;

                    }
                }
                break;
            }

            //идет вниз
            while(low < pole_size){
                if(balls[i][low] != NULL){
                    if(ball_colors[i][low][0] == cur_color[0] || ball_colors[i][low][0] == 'j'){
                        ++low;
                        ++down_balls;
                        //                        qDebug() << "down";
                        continue;

                    }
                }
                break;
            }

            //идет влево
            while(lht > -1){
                if(balls[lht][j] != NULL){
                    if(ball_colors[lht][j][0] == cur_color[0] || ball_colors[lht][j][0] == 'j'){
                        --lht;
                        ++left_balls;
                        //                        qDebug() << "left";
                        continue;

                    }
                }
                break;
            }

            //идет вправо
            while(rht < pole_size){
                if(balls[rht][j] != NULL){
                    if(ball_colors[rht][j][0] == cur_color[0] || ball_colors[rht][j][0] == 'j'){
                        ++rht;
                        ++rht_balls;
                        //                        qDebug() << "rht";
                        continue;

                    }
                }
                break;
            }


            //            qDebug() << "ANS IS " << ans_max << "   coords  " << i << "   " << j << "\n up, down, left, rht  " << up_balls << " " << down_balls << "  " << left_balls << "  " << rht_balls << " ";

            //max по тешкам
            //вниз

            if(down_balls >= 3 && left_balls >= 3 && rht_balls >= 3){
                if((down_balls + left_balls + rht_balls) > tmp){
                    tmp = down_balls + left_balls + rht_balls;
                    ans = 'd';
                }
            }

            //вверх
            if(up_balls >= 3 && left_balls >= 3 && rht_balls >= 3){
                if((up_balls + left_balls + rht_balls) > tmp){
                    tmp = up_balls + left_balls + rht_balls;
                    ans = 'u';
                }
            }

            //вправо
            if(down_balls >= 3 && up_balls >= 3 && rht_balls >= 3){
                if((down_balls + up_balls + rht_balls) > tmp){
                    tmp = down_balls + up_balls + rht_balls;
                    ans = 'r';
                }
            }

            //влево
            if(down_balls >= 3 && left_balls >= 3 && up_balls >= 3){
                if((down_balls + left_balls + up_balls) > tmp){
                    tmp = down_balls + left_balls + up_balls;
                    ans = 'l';
                }
            }
            if(tmp > max_Tea){
                max_Tea = tmp;
                ans_max = ans;
                cordX = i;
                cordY = j;
            }



        }
    }

    //удаление t-образных
    if(ans_max != 'n'){
        QString cur_color = ball_colors[cordX][cordY];
        int j = cordY, i = cordX;
        char ans = 'n';
        int tmp = 0;
        int up_balls, down_balls, left_balls, rht_balls;
        up_balls = down_balls = left_balls = rht_balls = 0;
        int up = j - 1, low = j + 1, rht = i + 1, lht = i - 1;


        //идет вверх
        while(up > -1){
            if(balls[i][up] != NULL){
                if(ball_colors[i][up] == cur_color || ball_colors[i][up][0] == 'j'){
                    --up;
                    ++up_balls;
                    //                    qDebug() << "up";
                    continue;

                }
            }
            break;
        }

        //идет вниз
        while(low < pole_size){
            if(balls[i][low] != NULL){
                if(ball_colors[i][low] == cur_color || ball_colors[i][low][0] == 'j'){
                    ++low;
                    ++down_balls;
                    //                    qDebug() << "down";
                    continue;

                }
            }
            break;
        }

        //идет влево
        while(lht > -1){
            if(balls[lht][j] != NULL){
                if(ball_colors[lht][j] == cur_color || ball_colors[lht][j][0] == 'j'){
                    --lht;
                    ++left_balls;
                    //                    qDebug() << "left";
                    continue;

                }
            }
            break;
        }

        //идет вправо
        while(rht < pole_size){
            if(balls[rht][j] != NULL){
                if(ball_colors[rht][j] == cur_color || ball_colors[rht][j][0] == 'j'){
                    ++rht;
                    ++rht_balls;
                    //                    qDebug() << "rht";
                    continue;

                }
            }
            break;
        }

        if(ans_max == 'l'){
            for(int i = 0; i <= left_balls; ++i){
                slotDeleteBall(cordX - i, cordY, true);
            }
            for(int i = 0; i <= up_balls; ++i){
                slotDeleteBall(cordX, cordY - i, true);
            }
            for(int i = 0; i <= down_balls; ++i){
                slotDeleteBall(cordX, cordY + i, true);
            }
        } else if(ans_max == 'r'){
            for(int i = 0; i <= rht_balls; ++i){
                slotDeleteBall(cordX + i, cordY, true);
            }
            for(int i = 0; i <= up_balls; ++i){
                slotDeleteBall(cordX, cordY - i, true);
            }
            for(int i = 0; i <= down_balls; ++i){
                slotDeleteBall(cordX, cordY + i, true);
            }
        } else if(ans_max == 'u'){
            for(int i = 0; i <= left_balls; ++i){
                slotDeleteBall(cordX - i, cordY, true);
            }
            for(int i = 0; i <= up_balls; ++i){
                slotDeleteBall(cordX, cordY - i, true);
            }
            for(int i = 0; i <= rht_balls; ++i){
                slotDeleteBall(cordX + i, cordY, true);
            }
        } else if(ans_max == 'd'){
            for(int i = 0; i <= left_balls; ++i){
                slotDeleteBall(cordX - i, cordY, true);
            }
            for(int i = 0; i <= down_balls; ++i){
                slotDeleteBall(cordX, cordY + i, true);
            }
            for(int i = 0; i <= rht_balls; ++i){
                slotDeleteBall(cordX + i, cordY, true);
            }
        }
        slotDeleteBall(cordX, cordY);
        //qDebug() << "SCORE " << max_Tea << "   coords  " << cordX << "   " << cordY << "\n up, down, left, rht  " << up_balls << " " << down_balls << "  " << left_balls << "  " << rht_balls << " ";


        return collapseBalls(prev + (max_Tea * 2 + 2));
    }

    //---------------------------------------------------------------------------------------------------------------------------
    //      По горизонталям
    QString curMaxColor;
    int maxLen = 0, maxRow = 0, maxCol = 0;
    QString curColor = "";
    int curLen = 1;

    for(int row = 0; row < pole_size; ++row){
        int beg = -1;
        curLen = 1;
        curColor = "";
        for(int col = 0; col < pole_size; ++col){
            if(ball_colors[col][row] != ""){
                beg = col;
                break;
            }
        }
        if(beg == -1){
            continue;
        }

        curColor = ball_colors[beg][row];
        curLen = 1;
        for(int col = beg + 1; col < pole_size; ++col) {
            if(ball_colors[col][row] == ""){
                if(curLen > maxLen){
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = row;
                    curColor = "";
                    orient = 'h';
                }
                curColor = "";
            } else if(curColor == ""){
                curColor = ball_colors[col][row];
                curLen = 1;
            } else if(ball_colors[col][row][0] == curColor[0] || ball_colors[col][row][0] == 'j' || curColor[0] == 'j') {
                curLen++;
                curColor = ball_colors[col][row];
                if(curLen > maxLen){
                    orient = 'h';
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = row;
                }
            } else {
                if(curLen > maxLen){
                    orient = 'h';
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = row;
                }
                curColor = ball_colors[col][row];
                curLen = 1;
            }
        }

    }
//    по вертикалям
    for(int col = 0; col < pole_size; ++col){
        int beg = -1;
        curLen = 1;
        curColor = "";
        for(int row = 0; row < pole_size; ++row){
            if(ball_colors[col][row] != ""){
                beg = row;
                break;
            }
        }
        if(beg == -1){
            continue;
        }
//        qDebug() << "BEGIN = " << beg;
        curColor = ball_colors[col][beg];
        curLen = 1;
        for(int row = beg + 1; row < pole_size; ++row) {
//            qDebug() << "ITEEEER";
            if(ball_colors[col][row] == ""){
                if(curLen > maxLen){
                    orient = 'v';
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = row;
                }
                curColor = "";
            } else if(curColor == ""){
                curColor = ball_colors[col][row];
//                qDebug() << "form '' to color ";
                curLen = 1;
            } else if(ball_colors[col][row][0] == curColor[0] || ball_colors[col][row][0] == 'j' || curColor[0] == 'j') {
//                qDebug() << "PLUSED ";
                curLen++;
                curColor = ball_colors[col][row];
                if(curLen > maxLen){
//                    qDebug() << "chng max";
                    orient = 'v';
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = row;
                }
            } else {
                if(curLen > maxLen){
//                    qDebug() << "chng max";
                    orient = 'v';
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = row;
                }
//                qDebug() << "form color to color ";
                curColor = ball_colors[col][row];
                curLen = 1;
            }

        }
        if(curLen > maxLen){
            orient = 'v';
            maxLen = curLen;
            curMaxColor = curColor;
            maxCol = col;
            maxRow = pole_size;
        }
    }


    //------------------------------------------------------------------------------------------------------------------------------
    //   возразстающие диагонали

    //первая половина
    for(int row = CNT_TO_COLLAPSE - 1; row < pole_size; ++row){
//        qDebug() << row << "  "  << " cur ROW";
        int beg = -1;
        curLen = 1;
        curColor = "";
        int t_row = row;
        for(int col = 0; col < row + 1  ; ++col){
            if(ball_colors[col][t_row] != ""){
                beg = col;
                break;
            }
            t_row--;
        }
        if(beg == -1){
//            qDebug() << "   continue";
            continue;
        }
//        qDebug() << t_row << "  "  << " cur tROW???";
        int main_row = t_row;
        curColor = ball_colors[beg][main_row];
        curLen = 1;
        t_row = main_row - 1;
        for(int col = beg + 1; col < row + 1; ++col) {
            if(ball_colors[col][t_row] == ""){
                if(curLen > maxLen){
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = t_row;
                    curColor = "";
                    orient = 'i';
                }
                curColor = "";
            } else if(curColor == ""){
                curColor = ball_colors[col][t_row];
                curLen = 1;
            } else if(ball_colors[col][t_row][0] == curColor[0] || ball_colors[col][t_row][0] == 'j' || curColor[0] == 'j') {
                curLen++;
                curColor = ball_colors[col][t_row];
                if(curLen > maxLen){
                    orient = 'i';
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = t_row;
                }
            } else {
                if(curLen > maxLen){
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = t_row;
                }
                curColor = ball_colors[col][t_row];
                curLen = 1;
            }
            //qDebug() << col << "  " << t_row  << "   " << curLen<< " cur col and row and curlen";
            t_row--;
        }

    }

    //вторая половина
    for (int col = 1; col < pole_size - CNT_TO_COLLAPSE + 1; ++col){
//        qDebug() << col << "  "  << " cur col";
        int beg = -1;
        curLen = 1;
        curColor = "";
        int t_col = col;
        for (int row = pole_size - 1; row >=  col; --row){
            if(ball_colors[t_col][row] != ""){
                beg = row;
                break;
            }
            t_col++;
        }
        if(beg == -1){
//            qDebug() << "   continue";
            continue;
        }
//        qDebug() << t_col << "  "  << " cur tCOL???";
        int main_col = t_col;
        curColor = ball_colors[main_col][beg];
        curLen = 1;
        t_col = main_col + 1;
        for (int row = beg - 1; row >= col; --row) {
            if(ball_colors[t_col][row] == ""){
                if(curLen > maxLen){
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = t_col;
                    maxRow = row;
                    curColor = "";
                    orient = 'i';
                }
                curColor = "";
            } else if(curColor == ""){
                curColor = ball_colors[t_col][row];
                curLen = 1;
            } else if(ball_colors[t_col][row][0] == curColor[0] || ball_colors[t_col][row][0] == 'j' || curColor[0] == 'j') {
                curLen++;
                ball_colors[t_col][row] = curColor;
                if(curLen > maxLen){
                    orient = 'i';
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = t_col;
                    maxRow = row;
                }
            } else {
                if(curLen > maxLen){
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = t_col;
                    maxRow = row;
                }
                curColor = ball_colors[t_col][row];
                curLen = 1;
            }
//            qDebug() << t_col << "  " << row  << "   " << curLen << "  " << curColor << "cur col and row and curlen";
            t_col++;
        }
    }


    //------------------------------------------------------------------------------------------------------------------------------
    //убывающие диагонали
    //    первая половина
    qDebug() << "DECREASING";
    for (int col = 0; col <= pole_size - CNT_TO_COLLAPSE; ++col) {
//        qDebug() << col << "  "  << " cur COL";
        int beg = -1;
        curLen = 1;
        curColor = "";
        int t_col = col;
        for (int row = 0; row < pole_size - col; ++row) {
            if(ball_colors[t_col][row] != "") {
                beg = row;
                break;
            }
            t_col++;
        }
        if (beg == -1) {
//            qDebug() << "    continue";
            continue;
        }
//        qDebug() << t_col << "  "  << " cur tCOL???";
        int main_col = t_col;
        curColor = ball_colors[main_col][beg];
        curLen = 1;
        t_col = main_col + 1;
        for (int row = beg + 1; row < pole_size - col; ++row) {
            if(ball_colors[t_col][row] == ""){
                if(curLen > maxLen){
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = t_col;
                    maxRow = row;
                    curColor = "";
                    orient = 'd';
                }
                curColor = "";
            } else if(curColor == ""){
                curColor = ball_colors[t_col][row];
                curLen = 1;
            } else if(ball_colors[t_col][row][0] == curColor[0] || ball_colors[t_col][row][0] == 'j' || curColor[0] == 'j') {
                curLen++;
                curColor = ball_colors[t_col][row];
                if(curLen > maxLen){
                    orient = 'd';
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = t_col;
                    maxRow = row;
                }
            } else {
                if(curLen > maxLen){
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = t_col;
                    maxRow = row;
                }
                curColor = ball_colors[t_col][row];
                curLen = 1;
            }
//            qDebug() << t_col << "  " << row  << "   " << curLen<< " cur col and row and curlen";
            t_col++;
        }
    }
    //вторая половина

    for (int row = 1; row <= pole_size - CNT_TO_COLLAPSE; ++row) {
//        qDebug() << row << "  "  << " cur ROW";
        int beg = -1;
        curLen = 1;
        curColor = "";
        int t_row = row;
        for (int col = 0; col < pole_size - row; ++col) {
            if(ball_colors[col][t_row] != "") {
                beg = col;
                break;
            }
            t_row++;
        }
        if (beg == -1) {
//            qDebug() << "continue";
            continue;
        }
//        qDebug() << t_row << "  "  << " cur tROW???";
        int main_row = t_row;
        curColor = ball_colors[beg][main_row];
        curLen = 1;
        t_row = main_row + 1;
        for (int col = beg + 1; col < pole_size - row; ++col) {
            if(ball_colors[col][t_row] == ""){
                if(curLen > maxLen){
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = t_row;
                    curColor = "";
                    orient = 'd';
                }
                curColor = "";
            } else if(curColor == ""){
                curColor = ball_colors[col][t_row];
                curLen = 1;
            } else if(ball_colors[col][t_row][0] == curColor[0] || ball_colors[col][t_row][0] == 'j' || curColor[0] == 'j') {
                curLen++;
                curColor = ball_colors[col][t_row];
                if(curLen > maxLen){
                    orient = 'd';
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = t_row;
                }
            } else {
                if(curLen > maxLen){
                    maxLen = curLen;
                    curMaxColor = curColor;
                    maxCol = col;
                    maxRow = t_row;
                }
                curColor = ball_colors[col][t_row];
                curLen = 1;
            }
//            qDebug() << col << "  " << t_row  << "   " << curLen<< " cur col and row and curlen";
            t_row++;
        }
    }







    //------------------------------------------------------------------------------------------------------------------------------




    qDebug() << " coords " << maxCol << "  " << maxRow << "  " << maxLen << "  " << curMaxColor << "  orient  " << orient;

    if(maxLen >= CNT_TO_COLLAPSE){
        if(orient == 'h'){
            for(int len = 0; len < maxLen; ++len){
                emit slotDeleteBall(maxCol - len, maxRow, true);
            }
        } else if (orient == 'v') {
            for(int len = 0; len < maxLen; ++len){
                emit slotDeleteBall(maxCol, maxRow - len, true);
            }
        } else if(orient == 'i') {
            for(int len = 0; len < maxLen; ++len){
                emit slotDeleteBall(maxCol - len, maxRow + len, true);
            }
        } else if(orient == 'd') {
            for(int len = 0; len < maxLen; ++len){
                emit slotDeleteBall(maxCol - len, maxRow - len, true);
            }
        }
        return maxLen + (maxLen - CNT_TO_COLLAPSE) * 2 + collapseBalls();
    }


    return 0;

}


QPair<int, int> Widget::gen_ball(){
    int rndRow = mersenne() % pole_size, rndCol = mersenne() % pole_size;
    for(int col = rndCol; col < pole_size; ++col){
        //qDebug() << rndRow << "geen  " << col;
        if(ball_colors[col][rndRow] == ""){
            QString color;
            QImage* image = NULL;
            int rnd_num = mersenne() % 100;
            bool special = false;
            if(rnd_num % 12 == 0) {
                rnd_num = 4;
            } else if(rnd_num % 5 == 0) {
                special = true;
                rnd_num = mersenne() % 4;
            } else {
                rnd_num = mersenne() % 4;
            }
            switch (rnd_num) {
            case 0:
                if(!special) {
                    color = "blue";
                } else {
                    image = blue_kill;
                    color = "blue_kill";
                }
                break;
            case 1:
                if(!special) {
                    color = "red";
                } else {
                    image = red_kill;
                    color = "red_kill";
                }
                break;
            case 2:
                if(!special) {
                    color = "green";
                } else {
                    image = green_kill;
                    color = "green_kill";
                }
                break;
            case 3:
                if(!special) {
                    color = "yellow";
                } else {
                    image = yellow_kill;
                    color = "yellow_kill";
                }
                break;
            case 4:
                color = "joker";
                image = joker;
                break;
            }
            slotCreateBall(col, rndRow, color, image);
            return {col, rndRow};
        }
    }
    for(int row = rndRow + 1; row < pole_size; ++row){
        for(int col = 0; col < pole_size; ++col){
            //qDebug() << row << "geen  " << col ;
            if(ball_colors[col][row] == ""){
                QString color;
                QImage* image = NULL;
                int rnd_num = mersenne() % 100;
                bool special = false;
                if(rnd_num % 12 == 0) {
                    rnd_num = 4;
                } else if(rnd_num % 5 == 0) {
                    special = true;
                    rnd_num = mersenne() % 4;
                } else {
                    rnd_num = mersenne() % 4;
                }
                switch (rnd_num) {
                case 0:
                    if(!special) {
                        color = "blue";
                    } else {
                        image = blue_kill;
                        color = "blue_kill";
                    }
                    break;
                case 1:
                    if(!special) {
                        color = "red";
                    } else {
                        image = red_kill;
                        color = "red_kill";
                    }
                    break;
                case 2:
                    if(!special) {
                        color = "green";
                    } else {
                        image = green_kill;
                        color = "green_kill";
                    }
                    break;
                case 3:
                    if(!special) {
                        color = "yellow";
                    } else {
                        image = yellow_kill;
                        color = "yellow_kill";
                    }
                    color = "yellow";
                    break;
                case 4:
                    color = "joker";
                    image = joker;
                    break;
                }
                slotCreateBall(col, row, color, image);
                return {col, row};
            }
        }
    }
    for(int row = 0; row < rndRow; ++row){
        for(int col = 0; col < pole_size; ++col){
            //qDebug() << row << "geen  " << col ;
            if(ball_colors[col][row] == ""){
                QString color;
                QImage* image = NULL;
                int rnd_num = mersenne() % 100;
                bool special = false;
                if(rnd_num % 12 == 0) {
                    rnd_num = 4;
                } else if(rnd_num % 5 == 0) {
                    special = true;
                    rnd_num = mersenne() % 4;
                } else {
                    rnd_num = mersenne() % 4;
                }
                switch (rnd_num) {
                case 0:
                    if(!special) {
                        color = "blue";
                    } else {
                        image = blue_kill;
                        color = "blue_kill";
                    }
                    break;
                case 1:
                    if(!special) {
                        color = "red";
                    } else {
                        image = red_kill;
                        color = "red_kill";
                    }
                    break;
                case 2:
                    if(!special) {
                        color = "green";
                    } else {
                        image = green_kill;
                        color = "green_kill";
                    }
                    break;
                case 3:
                    if(!special) {
                        color = "yellow";
                    } else {
                        image = yellow_kill;
                        color = "yellow_kill";
                    }
                    color = "yellow";
                    break;
                case 4:
                    color = "joker";
                    image = joker;
                    break;
                }
                slotCreateBall(col, row, color, image);
                return {col, row};
            }
        }
    }
    for(int col = 0; col < rndCol; ++col){
        //qDebug() << rndRow << "geen  " << col;
        if(ball_colors[col][rndRow] == ""){
            QString color;
            QImage* image = NULL;
            int rnd_num = mersenne() % 100;
            bool special = false;
            if(rnd_num % 12 == 0) {
                rnd_num = 4;
            } else if(rnd_num % 5 == 0) {
                special = true;
                rnd_num = mersenne() % 4;
            } else {
                rnd_num = mersenne() % 4;
            }
            switch (rnd_num) {
            case 0:
                if(!special) {
                    color = "blue";
                } else {
                    image = blue_kill;
                    color = "blue_kill";
                }
                break;
            case 1:
                if(!special) {
                    color = "red";
                } else {
                    image = red_kill;
                    color = "red_kill";
                }
                break;
            case 2:
                if(!special) {
                    color = "green";
                } else {
                    image = green_kill;
                    color = "green_kill";
                }
                break;
            case 3:
                if(!special) {
                    color = "yellow";
                } else {
                    image = yellow_kill;
                    color = "yellow_kill";
                }
                break;
            case 4:
                color = "joker";
                image = joker;
                break;
            }
            slotCreateBall(col, rndRow, color, image);
            return {col, rndRow};
        }
    }
    return {-1, -1};
}



//кнопка рестарт
void Widget::on_pushButton_clicked() {
    if(!done){
        return;
    }
    qDebug() << "x=pushed\n" ;
    for(int cur = 0; cur < balls.size(); ++cur){
        for(int elem = 0; elem < balls[cur].size(); ++elem){
            if(balls[cur][elem] == NULL) continue;
            ball_colors[cur][elem] = "";
            scene->removeItem(balls[cur][elem]);
            delete balls[cur][elem];
            balls[cur][elem] = NULL;
        }
    }
    first_sel = second_sel = {-1, -1};
    count = 0;
    ui->lcdNumber->display(count);
    for(int iter = 0; iter < CNT_TO_GEN; ++iter){
        gen_ball();
    }
    scene->update();

}


bool Widget::slotDeleteBall(int x, int y, bool boom){
    if(balls[x][y] == NULL) return false;
    scene->removeItem(balls[x][y]);
    ui->lcdNumber->display(count);
    if(ball_colors[x][y] == "joker" && boom){
        ball_colors[x][y] = nullptr;
        int temp = 0;
        for(int i = 0; i < pole_size; ++i){
            count += emit slotDeleteBall(i, y, boom);
        }
        for(int i = 0; i < pole_size; ++i){
            count += emit slotDeleteBall(x, i, boom);
        }
    } else if((ball_colors[x][y] == "green_kill" || ball_colors[x][y] == "blue_kill" || ball_colors[x][y] == "yellow_kill" || ball_colors[x][y] == "red_kill") && boom){
        ball_colors[x][y] = nullptr;
        int lg, rg, tg, bg;
        lg = std::max(0, x - 1);
        rg = std::min(pole_size - 1, x + 1);
        tg = std::max(0, y - 1);
        bg = std::min(pole_size - 1, y + 1);
        for(int i = lg; i <=rg; ++i){
            for (int j = tg; j <= bg; ++j){
                count+= emit slotDeleteBall(i, j, true);
            }
        }
    }
    ui->lcdNumber->display(count);
    ball_colors[x][y] = nullptr;
    delete balls[x][y];
    balls[x][y] = NULL;
    return true;

}

void Widget::slotCreateBall(int pos_x, int pos_y, QString color, QImage* image){
    if(color == "blue_kill"){
        image = blue_kill;
    } else if(color == "green_kill"){
        image = green_kill;
    } else if(color == "red_kill"){
        image = red_kill;
    } else if(color == "yellow_kill"){
        image = yellow_kill;
    } else if (color == "joker"){
        image = joker;
    }
    balls[pos_x][pos_y] = new Ball(color, image);
    scene->addItem(balls[pos_x][pos_y]);
    balls[pos_x][pos_y]->setPos(pos_x * 68 + 34, pos_y * 68 + 34);
    ball_colors[pos_x][pos_y] = color;

}
