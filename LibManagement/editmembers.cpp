#include "editmembers.h"
#include "ui_editmembers.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QByteArray>
#include <QSqlQuery>
#include <QBuffer>

editmembers::editmembers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editmembers)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);

    // Add options to the gender combo box
    ui->genderComboBox->addItem("Male");
    ui->genderComboBox->addItem("Female");

    connect(ui->saveButton, &QPushButton::clicked, this, &editmembers::on_saveButton_clicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &editmembers::on_searchButton_clicked);
}

editmembers::~editmembers()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void editmembers::setMemberDetails(int id, const QString& firstName, const QString& lastName, int contactNumber,
                                 const QString& email, const QString& gender)
{
    // Set the book details in the member variables
    m_id = id;
    m_firstName = firstName;
    m_lastName = lastName;
    m_contactNumber = contactNumber;
    m_email = email;
    m_gender = gender;

    // Update the UI with the book details
    ui->memberIdLineEdit->setText(QString::number(id));
    ui->firstNameLineEdit->setText(firstName);
    ui->lastNameLineEdit->setText(lastName);
    ui->contactNumberLineEdit->setText(QString::number(contactNumber));
    ui->emailLineEdit->setText(email);
    ui->genderComboBox->setCurrentText(gender);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void editmembers::on_searchButton_clicked()
{
    // Get the ID to search for
    QString id = ui->memberIdLineEdit->text();

    // Query the database based on the ID
    QSqlQuery query;
    query.prepare("SELECT * FROM members WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next())
    {
        // Retrieve the book details from the query result
        int id = query.value("id").toInt();
        QString firstName = query.value("firstName").toString();
        QString lastName = query.value("lastName").toString();
        int contactNumber = query.value("contactNumber").toInt();
        QString email = query.value("email").toString();
        QString gender = query.value("gender").toString();

        // Clear the gender combo box before setting the retrieved value
        ui->genderComboBox->setCurrentIndex(-1);

        // Set the retrieved book details in the UI
        setMemberDetails(id, firstName, lastName, contactNumber, email, gender);
    }
    else
    {
        // Book not found
        QMessageBox::information(this, "Error", "Book not found.");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void editmembers::on_saveButton_clicked()
{
    // Disconnect the slot to avoid multiple calls
    disconnect(ui->saveButton, &QPushButton::clicked, this, &editmembers::on_saveButton_clicked);

    // Get input values
    QString id = ui->memberIdLineEdit->text();
    QString firstName = ui->firstNameLineEdit->text();
    QString lastName = ui->lastNameLineEdit->text();
    QString contactNumber = ui->contactNumberLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString gender = ui->genderComboBox->currentText();

    // Validate input
    bool hasError = false;  // Track if there is an error

    if (id.isEmpty() || firstName.isEmpty() || lastName.isEmpty() || contactNumber.isEmpty() || email.isEmpty() || gender.isEmpty())
    {
        QMessageBox::information(this, "Error", "Please fill in all the required fields.");
        hasError = true;
    }

    // Check if there is an error
    if (hasError)
    {
        return;  // Exit the function without further processing
    }

    // Update data in the database
    QSqlQuery query;
    query.prepare("UPDATE members SET firstName = :firstName, lastName = :lastName, contactNumber = :contactNumber, "
                  "email = :email, gender = :gender WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":contactNumber", contactNumber);
    query.bindValue(":email", email);
    query.bindValue(":gender", gender);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Failed to update member in the database.");
        return;
    }

    // Display success message
    QMessageBox::information(this, "Success", "Member updated successfully.");

    // Clear the fields after saving
    ui->memberIdLineEdit->clear();
    ui->firstNameLineEdit->clear();
    ui->lastNameLineEdit->clear();
    ui->contactNumberLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->genderComboBox->setCurrentIndex(0);

    // Reconnect the slot
    connect(ui->saveButton, &QPushButton::clicked, this, &editmembers::on_saveButton_clicked);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void editmembers::on_closeButton_clicked()
{
    close();
}

