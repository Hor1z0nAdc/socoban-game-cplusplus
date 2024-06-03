#ifndef GAMEITEM_H
#define GAMEITEM_H


class gameItem
{
public:
    bool isCrate;
    bool isPlayer;
    //0-wall, 1-floor, 2-target
    int gameItemType;

    gameItem();
    virtual void setCrate(bool newIsCrate);
    virtual void setPlayer(bool newIsPlayer);
};

#endif // GAMEITEM_H
