#include "tool.h"
#include <QCryptographicHash>

Tool *Tool::instance =  NULL;
QString Tool::urlRoot = "http://127.0.0.1:5000/";
Tool::Tool()
{
    cookieJar = new QNetworkCookieJar;
    codec = QTextCodec::codecForName("utf-8");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");
    db.open();
    //initMapData();  //初始化获取图书类型以及有效证件的数据
}

Tool *Tool::getInstance()
{
    if(instance==NULL)
    {
        instance = new Tool();
    }
    return instance;
}

QSqlDatabase Tool::getDb() const
{
    return db;
}

void Tool::initMapData()
{
    bookClass.clear();
    identification.clear();
    QSqlQuery query(db);
    query.exec("select * from 有效证件");
    while (query.next()) {
        identification.insert(query.value(1).toString(),query.value(0).toInt());
    }
    query.exec("select * from 图书类型");
    while(query.next()){
        BookClassData bc;
        bc.id = query.value(0).toInt();
        bc.name = query.value(1).toString();
        bc.time = query.value(2).toInt();
        bookClass.insert(bc.name,bc);
    }
}

QNetworkCookieJar *Tool::getCookieJar()
{
    return this->cookieJar;
}

void Tool::tableAddData(QTableView *tv,QJsonArray array)
{
    QStandardItemModel *model = (QStandardItemModel*)tv->model();
    for(int i=0;i<array.size();i++)
    {
        QJsonArray arrayTemp = array[i].toArray();
        for(int j=0;j<arrayTemp.size();j++)
        {
            model->setItem(i,j,new QStandardItem(arrayTemp[j].toVariant().toString()));
        }
    }
}

QByteArray Tool::getUrlEncode(QString str)
{
    return codec->fromUnicode(str).toPercentEncoding();
}

QByteArray Tool::getRequestData(QStringList args,QStringList data)
{
    if(args.size()!=data.size()||args.size()==0)
    {
        return QByteArray("");
    }
    QByteArray array;
    array = QString(args[0]+"=").toUtf8()+getUrlEncode(data[0]);
    for(int i=1;i<args.size();i++)
    {
        array += QString("&"+args[i]+"=").toUtf8()+getUrlEncode(data[i]);
    }
    return array;
}

QString Tool::getMd5String(QString pwd)
{
    QByteArray byte = QCryptographicHash::hash(pwd.toUtf8(),QCryptographicHash::Md5);
    QString hashPwd = byte.toHex();
    return hashPwd;
}

void Tool::initBookClassByArray(QJsonArray array)
{
    bookClass.clear();
    for(int i=0;i<array.size();i++)
    {
        QJsonArray temp = array[i].toArray();
        BookClassData bc;
        bc.id  = temp[0].toInt();
        bc.name = temp[1].toString();
        bc.time = temp[2].toInt();
        bookClass.insert(bc.name,bc);
    }
}
