#ifndef READERMANAGEFORM_H
#define READERMANAGEFORM_H

#include <QWidget>
#include "tool.h"
#include <QButtonGroup>

namespace Ui {
class ReaderManageForm;
}

class ReaderManageForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderManageForm(QWidget *parent = 0);
    ~ReaderManageForm();

private:
    Ui::ReaderManageForm *ui;
    void init();
    void initByData(QString tempData);
    QNetworkAccessManager * netManager;
    int searchType;
    QButtonGroup *radioGroup;
};

#endif // READERMANAGEFORM_H
