#ifndef VIEWMEMBERS_H
#define VIEWMEMBERS_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class viewmembers;
}

class viewmembers : public QDialog
{
    Q_OBJECT

public:
    explicit viewmembers(QWidget *parent = nullptr);
    ~viewmembers();

private slots:
    void loadMembers();
    void searchMembers(const QString& searchText);
    void deleteMember();
    void on_close_clicked(); 
    void on_closeButton_clicked();

private:
    Ui::viewmembers *ui;
    QStandardItemModel* membersModel;

};

#endif // VIEWMEMBERS_H
