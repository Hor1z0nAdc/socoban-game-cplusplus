#ifndef TARGET_H
#define TARGET_H

#include <QGraphicsPixmapItem>
#include "gameitem.h"

class Target : public QGraphicsPixmapItem, public gameItem
{
public:
    target(QGraphicsItem *parent);
    void setCrate(bool newIsCrate);
    void setPlayer(bool newIsPlayer);
};

#endif // TARGET_H
