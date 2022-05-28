#include "mainwindow.h"
#include "player.h"
#include <QApplication>



//TODO: Take Ranges that have been saved and generate CSV file with all segments
//and ask user where to save
//TODO: Release also for LINUX
//VERSION ONE DONE
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("CSV Generator");

    Player player;
    player.show();

    return a.exec();
}
