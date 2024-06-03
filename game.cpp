#include "game.h"
#include "player.h"
#include "floor.h"
#include "wall.h"
#include "target.h"
#include <QDebug>
#include <QPushButton>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QKeyEvent>

game::game()
{
    //initialisation of variables
    offset = 140;
    squareSize = 60;
    buttonW = 80;
    buttonH = 40;
    windowW = 1300;
    windowH = 1000;
    isGoing = false;
    isWon = false;
    playerObject = nullptr;
    //w-wall, e-floor, t-target, b-box, p-player
    testMap = {
        {'e','e','w','w','w','w','w','e'},
        {'w','w','w','e','e','e','w','e'},
        {'w','t','p','b','e','e','w','e'},
        {'w','w','w','e','e','t','w','e'},
        {'w','t','w','w','b','e','w','e'},
        {'w','e','w','e','t','e','w','w'},
        {'w','b','e','b','b','b','t','w'},
        {'w','e','e','e','t','e','e','w'},
        {'w','w','w','w','w','w','w','w'}
    };

    //creates and configures the scene
    scene = new QGraphicsScene();
    setScene(scene);
    setFixedSize(windowW,windowH);
    scene->setSceneRect(0,0,windowW,windowH);
    setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //creation of buttons
    QPushButton *up = new QPushButton;
    QPushButton *down = new QPushButton;
    QPushButton *left = new QPushButton;
    QPushButton *right = new QPushButton;
    QPushButton *readFile = new QPushButton;
    QPushButton *startTest = new QPushButton;
    up->setText("up");
    down->setText("down");
    left->setText("left");
    right->setText("right");
    readFile->setText("read map file");
    startTest->setText("start text map");

    //set the font of buttons
    QFont font = up->font();
    font.setPointSize(13);
    up->setFont(font);
    down->setFont(font);
    left->setFont(font);
    right->setFont(font);
    readFile->setFont(font);
    startTest->setFont(font);

    //add buttons to the scene
    up->setGeometry(QRect(100, 10, buttonW, buttonH));
    down->setGeometry(QRect(100, 70, buttonW, buttonH));
    left->setGeometry(QRect(10, 30, buttonW, buttonH));
    right->setGeometry(QRect(190, 30, buttonW, buttonH));
    readFile->setGeometry(QRect(300, 10, 150, buttonH));
    startTest->setGeometry(QRect(300, 65, 150, buttonH));
    QGraphicsProxyWidget *proxy1 = scene->addWidget(up);
    QGraphicsProxyWidget *proxy2 = scene->addWidget(down);
    QGraphicsProxyWidget *proxy3 = scene->addWidget(left);
    QGraphicsProxyWidget *proxy4 = scene->addWidget(right);
    QGraphicsProxyWidget *proxy5 = scene->addWidget(readFile);
    QGraphicsProxyWidget *proxy6 = scene->addWidget(startTest);

    //connect signal-slot to buttons
    connect(up, SIGNAL(clicked()), this, SLOT(handleButtonMove()));
    connect(down, SIGNAL(clicked()), this, SLOT(handleButtonMove()));
    connect(left, SIGNAL(clicked()), this, SLOT(handleButtonMove()));
    connect(right, SIGNAL(clicked()), this, SLOT(handleButtonMove()));
    connect(readFile, SIGNAL(clicked()), this, SLOT(loadMap()));
    connect(startTest, SIGNAL(clicked()), this, SLOT(startTestMap()));
}

