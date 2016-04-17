#include "readermanageform.h"
#include "ui_readermanageform.h"

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

    searchType = 0;
    ui->label_title->setStyleSheet("color:#ff0000");
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&BookSearchForm::finishHttp);

    radioGroup = new QButtonGroup;
    radioGroup->addButton(ui->radio_name,0);
    radioGroup->addButton(ui->radi_barcode,1);

    ui->tv->setSortingEnabled(true);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStandardItemModel *model = new QStandardItemModel;
    QStringList list;
    list<<"图书条形码"<<"书名"<<"作者"<<"图书类型"<<"馆藏总数"<<"在馆数";
    model->setHorizontalHeaderLabels(list);
    ui->tv->setModel(model);

    QString tempData = "page=0";
    initByData(tempData);

}


void ReaderManageForm::initByData(QString tempData)
{

}
