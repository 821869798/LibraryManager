#ifndef FORMSWITCH_H
#define FORMSWITCH_H

#include <QWidget>
#include <QMap>
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
