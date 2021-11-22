#include "author.h"
#include "ui_author.h"

Author::Author(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Author)
{
    ui->setupUi(this);
}

Author::~Author()
{
    delete ui;
}

void Author::on_UnderstandButton_clicked()
{
    this->close();
}
