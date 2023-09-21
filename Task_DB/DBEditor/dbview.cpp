#include "dbview.h"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTableView>

DBView::DBView( QWidget* parent )
    : QWidget( parent )
{
    CreateUI();
}

void DBView::CreateUI()
{
    QGridLayout* gridLayout = new QGridLayout();
    {
        QLabel* label = new QLabel( "Таблицы:", this );
        label->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );

        comboBox = new QComboBox( this );
        connect( comboBox, &QComboBox::currentTextChanged,
            this, &DBView::ChangeTable );

        tableView = new QTableView( this );

        gridLayout->addWidget( label, 0, 0 );
        gridLayout->addWidget( comboBox, 0, 1 );
        gridLayout->addWidget( tableView, 1, 0, 1, 8 );
    }
    this->setLayout( gridLayout );
}

bool DBView::OpenDB( const QString& new_pathToDB )
{
    Clear();
    pathToDB = new_pathToDB;
    dbase = QSqlDatabase::addDatabase( DB_TYPE, pathToDB );
    dbase.setDatabaseName( pathToDB );
    if ( !dbase.open() )
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "Ошибка загрузки Базы Данных!\n %1" ).arg( dbase.lastError().text() ) );
        return false;
    }
    auto tables = dbase.tables();
    if ( tables.empty() )
    {
        QMessageBox::warning( this, this->metaObject()->className(), QString( "В базе данных отсутсвуют таблицы!\n %1" ).arg( pathToDB ) );
        return false;
    }
    comboBox->addItems( dbase.tables() );

    for ( auto table : tables )
    {
        QSqlTableModel* sqlModel = new QSqlTableModel( this, dbase );
        sqlModel->setTable( table );
        sqlModel->select();
        while ( sqlModel->canFetchMore() )
        {
            sqlModel->fetchMore();
        }
        sqlModel->setEditStrategy( QSqlTableModel::OnFieldChange );
        sqlModelsList.insert( table, sqlModel );
    }
    return true;
}

void DBView::ChangeTable()
{
    tableView->setModel( sqlModelsList.value( comboBox->currentText() ) );
    tableView->show();
}

void DBView::Clear()
{
    for ( auto sqlModel : sqlModelsList )
    {
        sqlModel->clear();
    }
    sqlModelsList.clear();
    dbase.close();
    comboBox->clear();
    pathToDB.clear();
}
