#ifndef SETTINGFORM_H
#define SETTINGFORM_H

#include <QWidget>
#include "tool.h"

namespace Ui {
class SettingForm;
}

class SettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingForm(QWidget *parent = 0);
    ~SettingForm();

private slots:
    void on_btn_Ok_clicked();

    void finishHttp(QNetworkReply *reply);
private:
    Ui::SettingForm *ui;
    QNetworkAccessManager * netManager;
};

#endif // SETTINGFORM_H
