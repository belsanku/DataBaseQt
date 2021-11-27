#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include <QString>

namespace Ui {
class admin;
}

class admin : public QWidget
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = 0);
    ~admin();

private slots:
    void on_ChangeButton_clicked();

private:
    Ui::admin *ui;
    QVector<QString>oldAcc;
};

#endif // ADMIN_H
