#include "mainwindow.h"
#include "loginform.h"
#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <QLibraryInfo>

void initializeTranslator(QString translation, QString directory = QString(""));

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/source/style.qss");
    initializeTranslator(QLatin1String("qt_") + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
    LoginForm log;
    log.start = false;
    log.show();

    return a.exec();
}


void initializeTranslator(QString translation, QString directory) {
    QTranslator *translator = new QTranslator(qApp);
    bool isSuccess = false;

    if (directory.isEmpty()) {
        isSuccess = translator->load(translation);
    }
    else {
        isSuccess = translator->load(translation, directory);
    }

    if (isSuccess) {
        qApp->installTranslator(translator);
    }
    else {
        QMessageBox::warning(0, "Error when loading translation", "There was an error loading the default translation\"" + QLocale::system().name() + "\"");
    }
}
