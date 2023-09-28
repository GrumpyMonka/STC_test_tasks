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
    // Устанавливаем название приложения
    this->setWindowTitle( NAME_APP );

    // Создаём основной лайаут на который будем всё накидывать
    auto gridLayout = new QGridLayout();
    {
        // Надпись
        labelCountRecords = new QLabel( "Кол-во записей: 0" );
        labelCountRecords->setMaximumHeight( 25 );

        // Таблица БД ( обьявлена после лейбла, потому как при старте LoadDB() изменит значение, а вызывать  LoadDB() ниже не хочется )
        dbView = new DBView( this );
        connect( dbView, &DBView::dbLoaded,
            this, &MainWindow::on_dbLoaded );
        dbView->LoadDB();

        // Кнопка для добавления записи в таблицу
        QPushButton* buttonAddRecord = new QPushButton( "Добавить", this );
        buttonAddRecord->setMinimumWidth( 60 );
        connect( buttonAddRecord, &QPushButton::clicked,
            dbView, &DBView::addRecord );

        // Добавление кнопки для удаления записей из таблицы
        QPushButton* buttonRemoveRecord = new QPushButton( "Удалить", this );
        buttonRemoveRecord->setMinimumWidth( 60 );
        connect( buttonRemoveRecord, &QPushButton::clicked,
            dbView, &DBView::removeRecord );

        // Фильтр для населения
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

        // Надпись для фильтра
        QLabel* labelFilter = new QLabel( "Фильтр SQL:", this );
        labelFilter->setMaximumHeight( 25 );

        // Поле фильтра
        textEditFilter = new QTextEdit( this );
        textEditFilter->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding );

        // Кнопка для применения фильтра
        QPushButton* buttonFilter = new QPushButton( "Применить фильтр", this );
        connect( buttonFilter, &QPushButton::clicked,
            this, &MainWindow::filterChanged );

        // Накидывание элементов на форму
        gridLayout->addWidget( dbView, 0, 0, 10, 1 );
        gridLayout->addWidget( labelCountRecords, 0, 1 );
        gridLayout->addWidget( buttonAddRecord, 1, 1 );
        gridLayout->addWidget( buttonRemoveRecord, 2, 1 );
        gridLayout->addWidget( comboBox, 3, 1 );
        gridLayout->addWidget( labelFilter, 4, 1 );
        gridLayout->addWidget( textEditFilter, 5, 1 );
        gridLayout->addWidget( buttonFilter, 6, 1 );
    }

    // Установаливаем лайаут на форму
    auto widget = new QWidget( this );
    widget->setLayout( gridLayout );
    this->setCentralWidget( widget );
}

void MainWindow::comboBoxItemChanged()
{
    // Фильтра на население
    dbView->setFilter( "population>=" + QString::number( filter_population[comboBox->currentIndex()] * 1000 ) );
}

void MainWindow::on_dbLoaded( int countRecords )
{
    // Вывод кол-ва найденных записей
    labelCountRecords->setText( "Кол-во записей: " + QString::number( countRecords ) );
}

void MainWindow::filterChanged()
{
    // Фильтр пользовательский + фильтр населения
    QString filter = "population>=" + QString::number( filter_population[comboBox->currentIndex()] * 1000 );
    if ( !textEditFilter->toPlainText().isEmpty() )
    {
        filter += " AND " + textEditFilter->toPlainText();
    }
    dbView->setFilter( filter );
}
