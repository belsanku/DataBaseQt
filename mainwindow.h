#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QFile>
#include <QDate>
#include <QIODevice>
#include <QSqlError>
#include <QTextCodec>
#include <QDataStream>
#include <QTextStream>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateComboBox();


private slots:
    void on_addStudent_clicked();

    void on_deleteStudent_clicked();

    void on_DBnamespace_clicked(const QModelIndex &index);

    void on_editButton_clicked();

    void on_UpdateButton_clicked();

    void on_Authors_clicked();

    void on_OpenFile_clicked();

    void on_SaveFile_clicked();

    void on_NewPassword_clicked();

    void on_sortDBby_clicked();

    void on_resetButton_clicked();

    void on_partyButton_clicked();

    void on_partyTableView_clicked(const QModelIndex &index);

    void on_deletePartyButton_clicked();

    void on_addPartyButton_clicked();

    void on_updatePartyButton_clicked();

    void on_AddPhoto_clicked();

protected:


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlDatabase partyDB;
    QSqlTableModel *model;
    QSqlTableModel *partyModel;
    QSqlQuery *query;
    QSqlQuery *partyQuery;

    QVector<QString>images;
    int row = 1;
    int partyRow = 1;
};

#endif // MAINWINDOW_H
