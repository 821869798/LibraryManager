#include "booksearchform.h"
#include "ui_booksearchform.h"

BookSearchForm::BookSearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookSearchForm)
{
    ui->setupUi(this);
}

BookSearchForm::~BookSearchForm()
{
    delete ui;
}

