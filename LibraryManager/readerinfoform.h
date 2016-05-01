#ifndef READERINFOFORM_H
#define READERINFOFORM_H

#include <QWidget>
#include "tool.h"

namespace Ui {
class ReaderInfoForm;
}

class ReaderInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderInfoForm(QWidget *parent = 0);
    ~ReaderInfoForm();
    void show();

private:
    Ui::ReaderInfoForm *ui;

    void finishHttp(QNetworkReply *reply);
    void init();
    void initByArray(QJsonArray array);
    bool firstShow;
    QNetworkAccessManager * netManager;
};

#endif // READERINFOFORM_H
