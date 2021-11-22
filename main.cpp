#include "mainwindow.h"
#include "loginform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/source/style.qss");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
    LoginForm log;
    log.start = false;
    log.show();

    return a.exec();
}
