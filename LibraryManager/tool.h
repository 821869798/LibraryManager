#ifndef TOOL_H
#define TOOL_H
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
    void tableAppendData(QTableView *tv, QJsonArray array);
private:
    static Tool *instance;
    QNetworkCookieJar *cookieJar;

    QTextCodec *codec;

};

#endif // TOOL_H
