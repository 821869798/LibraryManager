#include "formswitch.h"
#include <QMapIterator>

FormSwitch::FormSwitch()
{
    list.clear();
}

void FormSwitch::hideAll()
{
    QMapIterator<int, QWidget *> i(list);
    while (i.hasNext()) {
        i.next();
        i.value()->hide();
    }
}

void FormSwitch::addWidget(QWidget *widget, int id)
{
    list.insert(id,widget);
}

QWidget *FormSwitch::getWidget(int id)
{
    if(list.contains(id))
    {
        return list[id];
    }
    return NULL;
}

bool FormSwitch::showWidget(int id)
{
    if(list.contains(id))
    {
        list[id]->show();
        return true;
    }
    return false;
}

void FormSwitch::removeWidge(int id)
{
    if(list.contains(id))
    {
        list.remove(id);
    }
}

void FormSwitch::hideWidget(int id)
{
    if(list.contains(id))
    {
        list[id]->hide();
    }
}

void FormSwitch::clear()
{
    list.clear();
}

