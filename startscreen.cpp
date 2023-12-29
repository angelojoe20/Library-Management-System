#include "startscreen.h"
#include "ui_startscreen.h"

StartScreen::StartScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
}

StartScreen::~StartScreen()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StartScreen::on_pushButton_Start_clicked()
{
    this->hide();
    SplashScreen *LoadingScreen = new SplashScreen();
    LoadingScreen->show();

    //LoadingScreen->showMaximized();
    qApp->processEvents(QEventLoop::AllEvents);
    LoadingScreen->Start_Loading();
}

