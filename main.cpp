#include "mainwindow/mainwindow.h"

#include <QApplication>
#include "classes/configdb.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigDB db;
    MainWindow w;
    w.show();

    return a.exec();
}
