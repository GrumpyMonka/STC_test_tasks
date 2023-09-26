/**
 * @file dbview.h
 * @brief Виджет-таблица для просмотра Базы Данных
 */

#ifndef DBVIEW_H
#define DBVIEW_H

#include <QStandardItemModel>
#include <QTableView>
#include <QtSql>

/**
 * @brief Класс отвечающий за отображение данных из БД в таблице
 */
class DBView : public QTableView
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор нового DBView 
     * 
     * @param parent указатель на родительский виджет
     */
    explicit DBView( QWidget* parent = nullptr );

    /**
     * @brief Загрузить БД
     */
    void LoadDB();

    /**
     * @brief Деструктор
     */
    ~DBView();

public slots:
    /**
     * @brief Добавить запись в таблицу
     */
    void addRecord();

    /**
     * @brief Удалить выбранные записи
     */
    void removeRecord();

    /**
     * @brief Обработать изменение значения поля в таблице
     * 
     * @param item указатель на измененный элемент
     */
    void itemChanged( QStandardItem* item );

    /**
     * @brief Установить фильтр для таблицы
     * 
     * @param filter строка с фильтром
     */
    void setFilter( const QString& filter );

private:
    /**
     * @brief Загрузка данных из БД с фильтром
     * 
     * @param filter строка с фильтром, по умолчанию загружает всё
     */
    void LoadDataFormDB( const QString& filter = "TRUE" );

signals:
    /**
     * @brief Сигнал информирующий о том, что БД загружена 
     * 
     * @param countRecord кол-во записей, которые отображаются в таблице
     */
    void dbLoaded( int countRecord );

private:
    /**
     * @brief Параметры для БД
     */
    const QString DB_TYPE = "QSQLITE";
    const QString DB_PATH = QStandardPaths::standardLocations( QStandardPaths::AppDataLocation )[3] + "/" + "cities.db3";
    const QString DB_TABLE = "cities";
    const QString DB_TABLE_PK_NAME = "id";

    /**
     * @brief Параметры для отображения населения в тысячах
     */
    const double RATIO_POPULATION = 1000;
    const QString NEW_POPULATION_NAME = "population_in_thousands";

    /**
     * @brief Список колонок в БД
     */
    const QStringList COLUMNS{
        "prefix",
        "name",
        "map_point",
        "population",
        "country",
        "lat",
        "lon",
        "description"
    };

    /**
     * @brief Список колонок с новым названием для населения
     */
    const QStringList COLUMNS_NEW{
        COLUMNS[0],
        COLUMNS[1],
        COLUMNS[2],
        NEW_POPULATION_NAME,
        COLUMNS[4],
        COLUMNS[5],
        COLUMNS[6],
        COLUMNS[7]
    };

    const QString COLUMNS_LINE = COLUMNS.join( ',' );
    const QString COLUMNS_LINE_VALUE = ":" + COLUMNS.join( ", :" );

    /**
     * @brief Запросы к БД
     */
    const QString QUERY_GET_ALL = QString( "SELECT *, population / %1 AS %2 FROM %3 WHERE %4;" ).arg( QString::number( RATIO_POPULATION ) ).arg( NEW_POPULATION_NAME ).arg( DB_TABLE );
    const QString QUERY_INSERT = QString( "INSERT INTO %1(parent_id, %2) VALUES (0, %3);" ).arg( DB_TABLE ).arg( COLUMNS_LINE ).arg( COLUMNS_LINE_VALUE );
    const QString QUERY_REMOVE = QString( "DELETE FROM %1 WHERE %2 IN (%3);" ).arg( DB_TABLE ).arg( DB_TABLE_PK_NAME );
    const QString QUERY_CHANGE = QString( "UPDATE %1 SET %3 = :%3 WHERE %2 = :%2;" ).arg( DB_TABLE ).arg( DB_TABLE_PK_NAME );

    QStandardItemModel* sqlModel;
    QSqlDatabase dataBase;
};

#endif // DBVIEW_H
