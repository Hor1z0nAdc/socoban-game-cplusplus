#ifndef WALL_H
#define WALL_H

#include <QGraphicsPixmapItem>
#include "gameitem.h"

class Wall : public QGraphicsPixmapItem, public gameItem
{
public:
    wall();
    void setCrate(bool newIsCrate);
    void setPlayer(bool newIsPlayer);
};

#endif // WALL_H
