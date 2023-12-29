#include "editbook.h"
#include "ui_editbook.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QByteArray>
#include <QSqlQuery>
#include <QBuffer>

editbook::editbook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editbook)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);

    // Connect button signals to slots
    connect(ui->uploadCoverButton, &QPushButton::clicked, this, &editbook::on_uploadCoverButton_clicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &editbook::on_saveButton_clicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &editbook::on_searchButton_clicked);
}

editbook::~editbook()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void editbook::setBookDetails(const QString& id, const QString& bookName, const QString& author, const QString& genre,
                              int quantity, const QString& publisher, const QDate& publishedDate,
                              const QString& description, const QPixmap& coverImage)
{
    // Set the book details in the member variables
    m_id = id;
    m_bookName = bookName;
    m_author = author;
    m_genre = genre;
    m_quantity = quantity;
    m_publisher = publisher;
    m_publishedDate = publishedDate;
    m_description = description;
    m_coverImage = coverImage;

    // Update the UI with the book details
    ui->idLineEdit->setText(id);
    ui->bookNameLineEdit->setText(bookName);
    ui->authorLineEdit->setText(author);
    ui->genreLineEdit->setText(genre);
    ui->quantitySpinBox->setValue(quantity);
    ui->publisherLineEdit->setText(publisher);
    ui->dateEdit->setDate(publishedDate);
    ui->descriptionTextEdit->setPlainText(description);
    ui->coverLabel->setPixmap(coverImage.scaled(150, 200, Qt::KeepAspectRatio));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void editbook::on_searchButton_clicked()
{
    // Get the ID to search for
    QString id = ui->idLineEdit->text();

    // Query the database based on the ID
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next())
    {
        // Retrieve the book details from the query result
        QString bookName = query.value("bookName").toString();
        QString author = query.value("author").toString();
        QString genre = query.value("genre").toString();
        int quantity = query.value("quantity").toInt();
        QString publisher = query.value("publisher").toString();
        QDate publishedDate = query.value("publishedDate").toDate();
        QString description = query.value("description").toString();
        QByteArray coverData = query.value("coverImage").toByteArray();
        QPixmap coverImage;
        coverImage.loadFromData(coverData);

        // Set the retrieved book details in the UI
        setBookDetails(id, bookName, author, genre, quantity, publisher, publishedDate, description, coverImage);
    }
    else
    {
        // Book not found
        QMessageBox::information(this, "Error", "Book not found.");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void editbook::on_saveButton_clicked()
{
    // Disconnect the slot to avoid multiple calls
    disconnect(ui->saveButton, &QPushButton::clicked, this, &editbook::on_saveButton_clicked);

    // Get input values
    QString id = ui->idLineEdit->text();
    QString bookName = ui->bookNameLineEdit->text();
    QString author = ui->authorLineEdit->text();
    QString genre = ui->genreLineEdit->text();
    int quantity = ui->quantitySpinBox->value();
    QString publisher = ui->publisherLineEdit->text();
    QDate publishedDate = ui->dateEdit->date();
    QString description = ui->descriptionTextEdit->toPlainText();

    // Validate input
    bool hasError = false;  // Track if there is an error

    if (id.isEmpty() || bookName.isEmpty() || author.isEmpty() || genre.isEmpty() || quantity <= 0 || publisher.isEmpty())
    {
        QMessageBox::information(this, "Error", "Please fill in all the required fields.");
        hasError = true;
    }

    // Check if there is an error
    if (hasError)
    {
        return;  // Exit the function without further processing
    }

    // Convert cover image to QByteArray
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    m_coverImage.save(&buffer, "PNG"); // Change format if needed

    // Update data in the database
    QSqlQuery query;
    query.prepare("UPDATE books SET bookName = :bookName, author = :author, genre = :genre, "
                  "quantity = :quantity, publisher = :publisher, publishedDate = :publishedDate, "
                  "description = :description, coverImage = :coverImage WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":bookName", bookName);
    query.bindValue(":author", author);
    query.bindValue(":genre", genre);
    query.bindValue(":quantity", quantity);
    query.bindValue(":publisher", publisher);
    query.bindValue(":publishedDate", publishedDate.toString("yyyy-MM-dd"));
    query.bindValue(":description", description);
    query.bindValue(":coverImage", imageData);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Failed to update book in the database.");
        return;
    }

    // Display success message
    QMessageBox::information(this, "Success", "Book updated successfully.");

    // Clear the fields after saving
    ui->idLineEdit->clear();
    ui->bookNameLineEdit->clear();
    ui->authorLineEdit->clear();
    ui->genreLineEdit->clear();
    ui->quantitySpinBox->setValue(0);
    ui->publisherLineEdit->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->descriptionTextEdit->clear();
    ui->coverLabel->clear();

    // Reconnect the slot
    connect(ui->saveButton, &QPushButton::clicked, this, &editbook::on_saveButton_clicked);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void editbook::on_uploadCoverButton_clicked()
{
    // Disconnect the slot to avoid multiple calls
    disconnect(ui->uploadCoverButton, &QPushButton::clicked, this, &editbook::on_uploadCoverButton_clicked);

    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (imagePath.isEmpty())
        return;

    QPixmap image(imagePath);
    if (image.isNull())
    {
        QMessageBox::information(this, "Error", "Failed to load image.");
        return;
    }

    m_coverImage = image;

    ui->coverLabel->setPixmap(m_coverImage.scaled(150, 200, Qt::KeepAspectRatio));

    // Reconnect the slot
    connect(ui->uploadCoverButton, &QPushButton::clicked, this, &editbook::on_uploadCoverButton_clicked);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void editbook::on_closeButton_clicked()
{
    close();
}

