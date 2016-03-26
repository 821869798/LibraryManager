#ifndef BOOKSEARCHFORM_H
#define BOOKSEARCHFORM_H

#include <QWidget>

namespace Ui {
class BookSearchForm;
}

class BookSearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookSearchForm(QWidget *parent = 0);
    ~BookSearchForm();

private:
    Ui::BookSearchForm *ui;
};

#endif // BOOKSEARCHFORM_H
