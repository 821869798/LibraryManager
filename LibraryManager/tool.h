#ifndef TOOL_H
#define TOOL_H
#include <QtSql>
#include <QDebug>
#include <QMap>
#include "sqldata.h"
#include <QtNetwork>
#include <QJsonDocument>
#include <QTableView>
#include <QStandardItemModel>
#include <QTextCodec>

class Tool
{
public:
    Tool();
    static Tool * getInstance();
    static void setInstace(Tool *value);
    QSqlDatabase getDb() const;
//    void initMapData();
    QMap<QString,BookClassData> bookClass;
    QMap<QString,int> licenseMap;
    QNetworkCookieJar *getCookieJar();
    static QString urlRoot;

    void tableAddData(QTableView *tv, QJsonArray array);

    QByteArray getRequestData(QStringList args, QStringList data);  //获取编码后的post数据
    QString getMd5String(QString pwd);
    void initBookClassByArray(QJsonArray array);
    QByteArray getUrlEncode(QString str);  //转换url编码
    void initLicenseByArray(QJsonArray array);
    QString findLicenseById(int id);
private:
    static Tool *instance;
    QSqlDatabase db;
    QNetworkCookieJar *cookieJar;

    QTextCodec *codec;

};

#endif // TOOL_H
