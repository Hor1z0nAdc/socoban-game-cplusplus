#ifndef FLOOR_H
#define FLOOR_H

#include <QGraphicsPixmapItem>
#include "gameitem.h"

class Floor : public QGraphicsPixmapItem, public gameItem
{
public:
    floor(QGraphicsItem *parent);
    void setCrate(bool newIsCrate);
    void setPlayer(bool newIsPlayer);
};

#endif // FLOOR_H
