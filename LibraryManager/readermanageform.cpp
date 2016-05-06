#include "readermanageform.h"
#include "ui_readermanageform.h"
#include "styletool.h"
#include "readereditdialog.h"

ReaderManageForm::ReaderManageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderManageForm)
{
    ui->setupUi(this);
    this->init();
}

ReaderManageForm::~ReaderManageForm()
{
    delete ui;
}

void ReaderManageForm::show()
{
    QWidget::show();
    if(firstShow)
    {
        firstShow = false;
        QString tempData = "page=0";
        initByData(tempData);
    }
}

void ReaderManageForm::init()
{
    ui->btn_new->setIcon(QIcon(":/image/add.png"));
    ui->btn_del->setIcon(QIcon(":/image/min.png"));
    ui->btn_edit->setIcon(QIcon(":/image/edit.png"));
    ui->btn_front->setIcon(QIcon(":/image/left.png"));
    ui->btn_next->setIcon(QIcon(":/image/right.png"));
    ui->lineEdit->setMaxLength(20);

    searchType = 0;
    firstShow = true;
    ui->label_title->setStyleSheet("color:#ff0000");
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&ReaderManageForm::finishHttp);

    radioGroup = new QButtonGroup;
    radioGroup->addButton(ui->radio_name,0);
    radioGroup->addButton(ui->radio_barcode,1);

    ui->tv->setSortingEnabled(true);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStandardItemModel *model = new QStandardItemModel;
    QStringList list;
    list<<"读者条形码"<<"读者姓名"<<"性别"<<"读者类型"<<"生日"<<"证件名"<<"证件号码"<<
       "电话"<<"邮箱"<<"借书权限"<<"备注"<<"欠款";
    model->setHorizontalHeaderLabels(list);
    ui->tv->setModel(model);

}


void ReaderManageForm::initByData(QString tempData)
{
    QNetworkRequest req(QUrl(Tool::urlRoot+"reader/query?"+tempData));
    netManager->get(req);
}

void ReaderManageForm::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(path=="/reader/query")
        {
            if(repData!="false")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    QJsonArray array = jsonDocument.array();
                    int rowCount = ui->tv->model()->rowCount();
                    ui->tv->model()->removeRows(0,rowCount);
                    QStandardItemModel *model = (QStandardItemModel*)ui->tv->model();
                    for(int i=0;i<array.size();i++)
                    {
                        QJsonArray arrayTemp = array[i].toArray();
                        for(int j=0;j<arrayTemp.size();j++)
                        {
                            if(j==3)
                            {
                                if(arrayTemp[j].toInt()==0)
                                    model->setItem(i,j,new QStandardItem("普通"));
                                else
                                    model->setItem(i,j,new QStandardItem("高级"));
                            }
                            else if(j==9)
                            {
                                if(arrayTemp[j].toInt()==0)
                                    model->setItem(i,j,new QStandardItem("无"));
                                else
                                    model->setItem(i,j,new QStandardItem("有"));
                            }
                            else
                            {
                                model->setItem(i,j,new QStandardItem(arrayTemp[j].toVariant().toString()));
                            }
                        }
                    }
                }
            }
            else
            {
                StyleTool::getInstance()->messageBoxError("获取数据失败!");
            }

        }
        if(path=="/reader/delete")
        {
            if(repData=="true")
            {
                QString tempData = "page=0";
                ui->spinBox->setValue(1);
                initByData(tempData);
                StyleTool::getInstance()->messageBoxInfo("删除读者成功！");
            }
            else
            {
                StyleTool::getInstance()->messageBoxError("删除读者失败！");
            }
        }
        if(path=="/reader/change/other")
        {
               if(repData=="true")
               {
                   QString tempData = "page=0";
                   ui->spinBox->setValue(1);
                   initByData(tempData);
                    StyleTool::getInstance()->messageBoxInfo("更改成功！");
               }
               else
               {
                   StyleTool::getInstance()->messageBoxError("更改失败！");
               }
        }
    }
    else{
        StyleTool::getInstance()->netError();
    }
}

void ReaderManageForm::on_btn_jump_clicked()
{
    QString tempData;
    int value = ui->spinBox->value()-1;
    if(searchType)
    {
        tempData = getData+"&page="+QString::number(value);
    }
    else
    {
        tempData = "page="+QString::number(value);
    }
    initByData(tempData);
}

