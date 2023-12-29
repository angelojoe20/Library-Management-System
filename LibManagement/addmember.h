#ifndef ADDMEMBER_H
#define ADDMEMBER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class addmember;
}

class addmember : public QDialog
{
    Q_OBJECT

public:
    explicit addmember(QWidget *parent = nullptr);
    ~addmember();

private slots:
    void on_closeButton_clicked();

    void on_addMemberButton_clicked();

private:
    Ui::addmember *ui;

    QSqlDatabase db;

    QString memberId; // New member variable

    bool connectToDatabase();


};


#endif // ADDMEMBER_H
