/**
 * @file main.cpp
 * @brief Точка старта приложения
 */

#include "mainwindow.h"

#include <QApplication>

int main( int argc, char* argv[] )
{
    QApplication appDB( argc, argv );
    MainWindow window;
    window.show();
    qDebug() << QStandardPaths::standardLocations( QStandardPaths::AppDataLocation )[3];

    return appDB.exec();
}
