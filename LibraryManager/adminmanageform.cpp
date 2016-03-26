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
    ui->addAdminBtn->setIcon(QIcon(":/image/add.png"));
    ui->delAdminBtn->setIcon(QIcon(":/image/min.png"));
    this->init();
}

AdminManageForm::~AdminManageForm()
{
    delete ui;
}

void AdminManageForm::init()  //初始化数据
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select 账号,姓名,case 图书管理 when 1 then '允许' when 0 then '不允许' end as '图书管理',case 读者管理 when 1 then '允许' when 0 then '不允许' end as '读者管理' from 管理员");
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel(this);
    sqlproxy->setSourceModel(model);
    ui->tv->setModel(sqlproxy);
}

void AdminManageForm::on_delAdminBtn_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        int ok = StyleTool::getInstance()->messageBoxQuesion("确认要删除该管理员吗？");
        if(ok==1){
            QModelIndex index = ui->tv->model()->index(row,0);
            QString id = ui->tv->model()->data(index).toString();
            QSqlQuery query(Tool::getInstance()->getDb());
            query.exec("delete from 管理员 where 账号='"+id+"';");
            this->init();
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
    init();
}

void AdminManageForm::on_searchBtn_clicked()  //查询管理员
{
    QString sql = "select 账号,姓名,case 图书管理 when 1 then '允许' when 0 then '不允许' end as '图书管理',case 读者管理 when 1 then '允许' when 0 then '不允许' end as '读者管理' from 管理员 where 1=1 and ";
    QString id = ui->idEdit->text().trimmed();
    QString name = ui->nameEdit->text().trimmed();
    if(id!="")
    {
        sql += "账号='"+id+"' and ";
    }
    if(name!="")
    {
        sql += "姓名 like '%"+name+"%' and ";
    }
    sql =  sql.left(sql.size()-5);
    QSqlQueryModel*model = new QSqlQueryModel;
    model->setQuery(sql);
    QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel(this);
    sqlproxy->setSourceModel(model);
    ui->tv->setModel(sqlproxy);
}

void AdminManageForm::on_addBookPermissionBtn_clicked()  //添加图书管理权限
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv->model()->index(row,0);
        QString id = ui->tv->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("update 管理员 set 图书管理=1 where 账号='"+id+"'");
        StyleTool::getInstance()->messageBoxInfo("执行成功");
        this->init();
    }
}

void AdminManageForm::on_delBookPermissionbtn_clicked()  //移除图书管理权限
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv->model()->index(row,0);
        QString id = ui->tv->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("update 管理员 set 图书管理=0 where 账号='"+id+"'");
        StyleTool::getInstance()->messageBoxInfo("执行成功");
        this->init();
    }
}

void AdminManageForm::on_addReaderPermissionBtn_clicked()  //添加读者管理权限
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv->model()->index(row,0);
        QString id = ui->tv->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("update 管理员 set 读者管理=1 where 账号='"+id+"'");
        StyleTool::getInstance()->messageBoxInfo("执行成功");
        this->init();
    }
}

void AdminManageForm::on_delReaderPermissionBtn_clicked()  //移除读者管理权限
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        QModelIndex index = ui->tv->model()->index(row,0);
        QString id = ui->tv->model()->data(index).toString();
        QSqlQuery query(Tool::getInstance()->getDb());
        query.exec("update 管理员 set 读者管理=0 where 账号='"+id+"'");
        StyleTool::getInstance()->messageBoxInfo("执行成功");
        this->init();
    }
}
