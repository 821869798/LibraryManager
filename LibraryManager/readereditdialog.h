#ifndef READEREDITDIALOG_H
#define READEREDITDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "tool.h"

namespace Ui {
class ReaderEditDialog;
}

class ReaderEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReaderEditDialog(QWidget *parent = 0);
    ~ReaderEditDialog();

private:
    Ui::ReaderEditDialog *ui;
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
    void init();
};

#endif // READEREDITDIALOG_H
