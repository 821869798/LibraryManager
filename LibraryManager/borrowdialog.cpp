#include "borrowdialog.h"
#include "ui_borrowdialog.h"
#include "borrowcompletedialog.h"
#include "styletool.h"
#include "tool.h"
#include "logindialog.h"
#include <QMenu>
#include <QAction>

BorrowDialog::BorrowDialog(int currentType,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowDialog)
{
    ui->setupUi(this);
    this->currentType = currentType;
    this->InitStyle();
    this->init();
}

BorrowDialog::~BorrowDialog()
{
    delete ui;
}

void BorrowDialog::init()
{
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&BorrowDialog::finishHttp);
    ui->le_pwd->setEchoMode(QLineEdit::Password);
    if(currentType)
    {
        ui->lab_Title->setText("还书窗口");
        this->setWindowTitle("还书窗口");
        ui->btn_add->setText("添加还书");
        ui->btn_ok->setText("确认还书");
        ui->widget_borrow->hide();
    }
    else
    {
        ui->lab_Title->setText("借书窗口");
        this->setWindowTitle("借书窗口");
        ui->widget_return->hide();
    }
    QStandardItemModel *model = new QStandardItemModel;
    QStringList headList;
    headList<<"图书条形码"<<"书名"<<"作者"<<"图书类型"<<"馆藏总数"<<"在馆数";
    model->setHorizontalHeaderLabels(headList);
    ui->tv->setModel(model);
    ui->tv->setSortingEnabled(true);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void BorrowDialog::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(path=="/book/getone")
        {
            if(repData!="false")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    if(findTableViewEqual(jsonDocument.array()[0].toString()))
                    {
                        StyleTool::getInstance()->messageBoxError("一本书只能借一次");
                    }
                    else
                        Tool::getInstance()->tableAppendData(ui->tv,jsonDocument.array());
                }
            }
            else
            {
                 StyleTool::getInstance()->messageBoxError("没有该书!");
            }
        }
        if(path=="/borrow/add")
        {
            if(repData!="false")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    QJsonObject object = jsonDocument.object();
                    int avalible = object["avaliable"].toInt();
                    if(avalible)
                    {
                        BorrowCompleteDialog *bcd = new BorrowCompleteDialog(object);
                        bcd->show();
                        this->close();
                    }
                    else
                    {
                        StyleTool::getInstance()->messageBoxError("权限被限制，请联系管理员！");
                    }
                }
            }
            else
            {
                 StyleTool::getInstance()->messageBoxError("借书失败");
            }
        }
        if(path=="/borrow/return")
        {
            if(repData!="false")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    BorrowCompleteDialog *bcd = new BorrowCompleteDialog(jsonDocument.object(),1);
                    bcd->show();
                    this->close();
                }
            }
            else
            {
                StyleTool::getInstance()->messageBoxError("还书失败");
            }
        }

    }
    else{
        StyleTool::getInstance()->netError();
    }
}

void BorrowDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void BorrowDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void BorrowDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void BorrowDialog::on_btnMenu_Close_clicked()
{
    this->close();
}

void BorrowDialog::on_btnMenu_Max_clicked()
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

void BorrowDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void BorrowDialog::InitStyle()
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
    ui->btn_Icon->setIcon(QIcon(":/image/book.png"));
    ui->btnMenu->setIcon(QIcon(":/image/skin.png"));
    this->setWindowIcon(QIcon(":/image/book.png"));

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
    connect(skin1,&QAction::triggered,this,&BorrowDialog::changeSkin); //按钮事件
    connect(skin2,&QAction::triggered,this,&BorrowDialog::changeSkin);
    connect(skin3,&QAction::triggered,this,&BorrowDialog::changeSkin);
    connect(skin4,&QAction::triggered,this,&BorrowDialog::changeSkin);
    connect(skin5,&QAction::triggered,this,&BorrowDialog::changeSkin);
    connect(skin6,&QAction::triggered,this,&BorrowDialog::changeSkin);
    connect(skin7,&QAction::triggered,this,&BorrowDialog::changeSkin);
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

void BorrowDialog::changeSkin()
{
    int type = sender()->property("tag").toInt(0);
    StyleTool::getInstance()->SetStyle((StyleTool::AppStyle)type);
}

void BorrowDialog::on_btn_add_clicked()
{
    bool ok;
    QString barcode = StyleTool::getInstance()->showInputBox("请输入图书条形码:",ok);
    if(ok)
    {
        QNetworkRequest req(QUrl(Tool::urlRoot+"book/getone?barcode="+barcode));
        netManager->get(req);
    }
}

void BorrowDialog::on_btn_del_clicked()
{
    int row = ui->tv->currentIndex().row();
    if(row>=0){
        int ok = StyleTool::getInstance()->messageBoxQuesion("确认删除？");
        if(ok==1){
            QStandardItemModel *model = (QStandardItemModel*)ui->tv->model();
            model->removeRow(row);
        }
    }
}

bool BorrowDialog::findTableViewEqual(QString id)
{
    bool equal = false;
    QStandardItemModel *model = (QStandardItemModel*)ui->tv->model();
    for(int i=0;i<model->rowCount();i++)
    {
        if(id==model->index(i,0).data().toString())
        {
            equal = true;
            break;
        }
    }
    return equal;
}

void BorrowDialog::on_btn_ok_clicked()
{
    QStandardItemModel *model = (QStandardItemModel*)ui->tv->model();
    if(model->rowCount()==0)
    {
        StyleTool::getInstance()->messageBoxError("借书还书数量不能零！");
        return;
    }
    QString barcode = ui->le_barcode->text().trimmed();
    QString pwd = ui->le_pwd->text().trimmed();
    QString barcode_return = ui->le_barcode_return->text().trimmed();
    if(currentType==0&&(barcode==""||pwd==""))
    {
        StyleTool::getInstance()->messageBoxError("读者条形码和密码不能为空!");
        return;
    }
    if(currentType==1&&barcode_return=="")
    {
        StyleTool::getInstance()->messageBoxError("读者条形码不能为空!");
        return;
    }
    pwd = Tool::getInstance()->getMd5String(pwd);
    QJsonArray array;
    for(int i=0;i<model->rowCount();i++)
    {
        array.append(model->index(i,0).data().toString());
    }
    QJsonObject object;

    if(currentType)  //还书
    {
        object.insert("username",barcode_return);
    }
    else   //借书
    {
        object.insert("username",barcode);
        object.insert("password",pwd);
    }
    object.insert("booklist",array);
    QJsonDocument document;
    document.setObject(object);
    QString json_str(document.toJson());
    QByteArray postData = Tool::getInstance()->getRequestData(
                QStringList()<<"data",QStringList()<<json_str
                );
    QNetworkRequest req;
    if(currentType)
    {
        req.setUrl(QUrl(Tool::urlRoot+"borrow/return"));
    }
    else
    {
        req.setUrl(QUrl(Tool::urlRoot+"borrow/add"));
    }
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    netManager->post(req,postData);
}

void BorrowDialog::on_bt_login_clicked()
{
    LoginDialog *ld = new LoginDialog;
    ld->show();
    this->close();
}
