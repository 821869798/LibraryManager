#ifndef BOOKINFODIALOG_H
#define BOOKINFODIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "tool.h"

namespace Ui {
class BookInfoDialog;
}

class BookInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookInfoDialog(QString currentId,QWidget *parent = 0);
    ~BookInfoDialog();

private:
    Ui::BookInfoDialog *ui;

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
private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();

    QString currentId;
    QNetworkAccessManager * netManager;
    void init();
};

#endif // BOOKINFODIALOG_H
