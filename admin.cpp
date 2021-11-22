#include "admin.h"
#include "ui_admin.h"
#include "loginform.h"

admin::admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin)
{
    ui->setupUi(this);
    ui->CheckLabel->setVisible(false);
}

admin::~admin()
{
    delete ui;
}

void admin::on_ChangeButton_clicked()
{
    QString oldPassword = "";
    QString oldLogin = "";
    oldPassword = ui->oldPasswordEdit->text();
    oldLogin = ui->oldLoginEdit->text();
    this->close();
    /*if (oldPassword != LoginForm::password || oldLogin != LoginForm::login)
    {
        ui->CheckLabel->setVisible(true);
    }else{
       // LoginForm::password = ui->newPasswordEdit->text();
        //LoginForm::login = ui->newLoginEdit->text();
        this->close();
    }*/
}
