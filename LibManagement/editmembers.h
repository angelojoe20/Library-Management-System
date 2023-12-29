#ifndef EDITMEMBERS_H
#define EDITMEMBERS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class editmembers;
}

class editmembers : public QDialog
{
    Q_OBJECT

public:
    explicit editmembers(QWidget *parent = nullptr);
    ~editmembers();

    void setMemberDetails(int id, const QString& firstName, const QString& lastName, int contactNumber,
                          const QString& email, const QString& gender);

private slots:


    void on_searchButton_clicked();

    void on_closeButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::editmembers *ui;
    QSqlDatabase database;

    // Member variables to hold book details
    int m_id;
    QString m_firstName;
    QString m_lastName;
    int m_contactNumber;
    QString m_email;
    QString m_gender;

};

#endif // EDITMEMBERS_H
