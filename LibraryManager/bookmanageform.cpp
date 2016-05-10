#include "bookmanageform.h"
#include "ui_bookmanageform.h"
#include "tool.h"
#include "styletool.h"
#include "bookeditdialog.h"
#include "borrowinfodialog.h"

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
    firstShow = true;
    sortType = 0;

    //初始化按钮图标
    ui->seartBtn->setIcon(QIcon(":/image/search.png"));
    ui->bookAddBtn->setIcon(QIcon(":/image/add.png"));
    ui->bookEditBtn->setIcon(QIcon(":/image/edit.png"));
    ui->bookDelBtn->setIcon(QIcon(":/image/min.png"));
    ui->typeAddBtn->setIcon(QIcon(":/image/add.png"));
    ui->typeDelBtn->setIcon(QIcon(":/image/min.png"));
    ui->typeEditBtn->setIcon(QIcon(":/image/edit.png"));
    ui->btn_front->setIcon(QIcon(":/image/left.png"));
    ui->btn_next->setIcon(QIcon(":/image/right.png"));

    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&BookManageForm::finishHttp);

    //初始化tableviewtc
    QStandardItemModel *model = new QStandardItemModel;
    QStringList list;
    list<<"图书条形码"<<"书名"<<"作者"<<"图书类型"<<"馆藏总数"<<"在馆数"<<"借阅历史次数";
    model->setHorizontalHeaderLabels(list);
    ui->tv->setModel(model);
    ui->tv->setSortingEnabled(true);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void BookManageForm::show()
{
    QWidget::show();
    if(firstShow)
    {
        firstShow = false;
        initBookData(1);  //初始化图书数据
        initBookClass();  //初始化图书类别
    }
}

void BookManageForm::initBookClass() //初始化图书类别
{
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->label_type->setText("");
    ui->typeNameEdit->setText("");
    ui->borrowSumEdit->setText("");
    ui->borrowSumEdit->setValidator(new QIntValidator(1,120,this));
    bookClassState = 0;

    QNetworkRequest req(QUrl(Tool::urlRoot+"booktype/getall"));
    netManager->get(req);

}

void BookManageForm::initBookClassByArray()
{
    QStringListModel *model = new QStringListModel();
    QStringList sl;
    QMap<QString,BookClassData>::const_iterator it;
    for(it=Tool::getInstance()->bookClass.begin();it!=Tool::getInstance()->bookClass.end();it++)
    {
        sl.append(it.key());
    }
    model->setStringList(sl);
    ui->listView->setModel(model);
}

void BookManageForm::initBookData(int page) //初始化图书数据
{
    ui->idEdit->setText("");
    ui->box_page->setValue(page);
    int reverse = ui->checkBox->isChecked();
    QString getData = "page="+QString::number(page-1)+"&sort="+QString::number(sortType)+"&reverse="+QString::number(reverse);
    QNetworkRequest req(QUrl(Tool::urlRoot+"book/manage/query?"+getData));
    netManager->get(req);


//    QSqlQueryModel *model = new QSqlQueryModel;
//    model->setQuery("select 图书条形码,书名,作者 from 图书");
//    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
//    QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel(this);
//    sqlproxy->setSourceModel(model);
//    ui->tv->setModel(sqlproxy);
//    ui->tv->setSortingEnabled(true);
}


void BookManageForm::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(repData!="false")
        {
            if(path=="/booktype/getall")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    Tool::getInstance()->initBookClassByArray(jsonDocument.object()["alldata"].toArray());
                    initBookClassByArray();
                }
            }
            else if(path=="/booktype/new")
            {
                initBookClass();
                StyleTool::getInstance()->messageBoxInfo("添加成功！");
            }
            else if(path=="/booktype/delete")
            {
                initBookClass();
                StyleTool::getInstance()->messageBoxInfo("删除成功！");
            }
            else if(path=="/booktype/change")
            {
                initBookClass();
                StyleTool::getInstance()->messageBoxInfo("修改成功！");
            }
            else if(path=="/book/manage/query")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    //Tool::getInstance()->initBookClassByArray(jsonDocument.object()["alldata"].toArray());
                    int rowCount = ui->tv->model()->rowCount();
                    ui->tv->model()->removeRows(0,rowCount);
                    Tool::getInstance()->tableAddData(ui->tv,jsonDocument.array());
                }
            }else if(path=="/book/delete")
            {
                initBookData(1);
                StyleTool::getInstance()->messageBoxInfo("删除成功！");
            }
        }
        else
        {
            StyleTool::getInstance()->messageBoxError("操作失败!");
        }
    }
    else{
        StyleTool::getInstance()->netError();
    }
}
void BookManageForm::on_seartBtn_clicked()
{
//    QString id = ui->idEdit->text().trimmed();
//    QSqlQueryModel *model = new QSqlQueryModel;
//    model->setQuery("select 图书条形码,书名,作者 from 图书 where 图书条形码="+id);
//    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
//    QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel(this);
//    sqlproxy->setSourceModel(model);
//    ui->tv->setModel(sqlproxy);
    QString queryStr = ui->idEdit->text();
    if(queryStr!="")
    {
        QString getData = "page="+QString::number(0)+"&query="+QString(Tool::getInstance()->getUrlEncode(queryStr));
        QNetworkRequest req(QUrl(Tool::urlRoot+"book/manage/query?"+getData));
        netManager->get(req);
    }

}

