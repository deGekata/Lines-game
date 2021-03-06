#include "ellipseobject.h"
#include <QPainter>

EllipseObject::EllipseObject(QGraphicsItem *parent):QGraphicsObject(parent)
{

}

void EllipseObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
Q_UNUSED(option)
Q_UNUSED(widget)
painter->setRenderHint(QPainter::Antialiasing);
painter->setPen(QColor(0xf, 0xaf, 0xaa));
painter->setBrush(QBrush(QColor(0xaf, 0xa, 0xaa)));
QRectF rect = boundingRect();
painter->drawEllipse(rect);
}

QRectF EllipseObject::boundingRect() const
{
return QRectF(-250, -250, 500, 500);
}
