#include "addpartystudents.h"
#include "ui_addpartystudents.h"

addPartyStudents::addPartyStudents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addPartyStudents)
{
    ui->setupUi(this);
}

addPartyStudents::~addPartyStudents()
{
    delete ui;
}

void addPartyStudents::on_cancelButton_clicked()
{
    this->close();
}

void addPartyStudents::on_okButton_clicked()
{








    this->close();
}
