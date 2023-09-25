#ifndef DBVIEW_H
#define DBVIEW_H

#include <QStandardItemModel>
#include <QTableView>
#include <QtSql>

class DBView : public QTableView
{
    Q_OBJECT
public:
    explicit DBView( QWidget* parent = nullptr );
    void LoadDB();
    ~DBView();

public slots:
    void addRecord();
    void removeRecord();
    void itemChanged( QStandardItem* item );
    void setFilter( const QString& filter );

private:
    void LoadDataFormDB( const QString& filter = "TRUE" );

signals:
    void dbLoaded( int countRecord );

private:
    const QString DB_TYPE = "QSQLITE";
    const QString DB_PATH = "cities.db3";
    const QString DB_TABLE = "cities";
    const QString DB_TABLE_PK_NAME = "id";

    const double RATIO_POPULATION = 1000;
    const QString NEW_POPULATION_NAME = "population_in_thousands";

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

    const QStringList COLUMNS_NEW{
        "prefix",
        "name",
        "map_point",
        NEW_POPULATION_NAME,
        "country",
        "lat",
        "lon",
        "description"
    };

    const QString COLUMNS_LINE = COLUMNS.join( ',' );
    const QString COLUMNS_LINE_VALUE = ":" + COLUMNS.join( ", :" );

    const QString QUERY_GET_ALL = QString( "SELECT *, population / %1 AS %2 FROM %3 WHERE %4;" ).arg( QString::number( RATIO_POPULATION ) ).arg( NEW_POPULATION_NAME ).arg( DB_TABLE );
    const QString QUERY_INSERT = QString( "INSERT INTO %1(parent_id, %2) VALUES (0, %3);" ).arg( DB_TABLE ).arg( COLUMNS_LINE ).arg( COLUMNS_LINE_VALUE );
    const QString QUERY_REMOVE = QString( "DELETE FROM %1 WHERE %2 IN (%3);" ).arg( DB_TABLE ).arg( DB_TABLE_PK_NAME );
    const QString QUERY_CHANGE = QString( "UPDATE %1 SET %3 = :%3 WHERE %2 = :%2;" ).arg( DB_TABLE ).arg( DB_TABLE_PK_NAME );

    QStandardItemModel* sqlModel;
    QSqlDatabase dataBase;
};

#endif // DBVIEW_H
