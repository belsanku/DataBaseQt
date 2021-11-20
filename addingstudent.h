#ifndef ADDINGSTUDENT_H
#define ADDINGSTUDENT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QSqlError>

namespace Ui {
class AddingStudent;
}

class AddingStudent : public QWidget
{
    Q_OBJECT

public:
    explicit AddingStudent(QWidget *parent = 0);
    ~AddingStudent();

private slots:
    void on_DeniedButton_clicked();

    void on_SelectPhotoButton_clicked();

    void on_AcceptButton_clicked();

private:
    Ui::AddingStudent *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlQuery *query;
};

#endif // ADDINGSTUDENT_H
