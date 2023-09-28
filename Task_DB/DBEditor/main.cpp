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
    window.showMaximized();
    return appDB.exec();
}
