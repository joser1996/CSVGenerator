#include "mainwindow.h"
#include "player.h"
#include <QApplication>
//have music play from left of range slider with extra play button
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("CSV Generator");

    Player player;
    player.show();

    return a.exec();
}
