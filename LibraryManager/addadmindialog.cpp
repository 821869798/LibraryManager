#include "addadmindialog.h"
#include "ui_addadmindialog.h"
#include "styletool.h"
#include "tool.h"
#include <QMenu>
#include <QAction>


AddAdminDialog::AddAdminDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAdminDialog)
{
    ui->setupUi(this);
    ui->passEdit->setEchoMode(QLineEdit::Password);
    this->InitStyle();
    this->init();
}

AddAdminDialog::~AddAdminDialog()
{
    delete ui;
}

void AddAdminDialog::init()
{
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    mutex = true;
    connect(netManager,&QNetworkAccessManager::finished,this,&AddAdminDialog::finishHttp);
}

void AddAdminDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void AddAdminDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void AddAdminDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void AddAdminDialog::on_btnMenu_Close_clicked()
{
    this->close();
}

void AddAdminDialog::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        ui->btnMenu_Max->setIcon(QIcon(":/image/expand.png"));
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        ui->btnMenu_Max->setIcon(QIcon(":/image/shrink.png"));
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void AddAdminDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void AddAdminDialog::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);
    ui->btnMenu_Close->setIcon(QIcon(":/image/close.png"));
    ui->btnMenu_Max->setIcon(QIcon(":/image/expand.png"));
    ui->btnMenu_Max->setEnabled(false);
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
    connect(skin1,&QAction::triggered,this,&AddAdminDialog::changeSkin); //按钮事件
    connect(skin2,&QAction::triggered,this,&AddAdminDialog::changeSkin);
    connect(skin3,&QAction::triggered,this,&AddAdminDialog::changeSkin);
    connect(skin4,&QAction::triggered,this,&AddAdminDialog::changeSkin);
    connect(skin5,&QAction::triggered,this,&AddAdminDialog::changeSkin);
    connect(skin6,&QAction::triggered,this,&AddAdminDialog::changeSkin);
    connect(skin7,&QAction::triggered,this,&AddAdminDialog::changeSkin);
    skinMenu->addAction(skin1);
    skinMenu->addAction(skin2);
    skinMenu->addAction(skin3);
    skinMenu->addAction(skin4);
    skinMenu->addAction(skin5);
    skinMenu->addAction(skin6);
    skinMenu->addAction(skin7);
    ui->btnMenu->setMenu(skinMenu);
    ui->btnMenu->setStyleSheet("QPushButton::menu-indicator{image:None;}");
}

void AddAdminDialog::changeSkin()
{
    int type = sender()->property("tag").toInt(0);
    StyleTool::getInstance()->SetStyle((StyleTool::AppStyle)type);
}

void AddAdminDialog::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        if(repData=="true"){
            StyleTool::getInstance()->messageBoxInfo("添加管理员成功！");
            done(1);
            this->close();
        }else{
            StyleTool::getInstance()->messageBoxError("添加管理员失败！");
        }
    }else{
        StyleTool::getInstance()->netError();
    }
}

void AddAdminDialog::on_btnOk_clicked()  //添加管理员的按键事件
{
    //获取编辑框的数据
    QString id = ui->idEdit->text().trimmed();
    QString name = ui->nameEdit->text().trimmed();
    QString pass = ui->passEdit->text().trimmed();
    int bookManage = ui->bookBox->currentIndex();
    int readerManage = ui->readerBox->currentIndex();
    if(id==""||name==""){
        StyleTool::getInstance()->messageBoxError("账号或者姓名不能为空!");
        return;
    }
    QByteArray postData;
    if(pass=="")
    {
        postData = Tool::getInstance()->getRequestData(
                    QStringList()<<"username"<<"name"<<"bookmanage"<<"readermanage",
                    QStringList()<<id<<name<<QString::number(bookManage)<<QString::number(readerManage));
    }
    else
    {
        postData = Tool::getInstance()->getRequestData(
                    QStringList()<<"username"<<"name"<<"password"<<"bookmanage"<<"readermanage",
                    QStringList()<<id<<name<<Tool::getInstance()->getMd5String(pass)<<QString::number(bookManage)<<QString::number(readerManage));
    }

    QNetworkRequest req(QUrl(Tool::urlRoot+"adminer/new"));
    mutex = false;
    netManager->post(req,postData);
}

void AddAdminDialog::on_btnCancel_clicked()
{
    this->close();
}
