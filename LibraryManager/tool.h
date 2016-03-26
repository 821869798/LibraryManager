#ifndef TOOL_H
#define TOOL_H
#include <QtSql>
#include <QDebug>
#include <QMap>
#include "sqldata.h"
#include <QtNetwork>

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
    QNetworkCookieJar *getCookieJar();
    static QString urlRoot;

private:
    static Tool *instance;
    QSqlDatabase db;
    QNetworkCookieJar *cookieJar;

};

#endif // TOOL_H
