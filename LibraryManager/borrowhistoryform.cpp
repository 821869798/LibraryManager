#include "borrowhistoryform.h"
#include "ui_borrowhistoryform.h"
#include "styletool.h"

BorrowHistoryForm::BorrowHistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BorrowHistoryForm)
{
    ui->setupUi(this);
    init();
}

BorrowHistoryForm::~BorrowHistoryForm()
{
    delete ui;
}

void BorrowHistoryForm::init()
{
    ui->widget_history->hide();
    firstShow = true;
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&BorrowHistoryForm::finishHttp);

    QStandardItemModel *model = new QStandardItemModel;
    ui->tv->setModel(model);
    ui->tv->setSortingEnabled(true);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void BorrowHistoryForm::showAllBorrow()
{
    QStandardItemModel *model = (QStandardItemModel*)ui->tv->model();
    QStringList headList;
    headList<<"图书条形码"<<"书名"<<"作者"<<"借阅日期"<<"应还日期"<<"续借次数";
    model->setHorizontalHeaderLabels(headList);
    model->removeRows(0,model->rowCount());
    ui->tv->setModel(model);
    QNetworkRequest req(QUrl(Tool::urlRoot+"borrow/getborrow"));
    netManager->get(req);
}

void BorrowHistoryForm::showBorrowHistory(int page)
{
    ui->spinBox->setValue(page);
    QStandardItemModel *model = (QStandardItemModel*)ui->tv->model();
    QStringList headList;
    headList<<"图书条形码"<<"书名"<<"作者"<<"借阅日期"<<"归还日期"<<"逾期";
    model->setHorizontalHeaderLabels(headList);
    model->removeRows(0,model->rowCount());
    ui->tv->setModel(model);
    QNetworkRequest req(QUrl(Tool::urlRoot+"borrow/history?page="+QString::number(page-1)));
    netManager->get(req);
}

void BorrowHistoryForm::show()
{
    QWidget::show();
    if(firstShow)
    {
        firstShow = false;
        showAllBorrow();
    }
}

void BorrowHistoryForm::on_comboBox_currentIndexChanged(int index)
{
    if(index)
    {
        ui->widget_history->show();
        ui->widget_borrow->hide();
        showBorrowHistory();
    }else
    {
        ui->widget_history->hide();
        ui->widget_borrow->show();
        showAllBorrow();
    }
}

void BorrowHistoryForm::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(path=="/borrow/getborrow"||path=="/borrow/history")
        {
           if(repData!="false")
           {
               QJsonParseError json_error;
               QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
               if(json_error.error == QJsonParseError::NoError)
               {
                   Tool::getInstance()->tableAddData(ui->tv,jsonDocument.array());
               }
           }
           else
           {
               StyleTool::getInstance()->messageBoxError("获取数据失败");
           }
        }
        if (path=="/borrow/renew")
        {
            if(repData!="false")
            {

            }
            else
            {
                StyleTool::getInstance()->messageBoxError("续借失败");
            }
        }

    }
    else
    {
        StyleTool::getInstance()->netError();
    }
}
