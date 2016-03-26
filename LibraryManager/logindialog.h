#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
protected:

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnMenu_Close_clicked();


    void on_btnMenu_Min_clicked();
    void on_adminLoginBtn_clicked();

    void changeSkin();
private:
    Ui::LoginDialog *ui;
    QPoint mousePoint;
    bool mousePressed;
    QRect location;
    void InitStyle();
    void init();
};

#endif // LOGINDIALOG_H
