#include "settingform.h"
#include "ui_settingform.h"
#include "styletool.h"

SettingForm::SettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingForm)
{
    ui->setupUi(this);
    ui->le_pwd->setEchoMode(QLineEdit::Password);
    ui->le_pwd->setMaxLength(20);
    ui->le_pwdNow->setEchoMode(QLineEdit::Password);
    ui->le_pwdNow->setMaxLength(20);
    ui->le_pwdNow1->setEchoMode(QLineEdit::Password);
    ui->le_pwdNow1->setMaxLength(20);
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&SettingForm::finishHttp);
}

SettingForm::~SettingForm()
{
    delete ui;
}

void SettingForm::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(repData!="false")
        {
            if(path=="/role/changepwd")
            {
                StyleTool::getInstance()->messageBoxInfo("修改密码成功");
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
    ui->le_pwd->setText("");
    ui->le_pwdNow->setText("");
    ui->le_pwdNow1->setText("");
}

void SettingForm::on_btn_Ok_clicked()
{
    QString pwd = ui->le_pwd->text().trimmed();
    QString pwdNow = ui->le_pwdNow->text().trimmed();
    QString pwdNow1 = ui->le_pwdNow1->text().trimmed();
    if(pwd==""||pwdNow==""||pwdNow1==""){
        StyleTool::getInstance()->messageBoxError("输入的数据不能为空");
        return ;
    }
    if(pwdNow!=pwdNow1){
        StyleTool::getInstance()->messageBoxError("两次输入的密码不相同");
        return ;
    }
    QString byte1 = QCryptographicHash::hash(pwd.toUtf8(),QCryptographicHash::Md5).toHex();
    QString byte2 = QCryptographicHash::hash(pwdNow.toUtf8(),QCryptographicHash::Md5).toHex();

    QNetworkRequest req(QUrl(Tool::urlRoot+"role/changepwd"));
    QByteArray postData = Tool::getInstance()->getRequestData(
                QStringList()<<"old"<<"new",
                QStringList()<<byte1<<byte2
                );
    netManager->post(req,postData);
}

