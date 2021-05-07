#ifndef CUSTOMQGRAPHICSSCENE_H
#define CUSTOMQGRAPHICSSCENE_H
#include <QGraphicsScene>

class CustomQGraphicsScene : public QGraphicsScene
{


protected:
   void drawBackground(QPainter * painter, const QRectF & rect );
};

#endif // CUSTOMQGRAPHICSSCENE_H
