#include "dbview.h"

#include <QFile>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QTableView>
#include <QtSql>

#include <QDebug>

DBView::DBView( QWidget* parent )
    : QTableView( parent )
{
    sqlModel = new QStandardItemModel( this );
    setModel( sqlModel );
    connect( sqlModel, &QStandardItemModel::itemChanged,
        this, &DBView::itemChanged );
    this->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
}

DBView::~DBView()
{
    dataBase.close();
}

void DBView::LoadDataFormDB( const QString& filter )
{
    sqlModel->clear();
    sqlModel->setHorizontalHeaderLabels( QStringList() << "Перфикс"
                                                       << "Название"
                                                       << "Точка привязки"
                                                       << "Население (тыс)"
                                                       << "Страна"
                                                       << "Широта"
                                                       << "Долгота"
                                                       << "Описание" );

    QSqlQuery sqlQuery;
    if ( sqlQuery.exec( QString( QUERY_GET_ALL ).arg( filter ) ) )
    {
        while ( sqlQuery.next() )
        {
            QList<QStandardItem*> itemList;
            for ( auto column : COLUMNS_NEW )
            {
                itemList.append( new QStandardItem( sqlQuery.value( column ).toString() ) );
            }
            itemList.append( new QStandardItem( sqlQuery.value( DB_TABLE_PK_NAME ).toString() ) );
            sqlModel->appendRow( itemList );
        }
    }
    else
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "Ошибка загрузки данных из БД. %1" ).arg( sqlQuery.lastError().text() ) );
    }
    this->hideColumn( sqlModel->columnCount() - 1 );
    emit dbLoaded( sqlModel->rowCount() );
}

void DBView::LoadDB()
{
    if ( !QFile::exists( DB_PATH ) )
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "База Данных не найдена!\n %1" ).arg( DB_PATH ) );
        return;
    }

    dataBase = QSqlDatabase::addDatabase( DB_TYPE );
    dataBase.setDatabaseName( DB_PATH );
    if ( !dataBase.open() )
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "Ошибка загрузки Базы Данных!\n %1" ).arg( dataBase.lastError().text() ) );
        return;
    }

    LoadDataFormDB();
}

void DBView::addRecord()
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare( QUERY_INSERT );
    for ( auto value : COLUMNS )
    {
        sqlQuery.bindValue( ":" + value, "" );
    }

    if ( sqlQuery.exec() )
    {
        QList<QStandardItem*> itemList;
        for ( int i = 0; i < COLUMNS.size(); ++i )
        {
            itemList.append( new QStandardItem( "" ) );
        }
        itemList.append( new QStandardItem( sqlQuery.lastInsertId().toString() ) );
        sqlModel->appendRow( itemList );
        this->selectionModel()->select( itemList.first()->index(), QItemSelectionModel::ClearAndSelect );
        this->scrollToBottom();
    }
    else
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "Не удалось добавить запись в таблицу. %1. %2" ).arg( sqlQuery.lastError().text() ).arg( sqlQuery.lastQuery() ) );
    }
    emit dbLoaded( sqlModel->rowCount() );
}

void DBView::removeRecord()
{
    QStringList indexes;
    auto selectedRows = this->selectionModel()->selectedIndexes();
    for ( auto index : selectedRows )
    {
        indexes.push_back( sqlModel->item( index.row(), sqlModel->columnCount() - 1 )->text() );
    }

    QSqlQuery sqlQuery;
    if ( sqlQuery.exec( QString( QUERY_REMOVE ).arg( indexes.join( ", " ) ) ) )
    {
        for ( auto index : selectedRows )
        {
            sqlModel->removeRow( index.row() );
        }
    }
    else
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "Не удалось удалить запись из таблицы. %1. %2" ).arg( sqlQuery.lastError().text() ).arg( sqlQuery.lastQuery() ) );
    }
    emit dbLoaded( sqlModel->rowCount() );
}

void DBView::itemChanged( QStandardItem* item )
{
    QString name_column = COLUMNS.at( item->column() );
    QSqlQuery sqlQuery;
    QString value = item->text();
    if ( "population" == name_column )
    {
        value += " * " + QString::number( RATIO_POPULATION );
    }
    sqlQuery.prepare( QString( QUERY_CHANGE ).arg( name_column ) );
    sqlQuery.bindValue( ":" + DB_TABLE_PK_NAME, sqlModel->item( item->row(), sqlModel->columnCount() - 1 )->text() );
    sqlQuery.bindValue( ":" + name_column, value );
    if ( sqlQuery.exec() )
    {
    }
    else
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "Не удалось изменить запись в таблице. %1. %2" ).arg( sqlQuery.lastError().text() ).arg( sqlQuery.lastQuery() ) );
    }
}

void DBView::setFilter( const QString& filter )
{
    LoadDataFormDB( filter );
}
