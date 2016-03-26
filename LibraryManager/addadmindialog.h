#ifndef ADDADMINDIALOG_H
#define ADDADMINDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>


namespace Ui {
class AddAdminDialog;
}

class AddAdminDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAdminDialog(QWidget *parent = 0);
    ~AddAdminDialog();
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

private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();
private:
    Ui::AddAdminDialog *ui;
};

#endif // ADDADMINDIALOG_H
