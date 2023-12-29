#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QMainWindow>
#include "splashscreen.h"
namespace Ui {
class StartScreen;
}

class StartScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);
    ~StartScreen();

private slots:
    void on_pushButton_Start_clicked();

private:
    Ui::StartScreen *ui;
};

#endif // STARTSCREEN_H
