#ifndef BOOKSLIST_H
#define BOOKSLIST_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QModelIndex>

namespace Ui {
class bookslist;
}

class bookslist : public QDialog
{
    Q_OBJECT

public:
    explicit bookslist(QWidget *parent = nullptr);
    ~bookslist();

private slots:
    void deleteBook();
    void loadBooks();
    void searchBooks(const QString& searchText);
    void on_booksTable_clicked(const QModelIndex& index);
    void on_closeButton_clicked();
    void sortBooks();

private:
    Ui::bookslist *ui;
    QSqlQueryModel *model;
};

#endif // BOOKSLIST_H
