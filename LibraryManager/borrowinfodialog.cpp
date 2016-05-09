#include "borrowinfodialog.h"
#include "ui_borrowinfodialog.h"
#include "styletool.h"
#include "tool.h"
#include <QMenu>
#include <QAction>

BorrowInfoDialog::BorrowInfoDialog(QString barcode,QString bname,QString author,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowInfoDialog)
{
    ui->setupUi(this);
    currentId = barcode;
    this->InitStyle();
    this->init(barcode,bname,author);
}

BorrowInfoDialog::~BorrowInfoDialog()
{
    delete ui;
}

void BorrowInfoDialog::init(QString barcode,QString bname,QString author)
{
    ui->btn_front->setIcon(QIcon(":/image/left.png"));
    ui->btn_next->setIcon(QIcon(":/image/right.png"));
    ui->lab_barcode->setText("图书条形码:"+barcode);
    ui->lab_name->setText("书名:"+bname);
    ui->lab_author->setText("作者:"+author);
    netManager = new QNetworkAccessManager;
    netManager->setCookieJar(Tool::getInstance()->getCookieJar());
    connect(netManager,&QNetworkAccessManager::finished,this,&BorrowInfoDialog::finishHttp);

    ui->widget_history->hide();

    QStandardItemModel *model = new QStandardItemModel;
    ui->tv->setModel(model);
    ui->tv->setSortingEnabled(true);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);

    showAllBorrow();
}

void BorrowInfoDialog::showAllBorrow()
{
    QStandardItemModel *model = (QStandardItemModel*)ui->tv->model();
    QStringList headList;
    headList<<"读者条形码"<<"读者姓名"<<"借阅日期"<<"应还日期"<<"续借次数";
    model->setHorizontalHeaderLabels(headList);
    model->removeRows(0,model->rowCount());
    ui->tv->setModel(model);
    QNetworkRequest req(QUrl(Tool::urlRoot+"borrow/manage/getborrow?barcode="+currentId));
    netManager->get(req);
}

void BorrowInfoDialog::showBorrowHistory(int page)
{
    ui->spinBox->setValue(page);
    QStandardItemModel *model = (QStandardItemModel*)ui->tv->model();
    QStringList headList;
    headList<<"读者条形码"<<"读者姓名"<<"借阅日期"<<"归还日期"<<"逾期";
    model->setHorizontalHeaderLabels(headList);
    model->removeRows(0,model->rowCount());
    ui->tv->setModel(model);
    QNetworkRequest req(QUrl(Tool::urlRoot+"borrow/manage/history?page="+QString::number(page-1)
                             +"&barcode="+currentId));
    netManager->get(req);
}

void BorrowInfoDialog::finishHttp(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString repData = QString(reply->readAll());
        QString path = reply->url().path();
        if(repData!="false")
        {

            if(path=="/borrow/manage/getborrow"||path=="/borrow/manage/history")
            {
               QJsonParseError json_error;
               QJsonDocument jsonDocument = QJsonDocument::fromJson(repData.toUtf8(), &json_error);
               if(json_error.error == QJsonParseError::NoError)
               {
                   Tool::getInstance()->tableAddData(ui->tv,jsonDocument.array());
               }
            }
        }
        else
        {
            StyleTool::getInstance()->messageBoxError("获取数据失败!");
        }
    }
    else{
        StyleTool::getInstance()->netError();
    }
}

void BorrowInfoDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void BorrowInfoDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void BorrowInfoDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void BorrowInfoDialog::on_btnMenu_Close_clicked()
{
    this->close();
}

void BorrowInfoDialog::on_btnMenu_Max_clicked()
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

void BorrowInfoDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void BorrowInfoDialog::InitStyle()
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
    connect(skin1,&QAction::triggered,this,&BorrowInfoDialog::changeSkin); //按钮事件
    connect(skin2,&QAction::triggered,this,&BorrowInfoDialog::changeSkin);
    connect(skin3,&QAction::triggered,this,&BorrowInfoDialog::changeSkin);
    connect(skin4,&QAction::triggered,this,&BorrowInfoDialog::changeSkin);
    connect(skin5,&QAction::triggered,this,&BorrowInfoDialog::changeSkin);
    connect(skin6,&QAction::triggered,this,&BorrowInfoDialog::changeSkin);
    connect(skin7,&QAction::triggered,this,&BorrowInfoDialog::changeSkin);
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

void BorrowInfoDialog::changeSkin()
{
    int type = sender()->property("tag").toInt(0);
    StyleTool::getInstance()->SetStyle((StyleTool::AppStyle)type);
}

void BorrowInfoDialog::on_btn_front_clicked()
{
    ui->spinBox->setValue(ui->spinBox->value()-1);
    int value = ui->spinBox->value();
    showBorrowHistory(value);
}

void BorrowInfoDialog::on_btn_next_clicked()
{
    ui->spinBox->setValue(ui->spinBox->value()+1);
    int value = ui->spinBox->value();
    showBorrowHistory(value);
}

void BorrowInfoDialog::on_btn_getHistory_clicked()
{
    int value = ui->spinBox->value();
    showBorrowHistory(value);
}

void BorrowInfoDialog::on_comboBox_currentIndexChanged(int index)
{
    if(index)
    {
        ui->widget_history->show();
        showBorrowHistory(1);
    }else
    {
        ui->widget_history->hide();
        showAllBorrow();
    }
}
