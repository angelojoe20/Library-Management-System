#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include "loginscreen.h"
#include "ui_loginscreen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SplashScreen; }
QT_END_NAMESPACE

class SplashScreen : public QMainWindow

{
    Q_OBJECT

public:
    SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();

    void Start_Loading();
    void Open_New_Window();

private:
    Ui::SplashScreen *ui;
};

#endif // SPLASHSCREEN_H
