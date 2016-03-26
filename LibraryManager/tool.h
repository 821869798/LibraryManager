#ifndef TOOL_H
#define TOOL_H
#include <QtSql>
#include <QDebug>
#include <QMap>
#include "sqldata.h"

class Tool
{
public:
    Tool();
    static Tool * getInstance();
    static void setInstace(Tool *value);
    QSqlDatabase getDb() const;
    void initMapData();
    QMap<QString,BookClassData> bookClass;
    QMap<QString,int> identification;

private:
    static Tool *instance;
    QSqlDatabase db;

};

#endif // TOOL_H
