#ifndef BOOKEDITDIALOG_H
#define BOOKEDITDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "tool.h"

namespace Ui {
class BookEditDialog;
}

class BookEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookEditDialog(int editType,BookData *data = 0,QWidget *parent = 0);
    ~BookEditDialog();
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnMenu_Close_clicked();

    void on_btnMenu_Max_clicked();

    void on_btnMenu_Min_clicked();

    void changeSkin();
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

    void on_addBTn_clicked();

    void on_delBtn_clicked();

private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();

    int editType;
    BookData *bookData;
private:
    Ui::BookEditDialog *ui;
    void init();

};

#endif // BOOKEDITDIALOG_H
