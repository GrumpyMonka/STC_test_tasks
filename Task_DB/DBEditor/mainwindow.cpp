/**
 * @file mainwindow.cpp
 * @brief Главное окно программы
 */

#include "mainwindow.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
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
        labelCountRecords = new QLabel( "Кол-во записей: 0" );
        labelCountRecords->setMaximumHeight( 25 );

        dbView = new DBView( this );
        connect( dbView, &DBView::dbLoaded,
            this, &MainWindow::on_dbLoaded );
        dbView->LoadDB();

        QPushButton* buttonAddRecord = new QPushButton( "Добавить", this );
        buttonAddRecord->setMinimumWidth( 60 );
        connect( buttonAddRecord, &QPushButton::clicked,
            dbView, &DBView::addRecord );

        QPushButton* buttonRemoveRecord = new QPushButton( "Удалить", this );
        buttonRemoveRecord->setMinimumWidth( 60 );
        connect( buttonRemoveRecord, &QPushButton::clicked,
            dbView, &DBView::removeRecord );

        comboBox = new QComboBox( this );
        comboBox->addItems( QStringList() << "Население не учитывать."
                                          << "Население от 1 тыс. чел."
                                          << "Население от 10 тыс. чел."
                                          << "Население от 25 тыс. чел."
                                          << "Население от 50 тыс. чел."
                                          << "Население от 100 тыс. чел."
                                          << "Население от 250 тыс. чел."
                                          << "Население от 500 тыс. чел."
                                          << "Население от 1 млн. чел."
                                          << "Население от 2 млн. чел." );

        connect( comboBox, &QComboBox::currentTextChanged,
            this, &MainWindow::comboBoxItemChanged );

        QLabel* labelFilter = new QLabel( "Фильтр SQL:", this );
        labelFilter->setMaximumHeight( 25 );

        textEditFilter = new QTextEdit( this );
        textEditFilter->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding );

        QPushButton* buttonFilter = new QPushButton( "Применить фильтр", this );
        connect( buttonFilter, &QPushButton::clicked,
            this, &MainWindow::filterChanged );

        gridLayout->addWidget( dbView, 0, 0, 10, 1 );
        gridLayout->addWidget( labelCountRecords, 0, 1 );
        gridLayout->addWidget( buttonAddRecord, 1, 1 );
        gridLayout->addWidget( buttonRemoveRecord, 2, 1 );
        gridLayout->addWidget( comboBox, 3, 1 );
        gridLayout->addWidget( labelFilter, 4, 1 );
        gridLayout->addWidget( textEditFilter, 5, 1 );
        gridLayout->addWidget( buttonFilter, 6, 1 );
    }
    auto widget = new QWidget( this );
    widget->setLayout( gridLayout );
    this->setCentralWidget( widget );
}

void MainWindow::comboBoxItemChanged()
{
    dbView->setFilter( "population>=" + QString::number( filter_population[comboBox->currentIndex()] * 1000 ) );
}

void MainWindow::on_dbLoaded( int countRecords )
{
    labelCountRecords->setText( "Кол-во записей: " + QString::number( countRecords ) );
}

void MainWindow::filterChanged()
{
    QString filter = "population>=" + QString::number( filter_population[comboBox->currentIndex()] * 1000 );
    if ( !textEditFilter->toPlainText().isEmpty() )
    {
        filter += " AND " + textEditFilter->toPlainText();
    }
    dbView->setFilter( filter );
}
