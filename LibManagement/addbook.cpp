#include "addbook.h"
#include "ui_addbook.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QByteArray>

addBook::addBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addBook)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
}

addBook::~addBook()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void addBook::on_addBookButton_clicked()
{
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
    if (id.isEmpty() || bookName.isEmpty() || author.isEmpty() || genre.isEmpty() || quantity <= 0 || publisher.isEmpty())
    {
        QMessageBox::information(this, "Error", "Please fill in all the required fields.");
        return;
    }

    // Convert cover image to QByteArray
    QPixmap coverImage = ui->coverLabel->pixmap().scaled(150, 200);
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    coverImage.save(&buffer, "PNG"); // Change format if needed

    // Save data to database
    QSqlQuery query;
    query.prepare("INSERT INTO books (id, bookName, author, genre, quantity, publisher, publishedDate, description, coverImage) "
                  "VALUES (:id, :bookName, :author, :genre, :quantity, :publisher, :publishedDate, :description, :coverImage)");
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
        QMessageBox::critical(this, "Error", "Failed to add book to the database.");
        return;
    }

    // Clear input fields
    ui->idLineEdit->clear();
    ui->bookNameLineEdit->clear();
    ui->authorLineEdit->clear();
    ui->genreLineEdit->clear();
    ui->quantitySpinBox->setValue(0);
    ui->publisherLineEdit->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->descriptionTextEdit->clear();
    ui->coverLabel->clear();

    QMessageBox::information(this, "Success", "Book added successfully.");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void addBook::on_uploadCoverButton_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (imagePath.isEmpty())
        return;

    QPixmap image(imagePath);
    if (image.isNull())
    {
        QMessageBox::information(this, "Error", "Failed to load image.");
        return;
    }

    ui->coverLabel->setPixmap(image.scaled(150, 200, Qt::KeepAspectRatio));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void addBook::on_closeButton_clicked()
{
    close();
}

