#include "bookmanageform.h"
#include "ui_bookmanageform.h"
#include "tool.h"
#include "styletool.h"
#include "bookeditdialog.h"

BookManageForm::BookManageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookManageForm)
{
    ui->setupUi(this);
    this->init(); //初始化数据
}

BookManageForm::~BookManageForm()
{
    delete ui;
}

void BookManageForm::init()
{

    //初始化按钮图标
    ui->seartBtn->setIcon(QIcon(":/image/search.png"));
    ui->bookAddBtn->setIcon(QIcon(":/image/add.png"));
    ui->bookEditBtn->setIcon(QIcon(":/image/edit.png"));
    ui->bookDelBtn->setIcon(QIcon(":/image/min.png"));
    ui->typeAddBtn->setIcon(QIcon(":/image/add.png"));
    ui->typeDelBtn->setIcon(QIcon(":/image/min.png"));
    ui->typeEditBtn->setIcon(QIcon(":/image/edit.png"));

    initBookData();  //初始化图书数据
    initBookClass();  //初始化图书类别
}

void BookManageForm::initBookClass() //初始化图书类别
{
    Tool::getInstance()->initMapData();
    ui->label_type->setText("");
    ui->typeNameEdit->setText("");
    ui->borrowSumEdit->setText("");
    ui->borrowSumEdit->setValidator(new QIntValidator(1,120,this));
    bookClassState = 0;
    QStringListModel *model = new QStringListModel();
    QStringList sl;
    QMap<QString,BookClassData>::const_iterator it;
    for(it=Tool::getInstance()->bookClass.begin();it!=Tool::getInstance()->bookClass.end();it++)
    {
        sl.append(it.key());
    }
    model->setStringList(sl);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void BookManageForm::initBookData() //初始化图书数据
{
    ui->idEdit->setText("");
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select 图书条形码,书名,作者 from 图书");
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel(this);
    sqlproxy->setSourceModel(model);
    ui->tv->setModel(sqlproxy);
    ui->tv->setSortingEnabled(true);
}

void BookManageForm::on_seartBtn_clicked()
{
    QString id = ui->idEdit->text().trimmed();
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select 图书条形码,书名,作者 from 图书 where 图书条形码="+id);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel(this);
    sqlproxy->setSourceModel(model);
    ui->tv->setModel(sqlproxy);
}

void BookManageForm::on_bookEditBtn_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
       QString id = ui->tv->model()->data(ui->tv->model()->index(row,0)).toString();
       QSqlQuery query(Tool::getInstance()->getDb());
       query.exec("select * from 图书 where 图书条形码="+id);
       query.next();
       BookData * data = new BookData();
       data->id = id;
       data->name = query.value(1).toString();
       data->type = query.value(2).toInt();
       data->author = query.value(3).toString();
       data->publisher = query.value(4).toString();
       data->date = query.value(5).toDate();
       data->price = query.value(6).toDouble();
       data->page = query.value(7).toInt();
       data->bookCase = query.value(8).toInt();
       data->allNum = query.value(9).toInt();
       data->nowNum = query.value(10).toInt();
       BookEditDialog *bed = new BookEditDialog(1,data);
       bed->show();
       if(bed->exec()==1){
           initBookData();
       }
    }
}

void BookManageForm::on_bookAddBtn_clicked()
{
    BookEditDialog *bed = new BookEditDialog(0);
    bed->show();
    if(bed->exec()==1){
        initBookData();
    }
}

void BookManageForm::on_bookDelBtn_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        int ok = StyleTool::getInstance()->messageBoxQuesion("确认要删除该书？");
         if(ok==1){
             QString id = ui->tv->model()->data(ui->tv->model()->index(row,0)).toString();
             QSqlQuery query(Tool::getInstance()->getDb());
             query.exec("delete from 图书 where 图书条形码="+id);
             StyleTool::getInstance()->messageBoxInfo("执行成功！");
             initBookData();
         }
    }
}

void BookManageForm::on_typeAddBtn_clicked()
{
    ui->label_type->setText("添加：天数能为空，默认30");
    bookClassState = 1;
    ui->typeNameEdit->setText("");
    ui->borrowSumEdit->setText("");
}

void BookManageForm::on_typeDelBtn_clicked()
{
    int row = ui->listView->currentIndex().row();
    if(row>=0){
        int ok = StyleTool::getInstance()->messageBoxQuesion("确认要删除该图书类型吗？");
        if(ok==1){
            QSqlQuery query(Tool::getInstance()->getDb());
            QString typeStr = ui->listView->model()->data(ui->listView->currentIndex()).toString();
            BookClassData bc = Tool::getInstance()->bookClass[typeStr];
            query.exec("select * from 图书 where 图书类型="+QString::number(bc.id));
            if(query.next()){
                StyleTool::getInstance()->messageBoxError("该类型的图书已存在，不能删除");
            }else{
                query.exec("delete from 图书类型 where 类型简称="+QString::number(bc.id));
                StyleTool::getInstance()->messageBoxInfo("执行成功！");
                initBookClass();
            }
        }
    }


}

void BookManageForm::on_typeEditBtn_clicked()
{
    int row = ui->listView->currentIndex().row();
    if(row>=0){
        ui->label_type->setText("编辑：修改数据并提交");
        QString typeStr = ui->listView->model()->data(ui->listView->currentIndex()).toString();
        BookClassData bc = Tool::getInstance()->bookClass[typeStr];
        bookClassState = 2;
        ui->typeNameEdit->setText(bc.name);
        ui->borrowSumEdit->setText(QString::number(bc.time));
        currentClass = bc.id;
    }

}

void BookManageForm::on_showBookBtn_clicked()
{
    initBookData();
}



void BookManageForm::on_typeSubmitBtn_clicked()
{
    if(bookClassState==1)
    {
        QString name = ui->typeNameEdit->text().trimmed();
        QString borrow = ui->borrowSumEdit->text().trimmed();
        if(name=="")
        {
            StyleTool::getInstance()->messageBoxError("类型名称不能为空");
            return;
        }
        QString sql;
        if(borrow==""){
            sql = "insert into 图书类型(类型名称) values(:name)";
        }else{
            sql = "insert into 图书类型(类型名称,可借天数) values(:name,:borrow)";
        }
        QSqlQuery query(Tool::getInstance()->getDb());
        query.prepare(sql);
        query.bindValue(":name",name);
        query.bindValue(":borrow",borrow);
        query.exec();
        StyleTool::getInstance()->messageBoxInfo("执行成功！");
        initBookClass();
    }else if(bookClassState==2){
        QString name = ui->typeNameEdit->text().trimmed();
        QString borrow = ui->borrowSumEdit->text().trimmed();
        if(name==""||borrow.toInt()<=0){
            StyleTool::getInstance()->messageBoxError("数据不合法！");
            return;
        }
        QSqlQuery query(Tool::getInstance()->getDb());
        query.prepare("update 图书类型 set 类型名称=:name,可借天数=:borrow where 类型简称=:id");
        query.bindValue(":name",name);
        query.bindValue(":borrow",borrow);
        query.bindValue(":id",currentClass);
        query.exec();
        StyleTool::getInstance()->messageBoxInfo("执行成功！");
        initBookClass();
    }
}

void BookManageForm::on_typeCancelBtn_clicked()
{
    ui->label_type->setText("");
    ui->typeNameEdit->setText("");
    ui->borrowSumEdit->setText("");
    bookClassState = 0;
}
