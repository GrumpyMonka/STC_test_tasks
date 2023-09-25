#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QTextEdit>

#include <dbview.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = nullptr );

private:
    void CreateUI();

private slots:
    void comboBoxItemChanged();
    void filterChanged();
    void on_dbLoaded( int );

private:
    const QString NAME_APP = "DBEditor";

    QComboBox* comboBox;
    DBView* dbView = nullptr;
    QLabel* labelCountRecords;
    QTextEdit* textEditFilter;
    QVector<int>
        filter_population = {
            0,
            1,
            10,
            25,
            50,
            100,
            250,
            500,
            1000,
            2000,
        };
};
#endif // MAINWINDOW_H
