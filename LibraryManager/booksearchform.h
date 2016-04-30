#ifndef BOOKSEARCHFORM_H
#define BOOKSEARCHFORM_H

#include <QWidget>
#include <QButtonGroup>
#include "tool.h"

namespace Ui {
class BookSearchForm;
}

class BookSearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookSearchForm(int borrow = 0,QWidget *parent = 0);
    ~BookSearchForm();
    void show();
private slots:
    void on_btn_jump_clicked();

    void finishHttp(QNetworkReply *reply);

    void on_btn_query_clicked();

    void on_btn_showall_clicked();

    void on_btn_showinfo_clicked();

private:
    Ui::BookSearchForm *ui;
    void init();
    QString getData;
    void initByData(QString tempData);
    QNetworkAccessManager * netManager;
    int searchType;
    QButtonGroup *radioGroup;
    bool firstShow;
    int borrow; //借书权限
};

#endif // BOOKSEARCHFORM_H
