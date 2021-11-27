#ifndef ADDPARTYSTUDENTS_H
#define ADDPARTYSTUDENTS_H

#include <QWidget>

namespace Ui {
class addPartyStudents;
}

class addPartyStudents : public QWidget
{
    Q_OBJECT

public:
    explicit addPartyStudents(QWidget *parent = 0);
    ~addPartyStudents();

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::addPartyStudents *ui;
};

#endif // ADDPARTYSTUDENTS_H
