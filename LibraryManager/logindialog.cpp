#include "logindialog.h"
#include "ui_logindialog.h"
#include "styletool.h"
#include "rootadmindialog.h"
#include "adminerdialog.h"
#include "readerdialog.h"
#include "visitordialog.h"
#include "borrowdialog.h"
#include <QMenu>
#include <QAction>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    InitStyle();  //初始化界面
    init();  //初始化数据
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void LoginDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void LoginDialog::on_btnMenu_Close_clicked()
{
    qApp->exit();
}


void LoginDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void LoginDialog::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);
    //设置图标
    ui->btnMenu_Close->setIcon(QIcon(":/image/close.png"));
    ui->btnMenu_Max->setIcon(QIcon(":/image/expand.png"));
    ui->btnMenu_Max->setDisabled(true);
    ui->btnMenu_Min->setIcon(QIcon(":/image/min.png"));
    ui->btn_Icon->setIcon(QIcon(":/image/login.png"));
    ui->btnMenu->setIcon(QIcon(":/image/skin.png"));

    //添加换肤菜单
    QMenu * skinMenu = new QMenu();
    QAction *skin1 = new QAction(QIcon(":/image/black.png"),"black  ",NULL);
    skin1->setProperty("tag",0);  //标记属性
    QAction *skin2 = new QAction(QIcon(":/image/blue.png"),"blue  ",NULL);
    skin2->setProperty("tag",1);
    QAction *skin3 = new QAction(QIcon(":/image/gray.png"),"gray  ",NULL);
    skin3->setProperty("tag",2);
    QAction *skin4 = new QAction(QIcon(":/image/navy.png"),"navy  ",NULL);
    skin4->setProperty("tag",3);
    QAction *skin5 = new QAction(QIcon(":/image/brown.png"),"brown  ",NULL);
    skin5->setProperty("tag",4);
    QAction *skin6 = new QAction(QIcon(":/image/dev.png"),"dev  ",NULL);
    skin6->setProperty("tag",5);
    QAction *skin7 = new QAction(QIcon(":/image/silvery.png"),"silvery  ",NULL);
    skin7->setProperty("tag",6);
    connect(skin1,&QAction::triggered,this,&LoginDialog::changeSkin); //按钮事件
    connect(skin2,&QAction::triggered,this,&LoginDialog::changeSkin);
    connect(skin3,&QAction::triggered,this,&LoginDialog::changeSkin);
    connect(skin4,&QAction::triggered,this,&LoginDialog::changeSkin);
    connect(skin5,&QAction::triggered,this,&LoginDialog::changeSkin);
    connect(skin6,&QAction::triggered,this,&LoginDialog::changeSkin);
    connect(skin7,&QAction::triggered,this,&LoginDialog::changeSkin);
    skinMenu->addAction(skin1);
    skinMenu->addAction(skin2);
    skinMenu->addAction(skin3);
    skinMenu->addAction(skin4);
    skinMenu->addAction(skin5);
    skinMenu->addAction(skin6);
    skinMenu->addAction(skin7);
    ui->btnMenu->setMenu(skinMenu);
    ui->btnMenu->setStyleSheet("QPushButton::menu-indicator{image:None;}");
    this->setWindowIcon(QIcon(":/image/login.png"));
}

void LoginDialog::changeSkin()
{
    int type = sender()->property("tag").toInt(0);
    StyleTool::getInstance()->SetStyle((StyleTool::AppStyle)type);
}

void LoginDialog::init()
{
    ui->le1->setMaxLength(12);   //设置最长输入限制
    ui->le2->setMaxLength(20);
    ui->le3->setMaxLength(12);
    ui->le4->setMaxLength(20);
    ui->le2->setEchoMode(QLineEdit::Password);
    ui->le4->setEchoMode(QLineEdit::Password);

    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&LoginDialog::finishHttp);

    QNetworkRequest req(QUrl(Tool::urlRoot+"booktype/getall"));
    netManager->get(req);
    req.setUrl(QUrl(Tool::urlRoot+"license/getall"));
    netManager->get(req);
}

