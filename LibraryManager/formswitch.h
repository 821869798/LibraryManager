#ifndef FORMSWITCH_H
#define FORMSWITCH_H

#include <QWidget>
#include <QMap>
#include "adminmanageform.h"
#include "bookmanageform.h"
#include "readermanageform.h"
#include "booksearchform.h"
#include "borrowhistoryform.h"
#include "settingform.h"
#include "readerinfoform.h"
class FormSwitch
{
public:
    FormSwitch();
    void hideAll();
    void addWidget(QWidget* widget,int id);
    QWidget* getWidget(int id);
    bool showWidget(int id);
    void removeWidge(int id);
    void hideWidget(int id);
    void clear();
private:
    QMap<int,QWidget*> list;
};

#endif // FORMSWITCH_H
