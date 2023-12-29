#include "borrowbook.h"
#include "ui_borrowbook.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QPixmap>
#include <QSqlError>
#include <QTableView>
#include <QStandardItemModel>

borrowbook::borrowbook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::borrowbook)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    openDatabase();

    // Initialize the table view and model
    tableView = new QTableView(this);
    tableModel = new QStandardItemModel(this);
    tableView->setModel(tableModel);

    // Set the column headers for the table view
    QStringList headers;
    headers << "Book ID" << "Member ID" << "Issuance Date" << "Return Date" << "Notes" << "Status";
    tableModel->setHorizontalHeaderLabels(headers);

    // Add the table view to the layout
    QVBoxLayout *layout = new QVBoxLayout(ui->tableViewContainer);
    layout->addWidget(tableView);
    ui->tableViewContainer->setLayout(layout);

    // Retrieve existing issuance data and populate the table view
    populateIssuanceData();

    // Connect button signals to slots
    connect(ui->searchBookButton, &QPushButton::clicked, this, &borrowbook::searchBook);
    connect(ui->searchMemberButton, &QPushButton::clicked, this, &borrowbook::searchMember);
    connect(ui->issueBookButton, &QPushButton::clicked, this, &borrowbook::issueBook);
    connect(ui->deleteButton, &QPushButton::clicked, this, &borrowbook::deleteIssuance);
}

