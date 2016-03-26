#include "tool.h"

Tool *Tool::instance =  NULL;
Tool::Tool()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");
    db.open();
    initMapData();  //初始化获取图书类型以及有效证件的数据
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
