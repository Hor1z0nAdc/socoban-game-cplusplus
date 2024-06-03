#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>


class Player : public QGraphicsPixmapItem
{
public:
    int row, col;
    player(QGraphicsItem *parent = nullptr);
};

#endif // PLAYER_H
