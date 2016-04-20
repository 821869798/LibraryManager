#ifndef READERMANAGEFORM_H
#define READERMANAGEFORM_H

#include <QWidget>
#include "tool.h"
#include <QButtonGroup>

namespace Ui {
class ReaderManageForm;
}

class ReaderManageForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderManageForm(QWidget *parent = 0);
    ~ReaderManageForm();

    void show();
private slots:
    void on_btn_jump_clicked();

    void finishHttp(QNetworkReply *reply);
    void on_btn_query_clicked();

    void on_btn_showall_clicked();

private:
    Ui::ReaderManageForm *ui;
    QString getData;
    void init();
    void initByData(QString tempData);
    QNetworkAccessManager * netManager;
    int searchType;
    QButtonGroup *radioGroup;
};

#endif // READERMANAGEFORM_H
