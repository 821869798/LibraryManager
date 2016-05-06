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
    ui->le_phone->setText(array[7].toVariant().toString());
    ui->le_email->setText(array[8].toVariant().toString());
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
        if(path=="/reader/getself")
        {
            if(repData!="false")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    initByArray(jsonDocument.array());
                }
            }
            else
            {
                StyleTool::getInstance()->messageBoxError("获取数据失败");
            }
        }
        if(path=="/reader/changeself")
        {
            if(repData!="false")
            {
                StyleTool::getInstance()->messageBoxInfo("修改成功！");
            }
            else
            {
                StyleTool::getInstance()->messageBoxError("修改失败！");
            }
        }
    }
    else
    {
        StyleTool::getInstance()->netError();
    }
}

void ReaderInfoForm::on_btn_submit_clicked()
{

    int ok = StyleTool::getInstance()->messageBoxQuesion("确认提交修改的个人信息？");
    if(ok==1){
        QString email = ui->le_email->text();
        QString phone = ui->le_phone->text();
        QByteArray postData = Tool::getInstance()->getRequestData(
                    QStringList()<<"email"<<"phone",
                    QStringList()<<email<<phone
                    );
        QNetworkRequest req(QUrl(Tool::urlRoot+"reader/changeself"));
        req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        netManager->post(req,postData);
    }
}
