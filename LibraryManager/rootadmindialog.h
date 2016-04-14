#ifndef ROOTADMINDIALOG_H
#define ROOTADMINDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "adminmanageform.h"
#include "bookmanageform.h"
#include "readermanageform.h"
#include "booksearchform.h"

namespace Ui {
class RootAdminDialog;
}

class RootAdminDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RootAdminDialog(QWidget *parent = 0);
    ~RootAdminDialog();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnMenu_Close_clicked();

    void on_btnMenu_Max_clicked();

    void on_btnMenu_Min_clicked();

    void changeSkin();
    void on_btnAdmin_clicked();

    void on_btnBook_clicked();

    void on_btnReader_clicked();

    void on_btnSearch_clicked();

private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();

private:
    Ui::RootAdminDialog *ui;
    void init();
    AdminManageForm *amf;
    BookManageForm *bmf;
    ReaderManageForm *rmf;
    BookSearchForm *bsf;
};

#endif // ROOTADMINDIALOG_H
