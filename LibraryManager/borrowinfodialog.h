#ifndef BORROWINFODIALOG_H
#define BORROWINFODIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "tool.h"

namespace Ui {
class BorrowInfoDialog;
}

class BorrowInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowInfoDialog(QString barcode,QString bname,QString author,QWidget *parent = 0);
    ~BorrowInfoDialog();

private:
    Ui::BorrowInfoDialog *ui;

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnMenu_Close_clicked();

    void on_btnMenu_Max_clicked();

    void on_btnMenu_Min_clicked();

    void changeSkin();

    void finishHttp(QNetworkReply *reply);
    void on_btn_front_clicked();

    void on_btn_next_clicked();

    void on_btn_getHistory_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();

    void init(QString barcode,QString bname,QString author);
    QString currentId;
    QNetworkAccessManager * netManager;
    void showAllBorrow();
    void showBorrowHistory(int page);
};

#endif // BORROWINFODIALOG_H
