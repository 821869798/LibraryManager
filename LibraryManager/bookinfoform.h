#ifndef BOOKINFOFORM_H
#define BOOKINFOFORM_H

#include <QWidget>

namespace Ui {
class BookInfoForm;
}

class BookInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookInfoForm(QWidget *parent = 0);
    ~BookInfoForm();

private:
    Ui::BookInfoForm *ui;
};

#endif // BOOKINFOFORM_H
