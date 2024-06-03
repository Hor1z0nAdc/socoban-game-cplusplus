#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "game.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    game *view = new game();
    view->show();

    return a.exec();
}

