#include "adminmanageform.h"
#include "ui_adminmanageform.h"
#include "tool.h"
#include "styletool.h"
#include "addadmindialog.h"

AdminManageForm::AdminManageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminManageForm)
{
    ui->setupUi(this);
    ui->tv->setSortingEnabled(true);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->addAdminBtn->setIcon(QIcon(":/image/add.png"));
    ui->delAdminBtn->setIcon(QIcon(":/image/min.png"));

    mutex = true;
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&AdminManageForm::finishHttp);

    QStandardItemModel *model = new QStandardItemModel;
    QStringList list;
    list<<"账号"<<"姓名"<<"图书管理"<<"读者管理";
    model->setHorizontalHeaderLabels(list);
    ui->tv->setModel(model);

    this->init();
}

AdminManageForm::~AdminManageForm()
{
    delete ui;
}

void AdminManageForm::init()  //初始化数据
{
//    QSqlQueryModel *model = new QSqlQueryModel;
//    model->setQuery("select 账号,姓名,case 图书管理 when 1 then '允许' when 0 then '不允许' end as '图书管理',case 读者管理 when 1 then '允许' when 0 then '不允许' end as '读者管理' from 管理员");
//    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
//    QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel(this);
//    sqlproxy->setSourceModel(model);
//    ui->tv->setModel(sqlproxy);


    QNetworkRequest req(QUrl(Tool::urlRoot+"adminer/getall"));
    netManager->get(req);
    httpType = 0;
    mutex = false;
}

void AdminManageForm::on_delAdminBtn_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        int ok = StyleTool::getInstance()->messageBoxQuesion("确认要删除该管理员吗？");
        if(ok==1){
            QModelIndex index = ui->tv->model()->index(row,0);
            QString id = ui->tv->model()->data(index).toString();
            QNetworkRequest req(QUrl(Tool::urlRoot+"adminer/delete"));
            QByteArray postData;
            postData = Tool::getInstance()->getRequestData(
                        QStringList()<<"username",
                        QStringList()<<id);
            httpType = 2;
            mutex = false;
            netManager->post(req,postData);
        }
    }
}

void AdminManageForm::on_addAdminBtn_clicked()
{
    AddAdminDialog *aad = new AddAdminDialog;
    aad->show();
    if(aad->exec()==1){
        init();
    }
}

void AdminManageForm::on_showAllBtn_clicked()
{
    ui->idEdit->setText("");
    ui->nameEdit->setText("");
    if(mutex)
    {
        init();
    }
}

void AdminManageForm::on_searchBtn_clicked()  //查询管理员
{
    QString id = ui->idEdit->text().trimmed();
    QString name = ui->nameEdit->text().trimmed();
    QStringList list1,list2;
    if(!mutex)
        return;
    if(id!="")
    {
        list1<<"username";
        list2<<id;
    }
    if(name!="")
    {

        list1<<"name";
        list2<<name;
    }
    QNetworkRequest req(QUrl(Tool::urlRoot+"adminer/query"));
    QByteArray postData = Tool::getInstance()->getRequestData(list1,list2);
    httpType = 3;
    mutex = false;
    netManager->post(req,postData);
}

void AdminManageForm::on_addBookPermissionBtn_clicked()  //添加图书管理权限
{
    if(mutex)
        changePermission("1","1");
}

void AdminManageForm::on_delBookPermissionbtn_clicked()  //移除图书管理权限
{
    if(mutex)
        changePermission("0","1");
}

void AdminManageForm::on_addReaderPermissionBtn_clicked()  //添加读者管理权限
{
    if(mutex)
        changePermission("1","0");
}

void AdminManageForm::on_delReaderPermissionBtn_clicked()  //移除读者管理权限
{
    if(mutex)
        changePermission("0","0");
}


//更改权限
void AdminManageForm::changePermission(QString value,QString type)
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv->model()->index(row,0);
        QString id = ui->tv->model()->data(index).toString();
        QNetworkRequest req(QUrl(Tool::urlRoot+"adminer/set_manage"));
        QByteArray postData;
        postData = Tool::getInstance()->getRequestData(
                    QStringList()<<"username"<<"value"<<"type",
                    QStringList()<<id<<value<<type);
        httpType = 1;
        mutex = false;
        netManager->post(req,postData);
    }
}

void AdminManageForm::finishHttp(QNetworkReply *reply)
{
    mutex = true;
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        if(repData!="false"){
            if(httpType==0||httpType==3){
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    QJsonObject obj = jsonDocument.object();
                    QJsonArray array = obj["alldata"].toArray();
                    int rowCount = ui->tv->model()->rowCount();
                    ui->tv->model()->removeRows(0,rowCount);
                    Tool::getInstance()->tableAddData(ui->tv,array);
                }
            }else if(httpType==1){
                init();
                StyleTool::getInstance()->messageBoxInfo("更改权限成功");
            }else if(httpType==2){
                init();
                StyleTool::getInstance()->messageBoxInfo("删除管理员成功");
            }
        }else{
             StyleTool::getInstance()->messageBoxError("操作失败!");
        }
    }else{
        StyleTool::getInstance()->netError();
    }
}
