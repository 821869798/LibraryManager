#include "readermanageform.h"
#include "ui_readermanageform.h"

ReaderManageForm::ReaderManageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderManageForm)
{
    ui->setupUi(this);
}

ReaderManageForm::~ReaderManageForm()
{
    delete ui;
}
