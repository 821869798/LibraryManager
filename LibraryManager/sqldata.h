#ifndef SQLDATA
#define SQLDATA
#include <QString>
#include <QDate>

struct BookData
{
    QString id;
    QString name;
    int type;
    QString author;
    QString publisher;
    QDate date;
    double price;
    int page;
    int bookCase;
    int allNum;
    int nowNum;
};

struct BookClassData
{
    int id;
    QString name;
    int time;
};

#endif // SQLDATA

