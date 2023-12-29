#ifndef BORROWBOOK_H
#define BORROWBOOK_H

#include <QDialog>
#include <QSqlDatabase>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
class borrowbook;
}

class borrowbook : public QDialog
{
    Q_OBJECT

public:
    explicit borrowbook(QWidget *parent = nullptr);
    ~borrowbook();

private slots:
    void searchBook();
    void searchMember();
    void issueBook();

    void on_closeButton_clicked();

private:
    Ui::borrowbook *ui;
    QSqlDatabase database;

    QVBoxLayout* descriptionLayout;
    QTextEdit* descriptionTextEdit;

    QTableView *tableView;
    QStandardItemModel *tableModel;

    void populateIssuanceData();

    void deleteIssuance();

    void openDatabase();
};

#endif // BORROWBOOK_H
