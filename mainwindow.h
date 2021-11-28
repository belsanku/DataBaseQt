#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QFile>
#include <QDate>
#include <QIODevice>
#include <QStandardItemModel>
#include <QSqlError>
#include <QTextCodec>
#include <QTextStream>
#include <QTextFormat>
#include <QTextDocument>
#include <QPainter>
#include <QTextTableCell>
#include <QListWidget>
#include <QDataStream>
#include <QSet>
#include <QVector>
#include <QPdfWriter>

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

    void create_Students_folder();

    void update_PartyStudents_combo();

    void fillTableView();

    void saveSuccessTableView();

    void readSuccessTableView();

    void createSuccessTableView();

    void update_successComboBox();

    void createAkatsukiTableView();

    void update_akatsukiComboBox();

    void update_leaderOfAkatsukiComboBox();

    void checkSuccessFile();

    void readAkatsukiTable();

    void checkAkatsukiSuccessFile();


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

    void on_ChooseParty_currentIndexChanged(int index);

    void on_BeforeImage_clicked();

    void on_NextImage_clicked();

    void on_addStudentButton_clicked();

    void on_addingPartyStudents_highlighted(int index);

    void on_addingPartyStudents_currentIndexChanged(int index);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_saveInfo_clicked();

    void on_succButton_clicked();

    void on_masterButton_clicked();

    void on_saveSuccessTableViewButton_clicked();

    void on_setResultSession_clicked();

    void on_unsuccessfulStudentsTableViewButton_clicked();

    void on_resetSuccessTableView_clicked();

    void on_showGroupButton_clicked();

    void on_searchButton_clicked();

    void on_createPdfSuccessTableViewButton_clicked();

    void on_chooseGroupOfAkatsuki_currentTextChanged(const QString &arg1);

    void on_addAkatsukiLeader_clicked();

    void on_saveAkatsukiTableButton_clicked();

    void on_akatsukiTableView_doubleClicked(const QModelIndex &index);

    void on_akatsukiTableView_clicked(const QModelIndex &index);

    void on_akatsukiTableView_activated(const QModelIndex &index);

    void on_deleteLeaderButton_clicked();

protected:


private:
    Ui::MainWindow *ui;

    QSqlDatabase db;
    QSqlDatabase partyDB;
    QSqlTableModel *model;
    QSqlTableModel *partyModel;
    QSqlQuery *query;
    QSqlQuery *partyQuery;

    QStandardItemModel* csvModel;
    QStandardItemModel* successModel;
    QStandardItemModel* leaderModel;
    QStandardItemModel* leaderCsvModel;

    QSet<QString> uniqueGroup;

    QVector<QString>images;
    QVector<QString>StudentName;
    QVector<QString>StudentGroup;
    QVector<QString>ColumnNames;
    QVector<QString>AkatsukiColumnNames;

    int row = 1;
    int partyRow = 1;
    int leaderRow = 1;
    int cnt = 0;
};

#endif // MAINWINDOW_H
