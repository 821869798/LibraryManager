#ifndef BORROWCOMPLETEDIALOG_H
#define BORROWCOMPLETEDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "tool.h"


namespace Ui {
class BorrowCompleteDialog;
}

class BorrowCompleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowCompleteDialog(QJsonObject jsonObj,int currentType=0,QWidget *parent = 0);
    ~BorrowCompleteDialog();

private:
    Ui::BorrowCompleteDialog *ui;

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnMenu_Close_clicked();

    void on_btnMenu_Max_clicked();

    void on_btnMenu_Min_clicked();

    void changeSkin();

    void on_pushButton_clicked();

private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();

    int currentType;

    void init(QJsonObject jsonObj);
};

#endif // BORROWCOMPLETEDIALOG_H
