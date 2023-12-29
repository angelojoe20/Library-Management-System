#include "splashscreen.h"
#include "ui_splashscreen.h"
#include "loginscreen.h"
#include "ui_loginscreen.h"


SplashScreen::SplashScreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setMaximumWidth(900);
    setMaximumHeight(700);

    ui->progressBar->setMinimum(1);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setRange(1,100);
}

SplashScreen::~SplashScreen()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SplashScreen::Start_Loading()
{
    for (int value = ui->progressBar->minimum(); value <= ui->progressBar->maximum(); value++){
        QThread::msleep(30);
        ui->progressBar->setValue(value);
        ui->label_value->setText(QString::number(value));
        qApp->processEvents(QEventLoop::AllEvents);

        if (value == ui->progressBar->maximum()) {
        QTimer::singleShot(1000, this, &SplashScreen::Open_New_Window);

        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SplashScreen::Open_New_Window()
{
    // Create and show the new window
    loginscreen *loginScreen = new loginscreen;
    loginScreen->show();

    // Close the splash screen
    close();
}




