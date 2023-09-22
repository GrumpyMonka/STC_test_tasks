#include "dbview.h"

#include <QComboBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QTableView>

DBView::DBView( QWidget* parent )
    : QTableView( parent )
{
    LoadDB();
}

void DBView::LoadDB()
{
    if ( !QFile::exists( DB_PATH ) )
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "База Данных не найдена!\n %1" ).arg( DB_PATH ) );
        return;
    }

    QSqlDatabase dbase = QSqlDatabase::addDatabase( DB_TYPE );
    dbase.setDatabaseName( DB_PATH );
    if ( !dbase.open() )
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "Ошибка загрузки Базы Данных!\n %1" ).arg( dbase.lastError().text() ) );
        return;
    }

    sqlModel = new QSqlTableModel( this, dbase );
    sqlModel->setTable( DB_TABLE );
    sqlModel->select();
    while ( sqlModel->canFetchMore() )
    {
        sqlModel->fetchMore();
    }
    sqlModel->setEditStrategy( QSqlTableModel::OnFieldChange );
    sqlModel->setHeaderData( 3, Qt::Horizontal, "Префикс" );
    sqlModel->setHeaderData( 4, Qt::Horizontal, "Название" );
    sqlModel->setHeaderData( 5, Qt::Horizontal, "Название ENG" );
    sqlModel->setHeaderData( 6, Qt::Horizontal, "Точка привязки" );
    sqlModel->setHeaderData( 7, Qt::Horizontal, "Широта" );
    sqlModel->setHeaderData( 8, Qt::Horizontal, "Долгота" );
    sqlModel->setHeaderData( 10, Qt::Horizontal, "Население (тыс)" );
    sqlModel->setHeaderData( 11, Qt::Horizontal, "Описание" );

    this->setModel( sqlModel );
    this->hideColumn( 0 );
    this->hideColumn( 1 );
    this->hideColumn( 2 );
    this->hideColumn( 9 );
    this->hideColumn( 12 );
    this->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
}

void DBView::addRecord()
{
    auto index = this->currentIndex();
    if ( index.isValid() )
    {
        sqlModel->insertRow( index.row() );
    }
}
/*
префикс (в бд prefix)
название (в бд name)
точка привязки (в бд map_point)
население (в бд population, округлять до тыс.)
страна (в бд country)
широта (в бд lat)
долгота (в бд lon)
описание (в бд description));
*/
void DBView::removeRecord()
{
    auto index = this->currentIndex();
    if ( index.isValid() )
    {
        sqlModel->removeRow( this->currentIndex().row() );
        this->hideRow( this->currentIndex().row() );
    }
}
