#include "mainwindow.h"

#include <dbview.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
{
    CreateUI();
}

void MainWindow::CreateUI()
{
    this->showMaximized();
    this->setWindowTitle( NAME_APP );
    auto gridLayout = new QGridLayout();
    {
        dbView = new DBView( this );

        QPushButton* buttonAddRecord = new QPushButton( "Добавить", this );
        buttonAddRecord->setMinimumWidth( 60 );
        connect( buttonAddRecord, &QPushButton::clicked,
            dbView, &DBView::addRecord );

        QPushButton* buttonRemoveRecord = new QPushButton( "Удалить", this );
        buttonRemoveRecord->setMinimumWidth( 60 );
        connect( buttonRemoveRecord, &QPushButton::clicked,
            dbView, &DBView::removeRecord );

        comboBox = new QComboBox( this );

        gridLayout->addWidget( dbView, 0, 0, 10, 1 );
        gridLayout->addWidget( buttonAddRecord, 0, 1 );
        gridLayout->addWidget( buttonRemoveRecord, 1, 1 );
    }
    auto widget = new QWidget( this );
    widget->setLayout( gridLayout );
    this->setCentralWidget( widget );
}