void LoginDialog::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(repData!="false"){
            if(path=="/role/login")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    QJsonObject obj = jsonDocument.object();
                    int logintype = obj["logintype"].toInt();
                    if(logintype==0)
                    {
                        ReaderDialog *rd = new ReaderDialog;
                        rd->show();
                        this->close();
                    }else if(logintype==1)
                    {
                        int book = obj["book"].toInt();
                        int reader = obj["reader"].toInt();
                        AdminerDialog * ad = new AdminerDialog(book,reader);
                        ad->show();
                        this->close();
                    }
                    else if(logintype==2)
                    {
                        RootAdminDialog * rad = new RootAdminDialog;
                        rad->show();
                        this->close();
                    }
                }
            }
            if(path=="/booktype/getall")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    Tool::getInstance()->initBookClassByArray(jsonDocument.object()["alldata"].toArray());
                }
            }
            if(path=="/license/getall")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    Tool::getInstance()->initLicenseByArray(jsonDocument.array());
                }
            }

        }else{
             StyleTool::getInstance()->messageBoxError("账号或密码错误！");
        }
    }else{
        StyleTool::getInstance()->netError();
    }
}

void LoginDialog::on_adminLoginBtn_clicked() //管理员登陆
{
    QString id = ui->le3->text().trimmed();
    QString pwd = ui->le4->text().trimmed();
    if(id=="" || pwd=="")  //账号或密码为空
    {
        StyleTool::getInstance()->messageBoxError("账号或密码不能为空"); //弹出消息提示框
        return;
    }
    int loginType = ui->adminTypeBox->currentIndex()+1;  //登陆类型
    pwd = Tool::getInstance()->getMd5String(pwd); //用md5加密密码
    QByteArray postData = Tool::getInstance()->getRequestData(QStringList()<<"username"<<"password"<<"logintype",
                                                  QStringList()<<id<<pwd<<QString::number(loginType));

    QNetworkRequest req(QUrl(Tool::urlRoot+"role/login"));
    netManager->post(req,postData);


//    QSqlDatabase db = Tool::getInstance()->getDb();
//    QSqlQuery query(db);
//    QString sql;
//    if(ui->adminTypeBox->currentIndex()==1){
//        sql = "select * from 管理员 where 账号=:id and 密码=:pwd";
//    }else{
//        sql = "select * from 系统管理员 where 账号=:id and 密码=:pwd";
//    }
//    query.prepare(sql);
//    query.bindValue(":id",id);
//    query.bindValue(":pwd",pwd);
//    query.exec();
//    if(query.next()){
        //        UsrInformation::getInstance()->id = query.value(0).toString();
        //        UsrInformation::getInstance()->type = query.value(4).toInt(0);
        //        UsrInformation::getInstance()->borrowNum = query.value(5).toInt(0);
        //        ReaderDialog *rd = new ReaderDialog;
        //        rd->show();
        //        this->close();
//        if(ui->adminTypeBox->currentIndex()==0){

//        }else{
//            RootAdminDialog *rad = new RootAdminDialog();
//            rad->show();
//        }
//        RootAdminDialog *rad = new RootAdminDialog();
//        rad->show();
//        this->close();
//    }
//    else{
//        StyleTool::getInstance()->messageBoxError("账号或密码错误");
//        ui->le3->setText("");
//        ui->le4->setText("");
//    }
}

void LoginDialog::on_visitorBtn_clicked()
{
    VisitorDialog *vistor = new VisitorDialog;
    vistor->show();
    this->close();
}

void LoginDialog::on_readerLoginBtn_clicked()
{
    QString id = ui->le1->text().trimmed();
    QString pwd = ui->le2->text().trimmed();
    if(id=="" || pwd=="")  //账号或密码为空
    {
        StyleTool::getInstance()->messageBoxError("账号或密码不能为空"); //弹出消息提示框
        return;
    }
    pwd = Tool::getInstance()->getMd5String(pwd); //用md5加密密码
    QByteArray postData = Tool::getInstance()->getRequestData(QStringList()<<"username"<<"password"<<"logintype",
                                                  QStringList()<<id<<pwd<<QString::number(0));
    QNetworkRequest req(QUrl(Tool::urlRoot+"role/login"));
    netManager->post(req,postData);

}

void LoginDialog::on_btn_borrow_clicked()
{
    BorrowDialog *borrowDialog = new BorrowDialog;
    borrowDialog->show();
    this->close();
}

void LoginDialog::on_btn_return_clicked()
{
    BorrowDialog *borrowDialog = new BorrowDialog(1);
    borrowDialog->show();
    this->close();
}
