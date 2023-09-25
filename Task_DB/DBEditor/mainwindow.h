/**
 * @file mainwindow.h
 * @brief Главное окно программы
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QTextEdit>

#include <dbview.h>

/**
 * @brief Класс отвечающий за главное окно программы
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор нового Main Window 
     * 
     * @param parent указатель на родительский виджет 
     */
    MainWindow( QWidget* parent = nullptr );

private:
    /**
     * @brief Создать внешний вид приложения
     */
    void CreateUI();

private slots:
    /**
     * @brief Обработать изменения выбранного значения в comboBox
     */
    void comboBoxItemChanged();

    /**
     * @brief Обработать фильтр
     */
    void filterChanged();

    /**
     * @brief Обработать сигнал о том, что БД загружена
     */
    void on_dbLoaded( int );

private:
    const QString NAME_APP = "DBEditor";

    QComboBox* comboBox;
    DBView* dbView = nullptr;
    QLabel* labelCountRecords;
    QTextEdit* textEditFilter;

    /**
     * @brief Вектор со значениями соответствующими индексам на comboBox
     */
    QVector<int> filter_population = {
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
