#ifndef BALL_H
#define BALL_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include "widget.h"
class Ball : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    explicit Ball( QString, QImage*, QObject *parent = 0);
    ~Ball();
    QString color;
    QImage* image;
    static const int rad = 32;

signals:

public slots:

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:

};

#endif // BALL_H
