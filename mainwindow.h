#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QSqlError>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_addStudent_clicked();

    void on_deleteStudent_clicked();

    void on_DBnamespace_clicked(const QModelIndex &index);

    void on_editButton_clicked();

    void on_UpdateButton_clicked();

protected:


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlQuery *query;

    int row = 1;
};

#endif // MAINWINDOW_H
