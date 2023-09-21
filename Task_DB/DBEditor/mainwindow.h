#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <dbview.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = nullptr );

private:
    void CreateUI();
    void CreateMenuBar();

    void ConnectDB( const QString& pathToDB );

private slots:
    void slotOpenDB();

private:
    const QString NAME_APP = "DBEditor";

    DBView* dbView = nullptr;
};
#endif // MAINWINDOW_H
