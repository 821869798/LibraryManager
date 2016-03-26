#ifndef SKININPUTBOX_H
#define SKININPUTBOX_H

#include <QDialog>

namespace Ui {
class SkinInputBox;
}

class SkinInputBox : public QDialog
{
    Q_OBJECT

public:
    explicit SkinInputBox(QWidget *parent = 0);
    ~SkinInputBox();
    void SetMessage(QString title);
    QString GetValue()const {
        return value;
    }

private slots:
    void on_btnOk_clicked();

private:
    Ui::SkinInputBox *ui;

    void InitStyle();   //初始化无边框窗体

    QString value;
};

#endif // SKININPUTBOX_H
