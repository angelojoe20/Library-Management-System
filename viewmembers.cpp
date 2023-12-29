#include "viewmembers.h"
#include "ui_viewmembers.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QObject>
#include <QString>
#include <QMessageBox>


viewmembers::viewmembers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewmembers)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);

    // Connect the search button signal to the searchMembers slot
    QObject::connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &viewmembers::searchMembers);

    // Connect the delete button signal to the deleteMember slot
    QObject::connect(ui->deleteButton, &QPushButton::clicked, this, &viewmembers::deleteMember);

    // Load and display the list of members
    loadMembers();
}

viewmembers::~viewmembers()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void viewmembers::loadMembers()
{
    // Retrieve the list of members from the database
    QSqlQuery query("SELECT id, firstname, lastname, contactnumber, email, gender FROM members");


    // Check if the query executed successfully
    if (!query.exec())
    {
        qDebug() << "Failed to retrieve members from the database.";
        return;
    }

    // Create a model for the table view
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(6);  // Set the number of columns to 6
    model->setHeaderData(0, Qt::Horizontal, "ID"); // Add the ID header
    model->setHeaderData(1, Qt::Horizontal, "First Name");
    model->setHeaderData(2, Qt::Horizontal, "Last Name");
    model->setHeaderData(3, Qt::Horizontal, "Contact Number");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Gender");


    // Iterate over the query result
    while (query.next())
    {
        // Retrieve the member information
        int id = query.value("id").toInt();
        QString firstName = query.value("firstname").toString();
        QString lastName = query.value("lastname").toString();
        QString contactNumber = query.value("contactnumber").toString();
        QString email = query.value("email").toString();
        QString gender = query.value("gender").toString();

        // Add the member information to the model
        QList<QStandardItem*> rowData;
        rowData.append(new QStandardItem(QString::number(id))); // Convert the ID to QString
        rowData.append(new QStandardItem(firstName));
        rowData.append(new QStandardItem(lastName));
        rowData.append(new QStandardItem(contactNumber));
        rowData.append(new QStandardItem(email));
        rowData.append(new QStandardItem(gender));
        model->appendRow(rowData);

    }

    // Set the model for the table view
    ui->membersTable->setModel(model);
    ui->membersTable->resizeColumnsToContents();

    // Make the table view non-editable
    ui->membersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void viewmembers::searchMembers(const QString& searchText)
{
    // Retrieve the list of members matching the search text from the database
    QSqlQuery query;
    query.prepare("SELECT id, firstname, lastname, contactnumber, email, gender FROM members WHERE id LIKE :searchText OR firstname LIKE :searchText OR lastname LIKE :searchText OR email LIKE :searchText");
    query.bindValue(":searchText", "%" + searchText + "%");

    // Check if the query executed successfully
    if (!query.exec())
    {
        qDebug() << "Failed to search members from the database.";
        return;
    }

    // Create a model for the table view
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(6);  // Set the number of columns to 6
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "First Name");
    model->setHeaderData(2, Qt::Horizontal, "Last Name");
    model->setHeaderData(3, Qt::Horizontal, "Contact Number");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Gender");

    // Iterate over the query result
    while (query.next())
    {
        // Retrieve the member information
        QString id = query.value("id").toString();
        QString firstName = query.value("firstname").toString();
        QString lastName = query.value("lastname").toString();
        QString contactNumber = query.value("contactnumber").toString();
        QString email = query.value("email").toString();
        QString gender = query.value("gender").toString();

        // Add the member information to the model
        QList<QStandardItem*> rowData;
        rowData.append(new QStandardItem(id));
        rowData.append(new QStandardItem(firstName));
        rowData.append(new QStandardItem(lastName));
        rowData.append(new QStandardItem(contactNumber));
        rowData.append(new QStandardItem(email));
        rowData.append(new QStandardItem(gender));
        model->appendRow(rowData);
    }

    // Set the model for the table view
    ui->membersTable->setModel(model);
    ui->membersTable->resizeColumnsToContents();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void viewmembers::deleteMember()
{
    // Get the selected member's information from the table view
    QItemSelectionModel* selectionModel = ui->membersTable->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    // Check if a member is selected
    if (selectedRows.isEmpty())
    {
        QMessageBox::information(this, "Delete Member", "Please select a member to delete.");
        return;
    }

    // Confirm the deletion with the user
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Delete Member", "Are you sure you want to delete the selected member?", QMessageBox::Yes | QMessageBox::No);

    if (confirmation == QMessageBox::No)
        return;

    // Retrieve the member's information from the selected row
    QModelIndex selectedRow = selectedRows.at(0);
    QString firstName = ui->membersTable->model()->index(selectedRow.row(), 1).data().toString();
    QString lastName = ui->membersTable->model()->index(selectedRow.row(), 2).data().toString();

    // Delete the member from the database
    QSqlQuery query;
    query.prepare("DELETE FROM members WHERE firstname = :firstName AND lastname = :lastName");
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);

    // Check if the query executed successfully
    if (!query.exec())
    {
        qDebug() << "Failed to delete the member from the database.";
        QMessageBox::critical(this, "Delete Member", "Failed to delete the member from the database.");
        return;
    }

    // Refresh the members list
    loadMembers();

    // Display a success message
    QMessageBox::information(this, "Delete Member", "Member deleted successfully.");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void viewmembers::on_closeButton_clicked()
{
    close();
}
