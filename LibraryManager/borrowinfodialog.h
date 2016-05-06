#ifndef BORROWINFODIALOG_H
#define BORROWINFODIALOG_H

#include <QDialog>

namespace Ui {
class BorrowInfoDialog;
}

class BorrowInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowInfoDialog(QWidget *parent = 0);
    ~BorrowInfoDialog();

private:
    Ui::BorrowInfoDialog *ui;
};

#endif // BORROWINFODIALOG_H
