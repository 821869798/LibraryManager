#ifndef READERMANAGEFORM_H
#define READERMANAGEFORM_H

#include <QWidget>

namespace Ui {
class ReaderManageForm;
}

class ReaderManageForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderManageForm(QWidget *parent = 0);
    ~ReaderManageForm();

private:
    Ui::ReaderManageForm *ui;
};

#endif // READERMANAGEFORM_H
