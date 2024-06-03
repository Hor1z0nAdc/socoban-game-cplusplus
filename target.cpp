#include "target.h"

Target::target(QGraphicsItem *parent)
{
    isCrate = false;
    isPlayer = false;
    gameItemType = 2;
}

//sets the background depending on whether the square represents a target field or a box
void Target::setCrate(bool newIsCrate) {
    isCrate = newIsCrate;

    if(isCrate) {
        setPixmap(QPixmap(":/images/dark_crate.png").scaled(60,60));
    }
    else {
        setPixmap(QPixmap(":/images/target.png").scaled(60,60));
    }
}

//if the character is in the target field, it sets the target to transparent, otherwise normals
void Target::setPlayer(bool newIsPlayer) {
    isPlayer = newIsPlayer;

    if(isPlayer) {
        QPixmap pixM(QSize(60,60));
        pixM.fill(Qt::transparent);
        setPixmap(pixM);
    }
    else {
        setPixmap(QPixmap(":/images/target.png").scaled(60,60));
    }
}
