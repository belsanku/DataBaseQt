#include "loginform.h"
#include "ui_loginform.h"
#include <QObject>
#include <QString>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    ui->ErrorLabel->setVisible(false);
    connect(ui->OkayButton, SIGNAL(clicked()), this, SLOT(CheckAccount()));
    connect(ui->CancelButton, SIGNAL(clicked()), this, SLOT(ExitApplication()));
}

void LoginForm::CheckAccount()
{
    //Изменение цвета ErrorLabel;
    ui->ErrorLabel->setStyleSheet("color: #E51837");

    //Проверка полей на пустоту;
    QString login = "";
    QString password = "";
    login = ui->LoginLine->text();
    password = ui->PasswordLine->text();
    if (login.isEmpty() || password.isEmpty())
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
