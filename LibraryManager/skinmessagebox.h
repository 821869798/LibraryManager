#ifndef SKINMESSAGEBOX_H
#define SKINMESSAGEBOX_H

#include <QDialog>

namespace Ui {
class SkinMessageBox;
}

class SkinMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit SkinMessageBox(QWidget *parent = 0);
    ~SkinMessageBox();

    void SetMessage(const QString &msg, int type);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnOk_clicked();

private:
    Ui::SkinMessageBox *ui;

    QPoint mousePoint;              //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;              //鼠标是否按下
};

#endif // SKINMESSAGEBOX_H
