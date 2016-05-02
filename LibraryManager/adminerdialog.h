#ifndef ADMINERDIALOG_H
#define ADMINERDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "bookmanageform.h"
#include "readermanageform.h"
#include "booksearchform.h"
#include "settingform.h"
#include "formswitch.h"

namespace Ui {
class AdminerDialog;
}

class AdminerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminerDialog(int book = 0,int reader = 0,QWidget *parent = 0);
    ~AdminerDialog();

private:
    Ui::AdminerDialog *ui;
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

    void on_btnBook_clicked();

    void on_btnReader_clicked();

    void on_btnSearch_clicked();


    void on_btnSetting_clicked();

    void on_btnLogout_clicked();

private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();

private:
    void init(int book,int reader);
//    AdminManageForm *amf;
//    BookManageForm *bmf;
//    ReaderManageForm *rmf;
//    BookSearchForm *bsf;
//    SettingForm *setf;
    FormSwitch *fs;
};

#endif // ADMINERDIALOG_H