void BookManageForm::on_bookEditBtn_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
       QString id = ui->tv->model()->data(ui->tv->model()->index(row,0)).toString();
//       QSqlQuery query(Tool::getInstance()->getDb());
//       query.exec("select * from 图书 where 图书条形码="+id);
//       query.next();
//       BookData * data = new BookData();
//       data->id = id;
//       data->name = query.value(1).toString();
//       data->type = query.value(2).toInt();
//       data->author = query.value(3).toString();
//       data->publisher = query.value(4).toString();
//       data->date = query.value(5).toDate();
//       data->price = query.value(6).toDouble();
//       data->page = query.value(7).toInt();
//       data->bookCase = query.value(8).toInt();
//       data->allNum = query.value(9).toInt();
//       data->nowNum = query.value(10).toInt();
       BookEditDialog *bed = new BookEditDialog(id);
       bed->show();
       if(bed->exec()==1){
           initBookData(1);
       }
    }
}

void BookManageForm::on_bookAddBtn_clicked()
{
    BookEditDialog *bed = new BookEditDialog();
    bed->show();
    if(bed->exec()==1){
        initBookData(1);
    }
}

void BookManageForm::on_bookDelBtn_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        int ok = StyleTool::getInstance()->messageBoxQuesion("确认要删除该书？");
         if(ok==1){
             QString id = ui->tv->model()->data(ui->tv->model()->index(row,0)).toString();
//             QSqlQuery query(Tool::getInstance()->getDb());
//             query.exec("delete from 图书 where 图书条形码="+id);
//             StyleTool::getInstance()->messageBoxInfo("执行成功！");
//             initBookData(0);
             QNetworkRequest req(QUrl(Tool::urlRoot+"book/delete?barcode="+id));
             netManager->get(req);
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
            QString typeStr = ui->listView->model()->data(ui->listView->currentIndex()).toString();
            int bt_id = Tool::getInstance()->bookClass[typeStr].id;
            QNetworkRequest req(QUrl(Tool::urlRoot+"booktype/delete?id="+QString::number(bt_id)));
            netManager->get(req);
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
    initBookData(1);
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
        QStringList list1,list2;
        list1<<"name";
        list2<<name;
        if(borrow!=""&&borrow.toInt()>0){
            list1<<"borrownum";
            list2<<borrow;
        }
        QByteArray postData = Tool::getInstance()->getRequestData(list1,list2);
        QNetworkRequest req(QUrl(Tool::urlRoot+"booktype/new"));
        req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        netManager->post(req,postData);
    }else if(bookClassState==2){
        QString name = ui->typeNameEdit->text().trimmed();
        QString borrow = ui->borrowSumEdit->text().trimmed();
        if(name==""||borrow.toInt()<=0){
            StyleTool::getInstance()->messageBoxError("数据不合法！");
            return;
        }
        QByteArray postData = Tool::getInstance()->getRequestData(
                    QStringList()<<"id"<<"name"<<"borrownum",
                    QStringList()<<QString::number(currentClass)<<name<<borrow);
        QNetworkRequest req(QUrl(Tool::urlRoot+"booktype/change"));
        req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        netManager->post(req,postData);

//        QSqlQuery query(Tool::getInstance()->getDb());
//        query.prepare("update 图书类型 set 类型名称=:name,可借天数=:borrow where 类型简称=:id");
//        query.bindValue(":name",name);
//        query.bindValue(":borrow",borrow);
//        query.bindValue(":id",currentClass);
//        query.exec();
//        StyleTool::getInstance()->messageBoxInfo("执行成功！");
//        initBookClass();
    }
}

void BookManageForm::on_typeCancelBtn_clicked()
{
    ui->label_type->setText("");
    ui->typeNameEdit->setText("");
    ui->borrowSumEdit->setText("");
    bookClassState = 0;
}

void BookManageForm::on_btn_page_clicked()
{
    int value = ui->box_page->value();
    initBookData(value);
}

void BookManageForm::on_btn_front_clicked()
{
    int value = ui->box_page->value();
    value--;
    ui->box_page->setValue(value);
    value = ui->box_page->value();
    initBookData(value);
}

void BookManageForm::on_btn_next_clicked()
{
    int value = ui->box_page->value();
    value++;
    ui->box_page->setValue(value);
    value = ui->box_page->value();
    initBookData(value);
}

void BookManageForm::on_btn_noSort_clicked()
{
    sortType = 0;
    ui->lab_sort->setText("默认排序");
    initBookData(1);
}

void BookManageForm::on_btn_sortTime_clicked()
{
    sortType = 1;
    ui->lab_sort->setText("出版时间排序");
    initBookData(1);
}

void BookManageForm::on_btn_sortborrow_clicked()
{
    sortType = 2;
    ui->lab_sort->setText("借阅次数排序");
    initBookData(1);
}

void BookManageForm::on_checkBox_clicked()
{
    initBookData(1);
}

void BookManageForm::on_btn_borrowinfo_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        QString barcode = ui->tv->model()->data(ui->tv->model()->index(row,0)).toString();
        QString name = ui->tv->model()->data(ui->tv->model()->index(row,1)).toString();
        QString author = ui->tv->model()->data(ui->tv->model()->index(row,2)).toString();
        BorrowInfoDialog * borrowinfo = new BorrowInfoDialog(barcode,name,author);
        borrowinfo->show();
    }
}
