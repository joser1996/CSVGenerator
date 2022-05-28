#include "mainwindow.h"
#include "player.h"
#include <QApplication>


//TODO: Create TimeRangeView that will have saved ranges
//  - Make sure ranges don't overlap; and make sure that not a duplicate range
//
//TODO: Take Ranges that have been saved and generate CSV file with all segments
//and ask user where to save
//
//TODO: listen for key press when table is focus if SPACE is pressed then get currently
//selected row from table, update time from table, then emit the play signal to the
//media player
//
//TODO: Same for delete; listen for delete key then if press remove selected row if any and
//delete
//
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
