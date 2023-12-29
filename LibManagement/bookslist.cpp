#include "bookslist.h"
#include "ui_bookslist.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableWidgetItem>
#include <QTableView>
#include <QDate>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>
#include <QPixmap>
#include <QByteArray>
#include <QComboBox>

bookslist::bookslist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookslist)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);

    // Connect the search button signal to the searchBooks slot
    QObject::connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &bookslist::searchBooks);

    // Connect the delete button signal to the deleteBook slot
    QObject::connect(ui->deleteButton, &QPushButton::clicked, this, &bookslist::deleteBook);

    // Connect the sort combo box signal to the sortBooks slot
    QObject::connect(ui->sortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &bookslist::sortBooks);

    // Load and display the list of books
    loadBooks();
}

bookslist::~bookslist()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void bookslist::deleteBook()
{
    // Get the selected book's ID
    QModelIndex index = ui->booksTable->currentIndex();
    QString bookId = ui->booksTable->model()->data(ui->booksTable->model()->index(index.row(), 0)).toString();

    // Delete the book from the database
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE id = :bookId");
    query.bindValue(":bookId", bookId);

    // Check if the query executed successfully
    if (!query.exec())
    {
        qDebug() << "Failed to delete book from the database.";
        return;
    }

    // Show a prompt for successful deletion
    QMessageBox::information(this, "Success", "Book deleted successfully.");

    // Clear the UI
    ui->idLabel->clear();
    ui->bookNameLabel->clear();
    ui->authorLabel->clear();
    ui->genreLabel->clear();
    ui->quantityLabel->clear();
    ui->publisherLabel->clear();
    ui->publishedDateLabel->clear();
    ui->descriptionLabel->clear();
    ui->coverLabel->clear();
    ui->searchLineEdit->clear();

    // Reload and display the updated list of books
    loadBooks();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void bookslist::loadBooks()
{
    // Retrieve the list of books from the database
    QSqlQuery query("SELECT id, bookName, author, genre, publisher FROM books");

    // Check if the query executed successfully
    if (!query.exec())
    {
        qDebug() << "Failed to retrieve books from the database.";
        return;
    }

    // Create a model for the table view
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(5);  // Set the number of columns to 5
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Book Name");
    model->setHeaderData(2, Qt::Horizontal, "Author");
    model->setHeaderData(3, Qt::Horizontal, "Genre");
    model->setHeaderData(4, Qt::Horizontal, "Publisher");

    // Iterate over the query result
    while (query.next())
    {
        // Retrieve the book information
        QString id = query.value("id").toString();
        QString bookName = query.value("bookName").toString();
        QString author = query.value("author").toString();
        QString genre = query.value("genre").toString();
        QString publisher = query.value("publisher").toString();

        // Add the book information to the model
        QList<QStandardItem*> rowData;
        rowData.append(new QStandardItem(id));
        rowData.append(new QStandardItem(bookName));
        rowData.append(new QStandardItem(author));
        rowData.append(new QStandardItem(genre));
        rowData.append(new QStandardItem(publisher));
        model->appendRow(rowData);
    }

    // Set the model for the table view
    ui->booksTable->setModel(model);
    ui->booksTable->resizeColumnsToContents();

    // Make the table view non-editable
    ui->booksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void bookslist::searchBooks(const QString& searchText)
{
    // Retrieve the list of books matching the search text from the database
    QSqlQuery query;
    query.prepare("SELECT id, bookName, author, genre, publisher FROM books WHERE id LIKE :searchText OR bookName LIKE :searchText OR author LIKE :searchText OR genre LIKE :searchText OR publisher LIKE :searchText");
    query.bindValue(":searchText", "%" + searchText + "%");

    // Check if the query executed successfully
    if (!query.exec())
    {
        qDebug() << "Failed to search books from the database.";
        return;
    }

    // Create a model for the table view
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(5);  // Set the number of columns to 4
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Book Name");
    model->setHeaderData(2, Qt::Horizontal, "Author");
    model->setHeaderData(3, Qt::Horizontal, "Genre");
    model->setHeaderData(4, Qt::Horizontal, "Publisher");

    // Iterate over the query result
    while (query.next())
    {
        // Retrieve the book information
        QString id = query.value("id").toString();
        QString bookName = query.value("bookName").toString();
        QString author = query.value("author").toString();
        QString genre = query.value("genre").toString();
        QString publisher = query.value("publisher").toString();

        // Add the book information to the model
        QList<QStandardItem*> rowData;
        rowData.append(new QStandardItem(id));
        rowData.append(new QStandardItem(bookName));
        rowData.append(new QStandardItem(author));
        rowData.append(new QStandardItem(genre));
        rowData.append(new QStandardItem(publisher));
        model->appendRow(rowData);
    }

    // Set the model for the table view
    ui->booksTable->setModel(model);
    ui->booksTable->resizeColumnsToContents();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void bookslist::sortBooks()
{
    // Get the selected sorting option from the combo box
    QString sortingOption = ui->sortComboBox->currentText();

    // Sort the table based on the selected option
    if (sortingOption == "Genre")
    {
        ui->booksTable->sortByColumn(3, Qt::AscendingOrder);
    }
    else if (sortingOption == "Publisher")
    {
        ui->booksTable->sortByColumn(2, Qt::AscendingOrder);
    }
    else if (sortingOption == "Author")
    {
        ui->booksTable->sortByColumn(2, Qt::AscendingOrder); // Sort by author column (index 2)
    }
    else if (sortingOption == "ID")
    {
        ui->booksTable->sortByColumn(0, Qt::AscendingOrder);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void bookslist::on_booksTable_clicked(const QModelIndex& index)
{
    // Get the selected book's ID
    QModelIndex idIndex = ui->booksTable->model()->index(index.row(), 0);
    QString bookId = ui->booksTable->model()->data(idIndex).toString();

    // Retrieve the book details from the database
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE id = :bookId");
    query.bindValue(":bookId", bookId);

    // Check if the query executed successfully
    if (!query.exec() || !query.first())
    {
        qDebug() << "Failed to retrieve book details from the database.";
        return;
    }

    // Retrieve the book information
    QString id = query.value("id").toString();
    QString bookName = query.value("bookName").toString();
    QString author = query.value("author").toString();
    QString genre = query.value("genre").toString();
    int quantity = query.value("quantity").toInt();
    QString publisher = query.value("publisher").toString();
    QDate publishedDate = query.value("publishedDate").toDate();
    QString description = query.value("description").toString();
    QByteArray imageData = query.value("coverImage").toByteArray();

    // Show the book details in the UI
    ui->idLabel->setText(id);
    ui->bookNameLabel->setText(bookName);
    ui->authorLabel->setText(author);
    ui->genreLabel->setText(genre);
    ui->quantityLabel->setText(QString::number(quantity));
    ui->publisherLabel->setText(publisher);
    ui->publishedDateLabel->setText(publishedDate.toString("yyyy-MM-dd"));
    ui->descriptionLabel->setText(description);

    // Display the book cover image
    QPixmap coverImage;
    coverImage.loadFromData(imageData);
    ui->coverLabel->setPixmap(coverImage.scaled(150, 200));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void bookslist::on_closeButton_clicked()
{
    close();
}

