#include "mainwindow.h"
#include "player.h"
#include <QApplication>


//TODO: Create TimeRangeView that will have saved ranges
//  - Make sure ranges don't overlap; and make sure that not a duplicate range
//
//TODO: Take Ranges that have been saved and generate CSV file with all segments
//and ask user where to save
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("CSV Generator");

    Player player;
    player.show();

    return a.exec();
}
