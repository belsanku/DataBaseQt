#ifndef AUTHOR_H
#define AUTHOR_H

#include <QWidget>

namespace Ui {
class Author;
}

class Author : public QWidget
{
    Q_OBJECT

public:
    explicit Author(QWidget *parent = 0);
    ~Author();

private slots:
    void on_UnderstandButton_clicked();

private:
    Ui::Author *ui;
};

#endif // AUTHOR_H
