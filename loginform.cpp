#include "loginform.h"
#include "ui_loginform.h"
#include "admin.h"
#include <QObject>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QTextCodec>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    login = "admin";
    password = "admin";
    ui->ErrorLabel->setVisible(false);
    connect(ui->OkayButton, SIGNAL(clicked()), this, SLOT(CheckAccount()));
    connect(ui->CancelButton, SIGNAL(clicked()), this, SLOT(ExitApplication()));
}

void LoginForm::CheckAccount()
{
    //Изменение цвета ErrorLabel;
    ui->ErrorLabel->setStyleSheet("color: #E51837");

    //Проверка полей на пустоту;
    QString tempLogin = "";
    QString tempPassword = "";
    tempLogin = ui->LoginLine->text();
    tempPassword = ui->PasswordLine->text();
    if (tempLogin != login || tempPassword != password)
    {
        ui->ErrorLabel->setVisible(true);
    }else{
        ui->ErrorLabel->setVisible(false);
        LoginForm::start = true;
        MainWindow *WorkSpace = new MainWindow();
        WorkSpace->show();
        this->close();
    }
}

void LoginForm::ExitApplication()
{
    this->close();
}

LoginForm::~LoginForm()
{
    delete ui;
}