void ReaderManageForm::on_btn_front_clicked()
{
    QString tempData;
    ui->spinBox->setValue(ui->spinBox->value()-1);
    int value = ui->spinBox->value()-1;
    if(searchType)
    {
        tempData = getData+"&page="+QString::number(value);
    }
    else
    {
        tempData = "page="+QString::number(value);
    }
    initByData(tempData);
}

void ReaderManageForm::on_btn_next_clicked()
{
    QString tempData;
    ui->spinBox->setValue(ui->spinBox->value()+1);
    int value = ui->spinBox->value()-1;
    if(searchType)
    {
        tempData = getData+"&page="+QString::number(value);
    }
    else
    {
        tempData = "page="+QString::number(value);
    }
    initByData(tempData);
}


void ReaderManageForm::on_btn_query_clicked()
{
    QString queryStr = ui->lineEdit->text().trimmed();
    QString tempData = "page=0";
    if(queryStr!="")
    {
        tempData += "&type="+QString::number(radioGroup->checkedId());
        tempData +="&query="+queryStr;
    }
    if(tempData.size()<7)
    {
        getData = "";
    }
    else{
        getData = tempData.right(tempData.size()-7);
    }
    initByData(getData);
    searchType = 1;
    ui->spinBox->setValue(1);
    ui->label_title->setText("查询中");
}

void ReaderManageForm::on_btn_showall_clicked()
{
    searchType = 0;
    ui->lineEdit->setText("");
    ui->label_title->setText("全部读者");
    QString tempData = "page=0";
    ui->spinBox->setValue(1);
    initByData(tempData);
}

void ReaderManageForm::on_btn_new_clicked()
{
    ReaderEditDialog *red = new ReaderEditDialog;
    red->show();
    if(red->exec()==1)
    {
        ui->lineEdit->setText("");
        ui->label_title->setText("全部读者");
        QString tempData = "page=0";
        ui->spinBox->setValue(1);
        initByData(tempData);
    }
}

void ReaderManageForm::on_btn_edit_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
       QString id = ui->tv->model()->data(ui->tv->model()->index(row,0)).toString();
       ReaderEditDialog *red = new ReaderEditDialog(id);
       red->show();
       if(red->exec()==1){
           ui->lineEdit->setText("");
           ui->label_title->setText("全部读者");
           QString tempData = "page=0";
           ui->spinBox->setValue(1);
           initByData(tempData);
       }
    }
}

void ReaderManageForm::on_btn_del_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        int ok = StyleTool::getInstance()->messageBoxQuesion("确认要删除该读者吗？");
        if(ok==1){
            QString id = ui->tv->model()->data(ui->tv->model()->index(row,0)).toString();
            QNetworkRequest req(QUrl(Tool::urlRoot+"reader/delete?barcode="+id));
            netManager->get(req);
        }
    }
}

void ReaderManageForm::on_btn_avaliableYes_clicked()
{
    QJsonObject object;
    object.insert("avaliable",1);
    QJsonDocument document;
    document.setObject(object);
    QString json_str(document.toJson());
    changeReaderOther(json_str);
}

void ReaderManageForm::on_btn_avaliableNo_clicked()
{
    QJsonObject object;
    object.insert("avaliable",0);
    QJsonDocument document;
    document.setObject(object);
    QString json_str(document.toJson());
    changeReaderOther(json_str);
}

void ReaderManageForm::on_btn_clearArrears_clicked()
{
    QJsonObject object;
    object.insert("arrears",0);
    QJsonDocument document;
    document.setObject(object);
    QString json_str(document.toJson());
    changeReaderOther(json_str);
}

void ReaderManageForm::changeReaderOther(QString json_str)
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        QString id = ui->tv->model()->data(ui->tv->model()->index(row,0)).toString();

        QByteArray postData = Tool::getInstance()->getRequestData(
                    QStringList()<<"barcode"<<"data",
                    QStringList()<<id<<json_str
                    );
        QNetworkRequest req(QUrl(Tool::urlRoot+"reader/change/other"));
        req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        netManager->post(req,postData);
    }
}

