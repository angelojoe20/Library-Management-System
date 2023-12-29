#include "loginscreen.h"
#include "ui_loginscreen.h"
#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QLineEdit>
#include <QMessageBox>

loginscreen::loginscreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginscreen)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);

    // Create a connection to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Ruone/Documents/SCHOOL/2ND YEAR - 2ND SEM/SD/- FINAL ACTIVITIES -/LibManagement/LibManagement/library.db");

    // Open the database
    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
    }

    // Check if the "login" table exists
    if (!db.tables().contains("login")) {
        qDebug() << "Error: the 'login' table does not exist in the database.";
    }
}

loginscreen::~loginscreen()
{
    // Close the database connection
    QSqlDatabase::database().close();

    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loginscreen::on_login_button_clicked()
{
    {
        // Get the username and password from the text boxes
        QString username = ui->username->text();
        QString password = ui->password->text();

        // Create a query to check if the username and password are correct
        QSqlQuery query;
        query.prepare("SELECT * FROM login WHERE username = :username AND password = :password");
        query.bindValue(":username", username);
        query.bindValue(":password", password);

        // Execute the query
        if (query.exec() && query.next()) {

            // The username and password are correct
            qDebug() << "Login successful!";
            accept(); // close the login window
            mainMenu *mainmenu = new mainMenu;
            mainmenu->show(); // open the main window

        } else {

            // The username or password is incorrect
            qDebug() << "Login failed.";
            QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
        }
    }
}



