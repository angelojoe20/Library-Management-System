#ifndef EDITBOOK_H
#define EDITBOOK_H

#include <QDialog>
#include <QPixmap>
#include <QDate>

namespace Ui {
class editbook;
}

class editbook : public QDialog
{
    Q_OBJECT

public:
    explicit editbook(QWidget *parent = nullptr);
    ~editbook();

    void setBookDetails(const QString& id, const QString& bookName, const QString& author, const QString& genre,
                        int quantity, const QString& publisher, const QDate& publishedDate,
                        const QString& description, const QPixmap& coverImage);

private slots:


    void on_searchButton_clicked();

    void on_saveButton_clicked();

    void on_uploadCoverButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::editbook *ui;

    // Member variables to hold book details
    QString m_id;
    QString m_bookName;
    QString m_author;
    QString m_genre;
    int m_quantity;
    QString m_publisher;
    QDate m_publishedDate;
    QString m_description;
    QPixmap m_coverImage;
};

#endif // EDITBOOK_H