borrowbook::~borrowbook()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void borrowbook::openDatabase()
{
    // Create a connection to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Ruone/Documents/SCHOOL/2ND YEAR - 2ND SEM/SD/- FINAL ACTIVITIES -/LibManagement/LibManagement/library.db");

    // Open the database
    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return;
    }

    qDebug() << "Database opened successfully.";

    // Check if the "issuance" table already exists
    if (!db.tables().contains("issuance"))
    {
        // Create the "issuance" table
        QSqlQuery createTableQuery;
        createTableQuery.exec("CREATE TABLE issuance ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "bookId INTEGER,"
                              "memberId INTEGER,"
                              "issuanceDate TEXT,"
                              "returnDate TEXT,"
                              "notes TEXT,"
                              "status TEXT)");

        if (createTableQuery.lastError().isValid())
        {
            qDebug() << "Failed to create table:" << createTableQuery.lastError().text();
        }
        else
        {
            qDebug() << "Table created successfully.";
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void borrowbook::searchBook()
{
    QString bookId = ui->bookIdLineEdit->text().trimmed();

    // Check if the book ID is empty
    if (bookId.isEmpty())
    {
        // Clear the book details in the UI
        ui->bookNameLabel->clear();
        ui->authorLabel->clear();
        ui->genreLabel->clear();
        ui->descriptionTextEdit->clear();
        ui->coverImageLabel->clear();
        ui->availabilityLabel->clear();

        // Return from the function without showing the message box
        return;
    }

    // Execute the search query only if the book ID is not empty
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE id = :bookId");
    query.bindValue(":bookId", bookId);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Failed to search for the book: " + query.lastError().text());
        return;
    }

    if (query.next())
    {
        // Retrieve the book details
        QString bookName = query.value("bookName").toString();
        QString author = query.value("author").toString();
        QString genre = query.value("genre").toString();
        QString description = query.value("description").toString();
        QByteArray coverImage = query.value("coverImage").toByteArray();
        int quantity = query.value("quantity").toInt();

        // Display the book details in the UI
        ui->bookNameLabel->setText(bookName);
        ui->authorLabel->setText(author);
        ui->genreLabel->setText(genre);
        ui->descriptionTextEdit->setPlainText(description);

        QPixmap coverPixmap;
        coverPixmap.loadFromData(coverImage);
        ui->coverImageLabel->setPixmap(coverPixmap.scaled(200, 200, Qt::KeepAspectRatio));

        if (quantity > 0)
            ui->availabilityLabel->setText("Available");
        else
            ui->availabilityLabel->setText("Not Available");
    }
    else
    {
        ui->bookNameLabel->clear();
        ui->authorLabel->clear();
        ui->genreLabel->clear();
        ui->descriptionTextEdit->clear();
        ui->coverImageLabel->clear();
        ui->availabilityLabel->clear();

        QMessageBox::information(this, "Not Found", "No book found with the given ID.");
        return; // Add this line to return from the function
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void borrowbook::searchMember()
{
    // Retrieve the member ID from the UI
    QString memberId = ui->memberIdLineEdit->text().trimmed();

    // Validate the member ID
    if (memberId.isEmpty())
    {
        QMessageBox::information(this, "Invalid Input", "Please enter a member ID.");
        return;
    }

    // Construct the SQL query to search for the member
    QString queryStr = "SELECT * FROM members WHERE id = :memberId";
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":memberId", memberId);

    // Execute the SQL query
    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Failed to search for the member: " + query.lastError().text());
        return;
    }

    if (query.next())
    {
        // Retrieve the member details from the query result
        QString firstName = query.value("firstName").toString();
        QString lastName = query.value("lastName").toString();

        // Display the member details in the UI
        ui->firstNameLabel->setText(firstName);
        ui->lastNameLabel->setText(lastName);
    }
    else
    {
        // Clear the member details in the UI
        ui->firstNameLabel->clear();
        ui->lastNameLabel->clear();

        QMessageBox::information(this, "Not Found", "No member found with the given ID.");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void borrowbook::populateIssuanceData()
{
    // Clear the table view model
    tableModel->clear();

    // Add the column headers again after clearing the model
    QStringList headers;
    headers << "Book ID" << "Member ID" << "Issuance Date" << "Return Date" << "Notes" << "Status";
    tableModel->setHorizontalHeaderLabels(headers);

    // Retrieve the issuance data from the database
    QSqlQuery query("SELECT * FROM issuance");
    while (query.next()) {
        QString bookId = query.value("bookId").toString();
        QString memberId = query.value("memberId").toString();
        QString issuanceDate = query.value("issuanceDate").toString();
        QString returnDate = query.value("returnDate").toString();
        QString notes = query.value("notes").toString();
        QString status = query.value("status").toString();

        // Add the data to the table view model
        QList<QStandardItem *> items;
        items.append(new QStandardItem(bookId));
        items.append(new QStandardItem(memberId));
        items.append(new QStandardItem(issuanceDate));
        items.append(new QStandardItem(returnDate));
        items.append(new QStandardItem(notes));
        items.append(new QStandardItem(status));
        tableModel->appendRow(items);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void borrowbook::deleteIssuance()
{
    // Get the selected index from the table view
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedIndexes();

    // Check if any index is selected
    if (selectedIndexes.isEmpty()) {
        QMessageBox::information(this, "No Selection", "Please select an issuance to delete.");
        return;
    }

    // Retrieve the book ID and member ID from the selected row
    int row = selectedIndexes.first().row();
    QString bookId = tableModel->data(tableModel->index(row, 0)).toString();
    QString memberId = tableModel->data(tableModel->index(row, 1)).toString();

    // Confirm the deletion with a message box
    QMessageBox::StandardButton confirmDelete;
    confirmDelete = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete the selected issuance?",
                                          QMessageBox::Yes | QMessageBox::No);

    if (confirmDelete == QMessageBox::No) {
        return;
    }

    // Delete the issuance from the database
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM issuance WHERE bookId = :bookId AND memberId = :memberId");
    deleteQuery.bindValue(":bookId", bookId.toInt());
    deleteQuery.bindValue(":memberId", memberId.toInt());

    if (!deleteQuery.exec()) {
        QMessageBox::critical(this, "Error", "Failed to delete issuance: " + deleteQuery.lastError().text());
        return;
    }

    // Update the book quantity in the "books" table
    QSqlQuery updateQuantityQuery;
    updateQuantityQuery.prepare("UPDATE books SET quantity = quantity + 1 WHERE id = :bookId");
    updateQuantityQuery.bindValue(":bookId", bookId.toInt());

    if (!updateQuantityQuery.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update book quantity: " + updateQuantityQuery.lastError().text());
        return;
    }

    // Remove the selected row from the table view
    tableModel->removeRow(row);

    QMessageBox::information(this, "Success", "Issuance deleted successfully.");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void borrowbook::issueBook()
{
    // Retrieve the book ID and member ID from the UI
    QString bookId = ui->bookIdLineEdit->text().trimmed();
    QString memberId = ui->memberIdLineEdit->text().trimmed();

    // Validate the input fields
    if (bookId.isEmpty() || memberId.isEmpty())
    {
        QMessageBox::information(this, "Invalid Input", "Please enter both the book ID and member ID.");
        return;
    }

    // Check if the book is already issued
    QSqlQuery checkIssuedQuery;
    checkIssuedQuery.prepare("SELECT COUNT(*) FROM issuance WHERE bookId = :bookId AND status = 'Issued'");
    checkIssuedQuery.bindValue(":bookId", bookId.toInt());

    if (!checkIssuedQuery.exec())
    {
        QMessageBox::critical(this, "Error", "Failed to check book issuance status: " + checkIssuedQuery.lastError().text());
        return;
    }

    int issuanceCount = checkIssuedQuery.value(0).toInt();
    if (issuanceCount > 0) {
        QMessageBox::information(this, "Book Already Issued", "The selected book is already issued.");
        return;
    }

    // Retrieve the issuance date and return date from the UI
    QDate issuanceDate = ui->issuanceDateEdit->date();
    QDate returnDate = ui->returnDateEdit->date();

    // Retrieve the issuance notes from the UI
    QString notes = ui->notesTextEdit->toPlainText().trimmed();

    // Perform the book issuance process
    // You can update the necessary tables or perform other operations as required

    // Insert the issuance data
    QString queryStr = "INSERT INTO issuance (bookId, memberId, issuanceDate, returnDate, notes, status) "
                       "VALUES (:bookId, :memberId, :issuanceDate, :returnDate, :notes, 'Issued')"; // Set the initial status as 'Issued'
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":bookId", bookId.toInt());
    query.bindValue(":memberId", memberId.toInt());
    query.bindValue(":issuanceDate", issuanceDate.toString(Qt::ISODate));
    query.bindValue(":returnDate", returnDate.toString(Qt::ISODate));
    query.bindValue(":notes", notes);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Failed to issue the book: " + query.lastError().text());
        qDebug() << "Error issuing book:" << query.lastError().text();
        return;
    }

    // Update the book quantity in the "books" table
    QSqlQuery updateQuantityQuery;
    updateQuantityQuery.prepare("UPDATE books SET quantity = quantity - 1 WHERE id = :bookId");
    updateQuantityQuery.bindValue(":bookId", bookId.toInt());

    if (!updateQuantityQuery.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update book quantity: " + updateQuantityQuery.lastError().text());
        return;
    }

    // Add the issued book information to the table view
    QStringList rowData;
    rowData << bookId << memberId << issuanceDate.toString(Qt::ISODate) << returnDate.toString(Qt::ISODate) << notes << "Issued";

    QList<QStandardItem *> items;
    for (const QString& data : rowData) {
        items.append(new QStandardItem(data));
    }

    tableModel->appendRow(items);

    // Update the book status in the "books" table to "Issued"
    QSqlQuery updateStatusQuery;
    updateStatusQuery.prepare("UPDATE books SET status = 'Issued' WHERE id = :bookId");
    updateStatusQuery.bindValue(":bookId", bookId.toInt());

    if (!updateStatusQuery.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update book status: " + updateStatusQuery.lastError().text());
        return;
    }

    // Update the book availability label
    searchBook();

    // Display a success message
    QMessageBox::information(this, "Success", "Book issued successfully.");

    // Reset the UI fields
    ui->bookIdLineEdit->clear();
    ui->memberIdLineEdit->clear();
    ui->issuanceDateEdit->setDate(QDate::currentDate());
    ui->returnDateEdit->setDate(QDate::currentDate().addDays(7)); // Set a default return date of 7 days from the issuance date
    ui->notesTextEdit->clear();
    ui->bookNameLabel->clear();
    ui->authorLabel->clear();
    ui->genreLabel->clear();
    ui->coverImageLabel->clear();
    ui->availabilityLabel->clear();
    ui->firstNameLabel->clear();
    ui->lastNameLabel->clear();

    // Clear the description label
    ui->descriptionTextEdit->clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void borrowbook::on_closeButton_clicked()
{
    close();
}

