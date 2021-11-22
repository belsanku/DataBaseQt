#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();
    bool start;
    QString login;
    QString password;

private slots:
    void CheckAccount();
    void ExitApplication();


public slots:

private:
    Ui::LoginForm *ui;
};

#endif // LOGINFORM_H
