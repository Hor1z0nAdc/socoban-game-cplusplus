#include "floor.h"

Floor::floor(QGraphicsItem *parent)
{
    isCrate = false;
    isPlayer = false;
    gameItemType = 1;
}

//sets the background depending on whether the square represents a floor or a box
void Floor::setCrate(bool newIsCrate) {
    isCrate = newIsCrate;

    if(isCrate) {
        setPixmap(QPixmap(":/images/crate.png").scaled(60,60));
    }
    else {
        setPixmap(QPixmap(":/images/floor.png").scaled(60,60));
    }
}

//if the character is in the floor field, it sets the floor to transparent, otherwise to normal
void Floor::setPlayer(bool newIsPlayer) {
    isPlayer = newIsPlayer;

    if(isPlayer) {
        QPixmap pixM(QSize(60,60));
        pixM.fill(Qt::transparent);
        setPixmap(pixM);
    }
    else {
        setPixmap(QPixmap(":/images/floor.png").scaled(60,60));
    }
}
