#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include "gameitem.h"
#include "player.h"
#include <string>

using std::vector;
using std::string;

class game : public QGraphicsView
{
    Q_OBJECT
public:
    bool isGoing;
    bool isWon;
    int squareSize; //specifies the size of the fields in the scene
    int offset; //specifies the y offset before starting to draw the field
    int buttonW, buttonH;
    int windowW, windowH;
    QGraphicsScene *scene;
    Player *playerObject;
    vector<vector<char>> testMap;
    vector<vector<char>> currentMap;
    vector<gameItem*> gameObjects; // for manipulating the fiels's isCrate and isPlayer variables
    vector<QGraphicsPixmapItem*> pixObjects; //for deleting the fields from the scene and from the view

    game();
    void createElements(vector<vector<char> > map);
    void keyPressEvent(QKeyEvent *event);
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void clearGame();
    void winGame();
    bool checkWin();
    bool validateMove(string move);

public slots:
    void handleButtonMove();
    void loadMap();
    void startTestMap();
};

#endif // GAME_H