void game::createElements(vector<vector<char>> map) {
    Player *player;

    for (int i = 0; i < map.size(); i++) {
         for (int j = 0; j < map[0].size(); j++) {
            switch(map[i][j]) {
                case 'e':
                {
                    Floor *floor = new Floor();
                    floor->gameItemType = 1;
                    floor->setPos(j*squareSize, offset + i*squareSize);
                    floor->setPixmap(QPixmap(":/images/floor.png").scaled(squareSize,squareSize));

                    gameObjects.push_back(floor);
                    pixObjects.push_back(floor);
                    scene->addItem(floor);
                    break;
                }
                case 'w':
                {
                    Wall *wall = new Wall();
                    wall->setPixmap(QPixmap(":/images/wall.png").scaled(squareSize,squareSize));
                    wall->setPos(j*squareSize, offset + i*squareSize);

                    gameObjects.push_back(wall);
                    pixObjects.push_back(wall);
                    scene->addItem(wall);
                    break;
                }
                case 't':
                {
                    Target *target = new Target();
                    target->setPixmap(QPixmap(":/images/target.png").scaled(squareSize,squareSize));
                    target->setPos(j*squareSize, offset + i*squareSize);

                    gameObjects.push_back(target);
                    pixObjects.push_back(target);
                    scene->addItem(target);
                    break;
                }
                case 'b':
                {
                    Floor *crate = new Floor();
                    crate->gameItemType = 1;
                    crate->setCrate(true);
                    crate->setPos(j*squareSize, offset + i*squareSize);

                    gameObjects.push_back(crate);
                    pixObjects.push_back(crate);
                    scene->addItem(crate);
                    break;
                }
                case 'p':
                {
                    Floor *floor = new Floor();
                    floor->setPos(j*squareSize, offset + i*squareSize);
                    floor->setPixmap(QPixmap(":/images/floor.png").scaled(squareSize,squareSize));

                    player = new Player();
                    player->row = i;
                    player->col = j;
                    player->setPos(j*squareSize, offset + i*squareSize);
                    player->setPixmap(QPixmap(":/images/player.png").scaled(squareSize,squareSize));

                    playerObject = player;
                    gameObjects.push_back(floor);
                    pixObjects.push_back(floor);
                    scene->addItem(floor);
                    scene->addItem(player);
                    break;
                }
                default: {
                    break;
                }
            }
         }
    }
}

void game::keyPressEvent(QKeyEvent *event){
    if(playerObject == nullptr || !isGoing || isWon) {
        return;
    }

    bool isValid;
    int m = currentMap[0].size();
    int row = playerObject->row;
    int col = playerObject->col;

    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A){
        isValid = validateMove("left");

        if(isValid) {
            moveLeft();
        }
    }
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D){
        isValid = validateMove("right");

        if(isValid) {
           moveRight();
        }
    }
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W){
        isValid = validateMove("up");

        if(isValid) {
            moveUp();
        }
    }
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S){
        isValid = validateMove("down");

        if(isValid) {
            moveDown();
        }
    }
    else {
        return;
    }

    bool isWin = checkWin();
    if(isWin) {
        winGame();
    }
}

void game::handleButtonMove() {
    if(playerObject == nullptr || !isGoing || isWon) {
        return;
    }

    qDebug() << isWon;

    int m = currentMap[0].size();
    int row = playerObject->row;
    int col = playerObject->col;
    bool isValid;
    QString btnText = qobject_cast<QPushButton*>(sender())->text();

    if (QString::compare(btnText, "left") == 0){
        isValid = validateMove("left");

        if(isValid) {
           moveLeft();
        }
    }
    else if (QString::compare(btnText, "right") == 0){
        isValid = validateMove("right");

        if(isValid) {
           moveRight();
        }
    }
    else if (QString::compare(btnText, "up") == 0){
        isValid = validateMove("up");

        if(isValid) {
            moveUp();
        }
    }
    else if (QString::compare(btnText, "down") == 0){
        isValid = validateMove("down");

        if(isValid) {
            moveDown();
        }
    }
    else {
        return;
    }

    bool isWin = checkWin();
    if(isWin) {
       winGame();
    }
}

void game::moveLeft() {
    int m = currentMap[0].size();
    int row = playerObject->row;
    int col = playerObject->col;

    //checking the crate and moving it if necessary
    if(gameObjects.at(row*m + col - 1)->isCrate) {
       gameObjects.at(row*m + col - 1)->setCrate(false);
       gameObjects.at(row*m + col - 2)->setCrate(true);
    }

    //moving the player
    gameObjects.at(row*m + col)->setPlayer(false);
    gameObjects.at(row*m + col - 1)->setPlayer(true);
    playerObject->setPos(playerObject->x() - squareSize, playerObject->y());
    playerObject->col -=1;
}

