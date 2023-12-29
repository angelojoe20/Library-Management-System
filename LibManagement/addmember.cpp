#include "addmember.h"
#include "ui_addmember.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

addmember::addmember(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addmember)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);

    // Set up the gender combo box
    ui->genderComboBox->addItem("Male");
    ui->genderComboBox->addItem("Female");

    memberId = "";
}

addmember::~addmember()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void addmember::on_closeButton_clicked()
{
    close();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void addmember::on_addMemberButton_clicked()
{
    // Get the member information from the UI
    QString id = ui->idLineEdit->text().trimmed(); // Retrieve the ID from the line edit
    QString firstName = ui->firstNameLineEdit->text().trimmed();
    QString lastName = ui->lastNameLineEdit->text().trimmed();
    QString contactNumber = ui->contactNumberLineEdit->text().trimmed();
    QString email = ui->emailLineEdit->text().trimmed();
    QString gender = ui->genderComboBox->currentText();

    // Validate the member information
    if (id.isEmpty() || firstName.isEmpty() || lastName.isEmpty() || contactNumber.isEmpty() || email.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Please fill in all the fields.");
        return;
    }

    // Insert the member into the database
    QSqlQuery query;
    query.prepare("INSERT INTO members (id, firstName, lastName, contactNumber, email, gender) "
                  "VALUES (:id, :firstName, :lastName, :contactNumber, :email, :gender)");
    query.bindValue(":id", id);
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":contactNumber", contactNumber);
    query.bindValue(":email", email);
    query.bindValue(":gender", gender);

    // Check if the query executed successfully
    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Failed to add member to the database.\n"
                                             "Error details: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "Member added successfully.");

    // Clear the input fields
    ui->idLineEdit->clear();
    ui->firstNameLineEdit->clear();
    ui->lastNameLineEdit->clear();
    ui->contactNumberLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->genderComboBox->setCurrentIndex(0);
}



