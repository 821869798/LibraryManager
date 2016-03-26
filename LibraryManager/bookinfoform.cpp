#include "bookinfoform.h"
#include "ui_bookinfoform.h"

BookInfoForm::BookInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookInfoForm)
{
    ui->setupUi(this);
}

BookInfoForm::~BookInfoForm()
{
    delete ui;
}
