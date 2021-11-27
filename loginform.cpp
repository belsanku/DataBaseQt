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
    //Создание директории, хранящей логин и пароль//
    QString accountInfo = "D:/qt/CourseWork/source/Gallery/AccountInfo/";

    QDir dir;

    if(dir.mkpath(accountInfo))
        qDebug("Folder created");
    if(dir.exists(accountInfo))
        qDebug("Folder exists");

    //Чтение данных из файла//

    accountInfo = "D:/qt/CourseWork/source/Gallery/AccountInfo/admin.txt";

    QFile admin(accountInfo);

    if(admin.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!admin.atEnd())
            {
                //читаем строку
                QString str = admin.readLine();
                str.remove(str.length()-1, str.length());
                //Делим строку на слова разделенные пробелом
                adminInfo.push_back(str);
            }

        }
        else
        {
            qDebug()<< "can't open the file";
        }
    qDebug()<<adminInfo[0]<<" "<<adminInfo[1];
    login = adminInfo[0];
    password = adminInfo[1];

    /*///////////////////////////////////////////////////////*/
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


