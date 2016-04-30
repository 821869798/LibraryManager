#include "booksearchform.h"
#include "ui_booksearchform.h"
#include "styletool.h"
#include "bookinfodialog.h"

BookSearchForm::BookSearchForm(int borrow,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookSearchForm)
{
    ui->setupUi(this);
    this->borrow = borrow;
    init();
}

BookSearchForm::~BookSearchForm()
{
    delete ui;
}

void BookSearchForm::init()
{
    if(!borrow)
    {
        ui->btn_borrow->hide();
    }
    searchType = 0;
    firstShow = true;
    ui->label_title->setStyleSheet("color:#ff0000");
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&BookSearchForm::finishHttp);

    radioGroup = new QButtonGroup;
    radioGroup->addButton(ui->radio_name,0);
    radioGroup->addButton(ui->radi_barcode,1);
    radioGroup->addButton(ui->radio_author,2);
    radioGroup->addButton(ui->radio_press,3);
    radioGroup->addButton(ui->radio_bookshelf,4);

    ui->tv->setSortingEnabled(true);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStandardItemModel *model = new QStandardItemModel;
    QStringList list;
    list<<"图书条形码"<<"书名"<<"作者"<<"图书类型"<<"馆藏总数"<<"在馆数";
    model->setHorizontalHeaderLabels(list);
    ui->tv->setModel(model);

//    QNetworkRequest req(QUrl(Tool::urlRoot+"book/query?page=0"));
//    netManager->get(req);
//    QString tempData = "page=0";
//    initByData(tempData);
}

void BookSearchForm::initByData(QString tempData)
{
    QNetworkRequest req(QUrl(Tool::urlRoot+"book/query?"+tempData));
    netManager->get(req);
}

void BookSearchForm::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(repData!="false")
        {
            if(path=="/book/query")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    int rowCount = ui->tv->model()->rowCount();
                    ui->tv->model()->removeRows(0,rowCount);
                    Tool::getInstance()->tableAddData(ui->tv,jsonDocument.array());
                }
            }
        }
        else
        {
            StyleTool::getInstance()->messageBoxError("操作失败!");
        }
    }
    else
    {
        StyleTool::getInstance()->netError();
    }
}

void BookSearchForm::show()
{
    QWidget::show();
    QStringList sl;
    QMap<QString,BookClassData>::const_iterator it;
    for(it=Tool::getInstance()->bookClass.begin();it!=Tool::getInstance()->bookClass.end();it++)
    {
        sl.append(it.key());
    }
    ui->typeBox->addItems(sl);
    if(firstShow)
    {
        firstShow = false;
        QString tempData = "page=0";
        initByData(tempData);
    }

}

void BookSearchForm::on_btn_jump_clicked()
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


void BookSearchForm::on_btn_query_clicked()
{
    QString queryStr = ui->lineEdit->text().trimmed();
    QString tempData = "page=0";
    if(queryStr!="")
    {
        tempData += "&type="+QString::number(radioGroup->checkedId());
        tempData +="&query="+queryStr;
    }
    if(ui->checkBox->isChecked())
    {
        int btype = Tool::getInstance()->bookClass[ui->typeBox->currentText()].id;
        tempData += "&btype="+QString::number(btype);
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

void BookSearchForm::on_btn_showall_clicked()
{
    searchType = 0;
    ui->lineEdit->setText("");
    ui->label_title->setText("全部图书");
    QString tempData = "page=0";
    ui->spinBox->setValue(1);
    initByData(tempData);
}

void BookSearchForm::on_btn_showinfo_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
       QString id = ui->tv->model()->data(ui->tv->model()->index(row,0)).toString();
       BookInfoDialog *bid = new BookInfoDialog(id);
       bid->show();
    }
}
