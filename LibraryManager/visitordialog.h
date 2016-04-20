#ifndef VISITORDIALOG_H
#define VISITORDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "booksearchform.h"
#include "formswitch.h"


namespace Ui {
class VisitorDialog;
}

class VisitorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VisitorDialog(QWidget *parent = 0);
    ~VisitorDialog();

private:
    Ui::VisitorDialog *ui;
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

    void on_btnSearch_clicked();



private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();

private:
    void init();
    FormSwitch *fs;
};

#endif // VISITORDIALOG_H
