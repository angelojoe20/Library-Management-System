#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QDialog>
#include <QSqlQuery>
#include <QBuffer>
#include <QDate>

namespace Ui {
class addBook;
}

class addBook : public QDialog
{
    Q_OBJECT

public:
    explicit addBook(QWidget *parent = nullptr);
    ~addBook();

private slots:


    void on_addBookButton_clicked();

    void on_uploadCoverButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::addBook *ui;
};

#endif // ADDBOOK_H
