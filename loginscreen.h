#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QDialog>

namespace Ui {
class loginscreen;
}

class loginscreen : public QDialog
{
    Q_OBJECT

public:
    explicit loginscreen(QWidget *parent = nullptr);
    ~loginscreen();

private slots:
    void on_pushButton_clicked();

    void on_login_button_clicked();

    void on_adduser_button_clicked();

private:
    Ui::loginscreen *ui;
};

#endif // LOGINSCREEN_H
