#include "borrowinfodialog.h"
#include "ui_borrowinfodialog.h"

BorrowInfoDialog::BorrowInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowInfoDialog)
{
    ui->setupUi(this);
}

BorrowInfoDialog::~BorrowInfoDialog()
{
    delete ui;
}
