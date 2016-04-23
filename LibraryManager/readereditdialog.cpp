#include "readereditdialog.h"
#include "ui_readereditdialog.h"
#include "styletool.h"
#include "tool.h"
#include <QMenu>
#include <QAction>

ReaderEditDialog::ReaderEditDialog(QString currentId,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReaderEditDialog)
{
    ui->setupUi(this);
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&ReaderEditDialog::finishHttp);
    this->currentId = currentId;
    this->InitStyle();
    this->init();
}

ReaderEditDialog::~ReaderEditDialog()
{
    delete ui;
}

void ReaderEditDialog::init()
{
    ui->lab_Title->setText("添加读者窗口");
    this->setWindowTitle("添加读者窗口");
    QStringList sl;
    QMap<QString,int>::const_iterator it;
    QString tempText;
    for(it=Tool::getInstance()->licenseMap.begin();it!=Tool::getInstance()->licenseMap.end();it++)
    {
        sl.append(it.key());
        if(it.value()==0)
        {
            tempText = it.key();
        }
    }
    ui->box_license->addItems(sl);
    ui->box_license->setCurrentText(tempText);
    if(currentId!="")
    {
        ui->lab_Title->setText("编辑读者窗口");
        this->setWindowTitle("编辑读者窗口");
        ui->btnOk->setText("修改");
        QNetworkRequest req(QUrl(Tool::urlRoot+"reader/getone?barcode="+currentId));
        netManager->get(req);
    }
}

void ReaderEditDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void ReaderEditDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void ReaderEditDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void ReaderEditDialog::on_btnMenu_Close_clicked()
{
    this->close();
}

void ReaderEditDialog::on_btnMenu_Max_clicked()
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

void ReaderEditDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void ReaderEditDialog::InitStyle()
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
    connect(skin1,&QAction::triggered,this,&ReaderEditDialog::changeSkin); //按钮事件
    connect(skin2,&QAction::triggered,this,&ReaderEditDialog::changeSkin);
    connect(skin3,&QAction::triggered,this,&ReaderEditDialog::changeSkin);
    connect(skin4,&QAction::triggered,this,&ReaderEditDialog::changeSkin);
    connect(skin5,&QAction::triggered,this,&ReaderEditDialog::changeSkin);
    connect(skin6,&QAction::triggered,this,&ReaderEditDialog::changeSkin);
    connect(skin7,&QAction::triggered,this,&ReaderEditDialog::changeSkin);
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

void ReaderEditDialog::changeSkin()
{
    int type = sender()->property("tag").toInt(0);
    StyleTool::getInstance()->SetStyle((StyleTool::AppStyle)type);
}

void ReaderEditDialog::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(repData!="false")
        {
            if(path=="/reader/getone")
            {
                QJsonParseError json_error;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
                if(json_error.error == QJsonParseError::NoError)
                {
                    QJsonArray array = jsonDocument.array();
                    ui->le_barcode->setText(array[0].toString());
                    ui->le_name->setText(array[1].toString());
                    ui->box_sex->setCurrentText(array[2].toString());
                    ui->box_rtype->setCurrentIndex(array[3].toInt());
                    ui->dateEdit->setDate(QDate::fromString(array[4].toString(),"yyyy-MM-dd"));
                    ui->box_license->setCurrentText(array[5].toString());
                    ui->le_license->setText(array[6].toString());
                    ui->le_phone->setText(array[7].toString());
                    ui->le_email->setText(array[8].toString());
                    ui->le_note->setText(array[9].toString());
                }
            }
            if(path=="/reader/new"||path=="/reader/change")
            {
                StyleTool::getInstance()->messageBoxInfo("执行成功！");
                this->close();
                done(1);
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

void ReaderEditDialog::on_btnOk_clicked()
{
    QString barcode = ui->le_barcode->text();
    QString name = ui->le_name->text();
    QString sex = ui->box_sex->currentText();
    int rtype = ui->box_rtype->currentIndex();
    QString date = ui->dateEdit->date().toString("yyyy-MM-dd");
    int license_id = Tool::getInstance()->licenseMap[ui->box_license->currentText()];
    QString license_num = ui->le_license->text();
    QString phone = ui->le_license->text();
    QString email = ui->le_email->text();
    QString note = ui->le_note->text();
    if(barcode==""||name==""||license_num==""||phone=="")
    {
        StyleTool::getInstance()->messageBoxError("输入信息不完整！");
    }
    QJsonArray array;
    array.append(barcode);
    array.append(name);
    array.append(sex);
    array.append(rtype);
    array.append(date);
    array.append(license_id);
    array.append(license_num);
    array.append(phone);
    array.append(email);
    array.append(note);
    if(currentId!="")
        array.append(currentId);
    QJsonDocument document;
    document.setArray(array);
    QString json_str(document.toJson());
    QByteArray postData =  Tool::getInstance()->getRequestData(
                QStringList()<<"data",QStringList()<<json_str);
    QNetworkRequest req;
    if(currentId=="")
        req.setUrl(QUrl(Tool::urlRoot+"reader/new"));
    else
        req.setUrl(QUrl(Tool::urlRoot+"reader/change"));
    netManager->post(req,postData);

}

void ReaderEditDialog::on_btnCancel_clicked()
{
    this->close();
}
