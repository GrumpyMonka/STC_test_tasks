#ifndef DBVIEW_H
#define DBVIEW_H

#include <QComboBox>
#include <QTableView>
#include <QtSql>

class DBView : public QWidget
{
    Q_OBJECT
public:
    explicit DBView( QWidget* parent = nullptr );

    bool OpenDB( const QString& );

private:
    void CreateUI();
    void Clear();
    void ChangeTable();

private:
    const QString DB_TYPE = "QSQLITE";

    QString pathToDB;
    QTableView* tableView;
    QComboBox* comboBox;
    QSqlDatabase dbase;
    QHash<QString, QSqlTableModel*> sqlModelsList;
};

#endif // DBVIEW_H
