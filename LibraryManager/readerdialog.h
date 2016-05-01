#ifndef READERDIALOG_H
#define READERDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "booksearchform.h"
#include "settingform.h"
#include "formswitch.h"

namespace Ui {
class ReaderDialog;
}

class ReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReaderDialog(QWidget *parent = 0);
    ~ReaderDialog();

private:
    Ui::ReaderDialog *ui;

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

    void on_btnBorrow_clicked();

    void on_btnSetting_clicked();

    void on_btnSearch_clicked();

    void on_btnInfo_clicked();

private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();

    init();
    FormSwitch *fs;
};

#endif // READERDIALOG_H
