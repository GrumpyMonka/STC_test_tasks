#include "mainwindow.h"

#include <dbview.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
{
    CreateUI();
    CreateMenuBar();
}

void MainWindow::CreateUI()
{
    this->showMaximized();
    this->setWindowTitle( NAME_APP );
    auto gridLayout = new QGridLayout();
    {
        dbView = new DBView( this );
        gridLayout->addWidget( dbView, 0, 0, 0, 0 );
    }
    auto widget = new QWidget( this );
    widget->setLayout( gridLayout );
    this->setCentralWidget( widget );
}

void MainWindow::CreateMenuBar()
{
    auto menuBar = new QMenuBar( this );
    {
        auto menuFile = new QMenu( "Файл", this );
        {
            auto actionOpenDb = new QAction( "Открыть Базу Данных", this );
            actionOpenDb->setShortcut( QString( "Ctrl+O" ) );
            menuFile->addAction( actionOpenDb );
            connect( actionOpenDb, &QAction::triggered,
                this, &MainWindow::slotOpenDB );
        }
        menuBar->addMenu( menuFile );
    }
    this->setMenuBar( menuBar );
}

void MainWindow::slotOpenDB()
{
    this->setWindowTitle( NAME_APP );
    auto pathToDB = QFileDialog::getOpenFileName( this, "Open DB" );
    if ( dbView->OpenDB( pathToDB ) )
    {
        this->setWindowTitle( NAME_APP + "   *   " + pathToDB );
    }
}
