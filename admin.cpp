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
    QString oldLogin = "";
    QString oldPassword = "";

    QString newLogin = "";
    QString newPassword = "";

    QString accountInfo = "D:/qt/CourseWork/source/Gallery/AccountInfo/admin.txt";

    QFile admin(accountInfo);

    if(admin.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!admin.atEnd())
            {
                //читаем строку
                QString str = admin.readLine();
                str.remove(str.length()-1, str.length());
                //Делим строку на слова разделенные пробелом
                oldAcc.push_back(str);
            }
            admin.close();
        }
        else
        {
            qDebug()<< "can't open the file";
        }
    oldLogin = oldAcc[0];
    oldPassword = oldAcc[1];

    QString checkOldLogin = ui->oldLoginEdit->text();
    QString checkOldPassword = ui->oldPasswordEdit->text();

    QTextStream ts(&admin);

    if (oldLogin != checkOldLogin || oldPassword != checkOldPassword)
    {
        ui->CheckLabel->setVisible(true);
    }else
    {
        if (admin.open(QIODevice::WriteOnly))
        {
            newLogin = ui->newLoginEdit->text() + "\n";
            newPassword = ui->newPasswordEdit->text() + "\n";
            ts<<newLogin<<newPassword;
            admin.close();
        }
        this->close();
    }

}