void game::moveRight() {
    int m = currentMap[0].size();
    int row = playerObject->row;
    int col = playerObject->col;

    //checking the crate and moving it if necessary
    if(gameObjects.at(row*m + col + 1)->isCrate) {
       gameObjects.at(row*m + col + 1)->setCrate(false);
       gameObjects.at(row*m + col + 2)->setCrate(true);
    }

    //moving the player
    gameObjects.at(row*m + col)->setPlayer(false);
    gameObjects.at(row*m + col + 1)->setPlayer(true);
    playerObject->setPos(playerObject->x() + squareSize, playerObject->y());
    playerObject->col +=1;
}

void game::moveUp() {
    int m = currentMap[0].size();
    int row = playerObject->row;
    int col = playerObject->col;

    //checking the crate and moving it if necessary
    if(gameObjects.at((row-1)*m + col)->isCrate) {
       gameObjects.at((row-1)*m + col)->setCrate(false);
       gameObjects.at((row-2)*m + col)->setCrate(true);
    }

    //moving the player
    gameObjects.at(row*m + col)->setPlayer(false);
    gameObjects.at((row-1)*m + col)->setPlayer(true);
    playerObject->setPos(playerObject->x(), playerObject->y() - squareSize);;
    playerObject->row -=1;
}

void game::moveDown() {
    int m = currentMap[0].size();
    int row = playerObject->row;
    int col = playerObject->col;

    //checking the crate and moving it if necessary
    if(gameObjects.at((row+1)*m + col)->isCrate) {
       gameObjects.at((row+1)*m + col)->setCrate(false);
       gameObjects.at((row+2)*m + col)->setCrate(true);
    }

    //moving the player
    gameObjects.at(row*m + col)->setPlayer(false);
    gameObjects.at((row+1)*m + col)->setPlayer(true);
    playerObject->setPos(playerObject->x(), playerObject->y() + squareSize);
    playerObject->row +=1;
}

void game::loadMap() {
    if(isGoing) {
       clearGame();
    }

    //opening the map file
    QFile file(QFileDialog::getOpenFileName(this,tr("Open File"), "", tr("Text Files (*.txt)")));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.exists()) {
         return;
    }

    //file read preparation and reading
    QTextStream stream(&file);
    QString line = stream.readLine();
    QStringList list = line.split(" ");
    vector<vector<char>> tempMap;
    int cols = list.at(0).toInt();
    int rows = list.at(1).toInt();

    for(int i = 0; i < rows; i++) {
        vector<char> v;
        line = stream.readLine();

        for(int j = 0; j < cols; j++) {
            v.push_back(line.at(j).toLatin1());
        }
        tempMap.push_back(v);
    }

    file.close();
    currentMap = tempMap;
    isGoing = true;
    isWon = false;
    createElements(currentMap);
}

void game::startTestMap() {
    if(isGoing) {
        clearGame();
    }

    isGoing = true;
    isWon = false;
    currentMap = testMap;
    createElements(currentMap);
 }

void game::clearGame() {
    //deletes field objects from the loaded map
    for(int i = 0; i < pixObjects.size(); i++) {
        scene->removeItem(pixObjects[i]);
        delete pixObjects[i];
    }

    scene->removeItem(playerObject);
    playerObject = nullptr;
    gameObjects.clear();
    pixObjects.clear();

    //updates the view of the scene
    viewport()->update();
}

void game::winGame() {
    isWon = true;

    QMessageBox msg(QMessageBox::Information,"Congratulations", "Good job! You have won the game!");
    QFont font;
    font.setBold(true);
    font.setPointSize(14);
    msg.setFont(font);
    msg.exec();
}

