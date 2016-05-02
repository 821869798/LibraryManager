#ifndef BORROWDIALOG_H
#define BORROWDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "tool.h"

namespace Ui {
class BorrowDialog;
}

class BorrowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowDialog(int currentType=0,QWidget *parent = 0);
    ~BorrowDialog();

private:
    Ui::BorrowDialog *ui;

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
    void on_btn_add_clicked();

    void on_btn_del_clicked();

    void on_btn_ok_clicked();

    void on_bt_login_clicked();

private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();

    int currentType;
    QNetworkAccessManager * netManager;
    void init();
    bool findTableViewEqual(QString id);
};

#endif // BORROWDIALOG_H
