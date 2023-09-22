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

private:
    const QString NAME_APP = "DBEditor";

    QComboBox* comboBox;
    DBView* dbView = nullptr;
};
#endif // MAINWINDOW_H