bool game::validateMove(string move) {
        bool isValid = true;
        int m = currentMap[0].size();
        int row = playerObject->row;
        int col = playerObject->col;

        if(move == "left") {
            //border control
            if(col == 0) {
                isValid = false;
            }
            //empty field control
            else if(currentMap[row][col - 1] != 'e' && currentMap[row][col - 1] != 't' &&
                    currentMap[row][col - 1] != 'b'  && currentMap[row][col - 1] != 'p' && currentMap[row + 1][col] != 'w') {
                 isValid = false;
            }
            //next to wall control
            else if(currentMap[row][col - 1] == 'w') {
                isValid = false;
            }

            //position check for checks that include two squares at the same time
            else if(col > 1) {
                //box next to wall control
                 if(gameObjects.at(row*m + col - 1)->isCrate && currentMap[row][col - 2] == 'w') {
                    isValid = false;
                  }
                //box next to box control
                else if(gameObjects.at(row*m + col - 1)->isCrate && gameObjects.at(row*m + col - 2)->isCrate) {
                    isValid = false;
                }
            }
        }

        else if(move == "right") {
            //border control
            if(col == m-1) {
                isValid = false;
            }
            //empty field control
            else if(currentMap[row][col + 1] != 'e' && currentMap[row][col + 1] != 't' &&
                    currentMap[row][col + 1] != 'b'  && currentMap[row][col + 1] != 'p' && currentMap[row + 1][col] != 'w') {
                 isValid = false;
            }
            //next to wall control
            else if(currentMap[row][col + 1] == 'w') {
                isValid = false;
            }

            //position check for checks that include two squares at the same time
            else if(col <= m-2) {
                //box next to wall control
                if(gameObjects.at(row*m + col + 1)->isCrate && currentMap[row][col + 2] == 'w') {
                    isValid = false;
                }
                //box next to box control
                else if(gameObjects.at(row*m + col +1)->isCrate && gameObjects.at(row*m + col +2)->isCrate) {
                    isValid = false;
                }
            }
        }

        else if(move == "up") {
            //border control
            if(row == 0) {
                isValid = false;
            }
            //empty field control
            else if(currentMap[row - 1][col] != 'e' && currentMap[row - 1][col] != 't' &&
                    currentMap[row - 1][col] != 'b'  && currentMap[row - 1][col] != 'p' &&  currentMap[row + 1][col] != 'w') {
                 isValid = false;
            }
            //next to wall control
            else if(currentMap[row - 1][col] == 'w') {
                isValid = false;
            }

            //position check for checks that include two squares at the same time
            else if(row >= 2) {
                //box next to wall control
                if(gameObjects.at((row - 1)*m + col)->isCrate && currentMap[row - 2][col] == 'w') {
                    isValid = false;
                }
                //box next to box control
                else if(gameObjects.at((row - 1)*m + col)->isCrate && gameObjects.at((row - 2)*m + col)->isCrate) {
                    isValid = false;
                }
            }
        }

        else if(move == "down") {
            //border control
            if(row == currentMap.size() - 1) {
                isValid = false;
            }
            //empty field control
            else if(currentMap[row + 1][col] != 'e' && currentMap[row + 1][col] != 't' &&
                    currentMap[row + 1][col] != 'b' && currentMap[row + 1][col] != 'p' &&  currentMap[row + 1][col] != 'w') {
                 isValid = false;
            }
            //next to wall control
            else if(currentMap[row + 1][col] == 'w') {
                isValid = false;
            }

            //position check for checks that include two squares at the same time
            else if(row < currentMap.size() - 2) {
                //box next to wall control
                if(gameObjects.at((row + 1)*m + col )->isCrate && currentMap[row + 2][col] == 'w') {
                    isValid = false;
                }
                //box next to box control
                else if(gameObjects.at((row + 1)*m + col)->isCrate && gameObjects.at((row + 2)*m + col )->isCrate) {
                    isValid = false;
                }
            }
        }
        return isValid;
}

bool game::checkWin() {
    bool isWin = true;

    for (int i = 0; i < currentMap.size(); i++) {
         for (int j = 0; j < currentMap[0].size(); j++) {
            if(currentMap[i][j] == 'b' || currentMap[i][j] == 'p' || currentMap[i][j] == 'e' || currentMap[i][j] == 't' || currentMap[i][j] == 'w'){
                gameItem *item = gameObjects.at(i * currentMap[0].size() + j);

                if(item->gameItemType == 1 && item->isCrate) {
                    isWin = false;
                }
            }
         }
    }

    return isWin;
}
