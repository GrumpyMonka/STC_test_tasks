/**
 * @file dbview.cpp
 * @brief Виджет-таблица для просмотра Базы Данных
 */

#include "dbview.h"

#include <QFile>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QTableView>
#include <QtSql>

DBView::DBView( QWidget* parent )
    : QTableView( parent )
{
    // Создание модели
    sqlModel = new QStandardItemModel( this );
    setModel( sqlModel );
    connect( sqlModel, &QStandardItemModel::itemChanged,
        this, &DBView::itemChanged );

    // Магия, чтобы столбцы растягивались на всю таблицу, а не теснились слева
    this->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
}

DBView::~DBView()
{
    dataBase.close();
}

void DBView::LoadDataFormDB( const QString& filter )
{
    // Установка названия столбцов
    sqlModel->clear();
    sqlModel->setHorizontalHeaderLabels( QStringList() << "Перфикс"
                                                       << "Название"
                                                       << "Точка привязки"
                                                       << "Население (тыс)"
                                                       << "Страна"
                                                       << "Широта"
                                                       << "Долгота"
                                                       << "Описание" );

    // Запрос для получения всех записей, удовлетворяющих фильтру
    QSqlQuery sqlQuery;
    if ( sqlQuery.exec( QString( QUERY_GET_ALL ).arg( filter ) ) )
    {
        while ( sqlQuery.next() )
        {
            // Получение записей и занесение в модель таблицы
            QList<QStandardItem*> itemList;
            for ( auto column : COLUMNS_NEW )
            {
                itemList.append( new QStandardItem( sqlQuery.value( column ).toString() ) );
            }
            // Скрытая колонка с id, чтобы иметь возможность нормально обращаться к БД
            itemList.append( new QStandardItem( sqlQuery.value( DB_TABLE_PK_NAME ).toString() ) );
            sqlModel->appendRow( itemList );
        }
    }
    else
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "Ошибка загрузки данных из БД. %1" ).arg( sqlQuery.lastError().text() ) );
    }
    // Скрываем столбец с id
    this->hideColumn( sqlModel->columnCount() - 1 );
    emit dbLoaded( sqlModel->rowCount() );
}

void DBView::LoadDB()
{
    // Грузим БД
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
    // Запрос на добавление пустой строки
    QSqlQuery sqlQuery;
    sqlQuery.prepare( QUERY_INSERT );
    for ( auto value : COLUMNS )
    {
        sqlQuery.bindValue( ":" + value, "" );
    }

    // Если запрос отработал успешно, то добавляем в модель таблицы
    if ( sqlQuery.exec() )
    {
        QList<QStandardItem*> itemList;
        for ( int i = 0; i < COLUMNS.size(); ++i )
        {
            itemList.append( new QStandardItem( "" ) );
        }
        // Получаем id добавленной записи
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
    // Получаем индексы для удаления
    QStringList indexes;
    auto selectedRows = this->selectionModel()->selectedIndexes();
    for ( auto index : selectedRows )
    {
        indexes.push_back( sqlModel->item( index.row(), sqlModel->columnCount() - 1 )->text() );
    }

    // Отправляем запрос на удаление и, если он прошёл, то удаляем эти элементы из таблицы
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
    // Получаем название полонки в которой произошло изменение
    QString name_column = COLUMNS.at( item->column() );

    // Составляем запрос на изменение
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
        // Пусть будет
    }
    else
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "Не удалось изменить запись в таблице. %1. %2" ).arg( sqlQuery.lastError().text() ).arg( sqlQuery.lastQuery() ) );
    }
}

void DBView::setFilter( const QString& filter )
{
    // Применение фильтра
    LoadDataFormDB( filter );
}
