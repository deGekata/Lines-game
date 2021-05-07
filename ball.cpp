#include "ball.h"
#include "widget.h"

struct color {
    QColor flat, focal, contour;
    color() = default;

    color(QColor flat, QColor focal, QColor contour) {
        this->flat = flat;
        this->focal = focal;
        this->contour = contour;
    }
};

QMap<QString, color> colors = {
    {"red", color(QColor(216,30,20), QColor(168,24,17), QColor(0,0,0))},
    {"blue", color(QColor(55, 38, 237), QColor(44, 35,145), QColor(0,0,0))},
    {"green", color(QColor(50, 240, 105), QColor(7, 132, 44), QColor(0,0,0))},
    {"yellow", color(QColor(253, 253, 98), QColor(238, 224, 29), QColor(0,0,0))},
};


Ball::Ball(QString color, QImage* image, QObject *parent){
    this->color = color;
    this->image = image;

}

Ball::~Ball() { }

QRectF Ball::boundingRect() const {
    return QRectF(-32, -32, 64, 64);
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);  Q_UNUSED(widget);

    int x = 0, y = 0, r = 32;
    if(color == "joker") {
        painter->drawImage(QRect(-32, -32, 64, 64), *this->image);
        return;
    } else if(color == "green_kill") {
        painter->drawImage(QRect(-32, -32, 64, 64), *this->image);
        return;
    } else if(color == "yellow_kill") {
        painter->drawImage(QRect(-32, -32, 64, 64), *this->image);
        return;
    } else if(color == "red_kill") {
        painter->drawImage(QRect(-32, -32, 64, 64), *this->image);
        return;
    } else if(color == "blue_kill") {
        painter->drawImage(QRect(-32, -32, 64, 64), *this->image);
        return;
    }




    QRadialGradient* gradient;
    gradient = new QRadialGradient(x, y, r);
    gradient->setFocalPoint(x - 20, y - 20);
    gradient->setColorAt(0.0, colors[color].flat);
    gradient->setColorAt(0.9, colors[color].focal);
    gradient->setColorAt(1, colors[color].contour);


    painter->setBrush(*gradient);
    QRectF rect = boundingRect();
    painter->drawEllipse(rect);


    /*painter->setBrush(Qt::SolidPattern);
    painter->setBrush(QColor(40,40,40));
    painter->drawRect(0, 0, 100, 100);
    QRadialGradient *gradient = new QRadialGradient;
    gradient->setCenter(30, 30);
    gradient->setRadius(40);
    gradient->setColorAt(0.0, QColor(216,30,20));
    gradient->setColorAt(0.9, QColor(168,24,17));
    gradient->setColorAt(1, QColor(0,0,0));
    //gradient->setColorAt(1, colors[color]);
    gradient->setFocalPoint(40, 40);
    painter->setBrush(*gradient);
*/
    delete  gradient;
}
