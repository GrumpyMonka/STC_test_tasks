#ifndef DBVIEW_H
#define DBVIEW_H

#include <QTableView>
#include <QtSql>

class DBView : public QTableView
{
    Q_OBJECT
public:
    explicit DBView( QWidget* parent = nullptr );

public slots:
    void addRecord();
    void removeRecord();

private:
    void LoadDB();
    void ConvertPopulation( double ratio );

private:
    const QString DB_TYPE = "QSQLITE";
    const QString DB_PATH = "cities.db3";
    const QString DB_TABLE = "cities";

    QSqlTableModel* sqlModel;
};

#endif // DBVIEW_H
