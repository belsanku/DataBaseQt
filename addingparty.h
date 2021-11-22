#ifndef ADDINGPARTY_H
#define ADDINGPARTY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QSqlError>

namespace Ui {
class AddingParty;
}

class AddingParty : public QWidget
{
    Q_OBJECT

public:
    explicit AddingParty(QWidget *parent = 0);
    ~AddingParty();

private slots:
    void on_declineParty_clicked();

    void on_acceptParty_clicked();

private:
    Ui::AddingParty *ui;
    QSqlDatabase partyDB;
    QSqlTableModel *partyModel;
    QSqlQuery *partyQuery;
};

#endif // ADDINGPARTY_H
