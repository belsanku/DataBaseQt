#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addingstudent.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./DataBase.db");
    if (db.open())
    {
        qDebug("open");
    }else
        qDebug("open error");

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE Журнал(ФИО TEXT, Дата_Рождения DATE, Группа TEXT, Фотография TEXT, Почта TEXT, Телефон_Родителей TEXT);");

    if( !query->exec())
            qDebug() << "Unable to create table:\n" << query->lastError();

    query->clear();
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addStudent_clicked()
{
    //model->insertRow(model->rowCount());
    AddingStudent *newStudent = new AddingStudent();
    newStudent->show();
}

void MainWindow::on_deleteStudent_clicked()
{
    model->removeRow(row);
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);
}

void MainWindow::on_DBnamespace_clicked(const QModelIndex &index)
{
    row = index.row();
}

void MainWindow::on_editButton_clicked()
{

}

void MainWindow::on_UpdateButton_clicked()
{
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);
}
