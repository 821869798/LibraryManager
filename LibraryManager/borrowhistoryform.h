#ifndef BORROWHISTORYFORM_H
#define BORROWHISTORYFORM_H

#include <QWidget>
#include "tool.h"

namespace Ui {
class BorrowHistoryForm;
}

class BorrowHistoryForm : public QWidget
{
    Q_OBJECT

public:
    explicit BorrowHistoryForm(QWidget *parent = 0);
    ~BorrowHistoryForm();
    void show();
private slots:
    void on_comboBox_currentIndexChanged(int index);

    void finishHttp(QNetworkReply *reply);
    void on_btn_renew_one_clicked();

    void on_btn_renew_all_clicked();
    void on_btn_getHistory_clicked();

private:
    Ui::BorrowHistoryForm *ui;

    bool firstShow;
    QNetworkAccessManager * netManager;
    void init();
    void showAllBorrow();
    void showBorrowHistory(int page=1);
};

#endif // BORROWHISTORYFORM_H
