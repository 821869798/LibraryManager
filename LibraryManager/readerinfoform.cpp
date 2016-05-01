#include "readerinfoform.h"
#include "ui_readerinfoform.h"
#include "styletool.h"

ReaderInfoForm::ReaderInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderInfoForm)
{
    ui->setupUi(this);
    this->init();
}

ReaderInfoForm::~ReaderInfoForm()
{
    delete ui;
}

void ReaderInfoForm::show()
{
    QWidget::show();
    if(firstShow)
    {
        firstShow = false;
        QNetworkRequest req(QUrl(Tool::urlRoot+"reader/getself"));
        netManager->get(req);
    }
}

void ReaderInfoForm::init()
{
    firstShow = true;
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&ReaderInfoForm::finishHttp);
}

void ReaderInfoForm::initByArray(QJsonArray array)
{
    ui->lab_barcode->setText(ui->lab_barcode->text()+array[0].toVariant().toString());
    ui->lab_name->setText(ui->lab_name->text()+array[1].toVariant().toString());
    ui->lab_sex->setText(ui->lab_sex->text()+array[2].toVariant().toString());
    int rtype = array[3].toInt();
    if(rtype)
    {
        ui->lab_rtype->setText(ui->lab_rtype->text()+"高级");
    }
    else
    {
        ui->lab_rtype->setText(ui->lab_rtype->text()+"普通");
    }
    ui->lab_date->setText(ui->lab_date->text()+array[4].toVariant().toString());
    ui->label_licenseid->setText(ui->label_licenseid->text()+array[5].toVariant().toString());
    ui->label_licensenum->setText(ui->label_licensenum->text()+array[6].toVariant().toString());
    ui->lab_phone->setText(ui->lab_phone->text()+array[7].toVariant().toString());
    ui->lab_email->setText(ui->lab_email->text()+array[8].toVariant().toString());
    int avaliable = array[9].toInt();
    if(avaliable)
    {
        ui->lab_avalible->setText(ui->lab_avalible->text()+"正常");
    }
    else
    {
        ui->lab_avalible->setText(ui->lab_avalible->text()+"限制");
    }
    ui->lab_note->setText(ui->lab_note->text()+array[10].toVariant().toString());
    ui->lab_arrears->setText(ui->lab_arrears->text()+array[11].toVariant().toString());
}


void ReaderInfoForm::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(repData!="false")
        {
            if(path=="/reader/getself")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    initByArray(jsonDocument.array());
                }
            }
        }
        else
        {
            StyleTool::getInstance()->messageBoxError("获取数据失败");
        }
    }
    else
    {
        StyleTool::getInstance()->netError();
    }
}
