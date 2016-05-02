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

    void on_btn_new_clicked();

    void on_btn_edit_clicked();

    void on_btn_del_clicked();

    void on_btn_avaliableYes_clicked();

    void on_btn_avaliableNo_clicked();

    void on_btn_clearArrears_clicked();

private:
    Ui::ReaderManageForm *ui;

    bool firstShow;
    QString getData;
    void init();
    void initByData(QString tempData);
    QNetworkAccessManager * netManager;
    int searchType;
    QButtonGroup *radioGroup;
    void changeReaderOther(QString json_str);
};

#endif // READERMANAGEFORM_H
