#include "addingstudent.h"
#include "ui_addingstudent.h"
#include "mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QString>
#include <QLabel>
#include <QFile>
#include <QDateTime>

AddingStudent::AddingStudent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddingStudent)
{
    ui->setupUi(this);
}

AddingStudent::~AddingStudent()
{
    delete ui;
}

void AddingStudent::on_DeniedButton_clicked()
{
    this->close();
}

void AddingStudent::on_SelectPhotoButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(0, "Открыть изображение", QDir::currentPath(), "Файлы изображений (*.png *.jpg *.bmp)");
    QPixmap pixmap(filename);
    ui->PhotoLabel->setPixmap(pixmap);
    ui->PhotoPathLabel->setText(filename);
}

void AddingStudent::on_AcceptButton_clicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./DataBase.db");
    if (db.open())
    {
        qDebug("open");
    }else
        qDebug("open error");

    query = new QSqlQuery(db);

    QString name = ui->FamilyLine->text();
    QString birth = ui->dateEdit->text();
    QString group = ui->GroupEdit->text();
    QString photo = ui->PhotoPathLabel->text();
    QString mail = ui->MailEdit->text();
    QString phone = ui->PhoneEdit->text();

    query->prepare("INSERT INTO Журнал(ФИО, Дата_Рождения, Группа, Фотография, Почта, Телефон_Родителей) VALUES (:name, :birth, :group, :photo, :mail, :phone);");
    query->bindValue(":name", name);
    query->bindValue(":birth", birth);
    query->bindValue(":group", group);
    query->bindValue(":photo", photo);
    query->bindValue(":mail", mail);
    query->bindValue(":phone", phone);

    query->exec();
    query->clear();

    this->close();
}
