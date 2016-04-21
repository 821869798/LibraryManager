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

void ReaderManageForm::init()
{
    ui->btn_new->setIcon(QIcon(":/image/add.png"));
    ui->btn_del->setIcon(QIcon(":/image/min.png"));
    ui->btn_edit->setIcon(QIcon(":/image/edit.png"));
    ui->lineEdit->setMaxLength(20);

    searchType = 0;
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


    QString tempData = "page=0";
    initByData(tempData);
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
        if(repData!="false")
        {
            if(path=="/reader/query")
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
        ui->label_title->setText("全部读者");
        QString tempData = "page=0";
        initByData(tempData);
    }
}
