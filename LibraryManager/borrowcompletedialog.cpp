#include "borrowcompletedialog.h"
#include "ui_borrowcompletedialog.h"
#include "styletool.h"
#include "tool.h"
#include <QMenu>
#include <QAction>
#include "logindialog.h"


BorrowCompleteDialog::BorrowCompleteDialog(QJsonObject jsonObj,int currentType,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowCompleteDialog)
{
    ui->setupUi(this);
    this->InitStyle();
    this->currentType = currentType;
    this->init(jsonObj);
}

BorrowCompleteDialog::~BorrowCompleteDialog()
{
    delete ui;
}

void BorrowCompleteDialog::init(QJsonObject jsonObj)
{
    QStringList headList;

    if(currentType)
    {
        ui->lab_Title->setText("还书完成窗口");
        this->setWindowTitle("还书完成窗口");
        ui->label->setText("成功归还图书信息如下：");
        headList<<"图书条形码"<<"书名"<<"作者"<<"借阅日期"<<"归还日期"<<"逾期";
    }
    else
    {
        ui->lab_Title->setText("借书完成窗口");
        this->setWindowTitle("借书完成窗口");
         ui->label->setText("成功借阅图书信息如下：");
         headList<<"图书条形码"<<"书名"<<"作者"<<"在馆数"<<"借阅日期"<<"应还日期"<<"续借次数";
    }

    QStandardItemModel *model = new QStandardItemModel;

    model->setHorizontalHeaderLabels(headList);
    ui->tv->setModel(model);
    ui->tv->setSortingEnabled(true);
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->label_barcode->setText(ui->label_barcode->text()+jsonObj["barcode"].toString());
    ui->label_name->setText(ui->label_name->text()+jsonObj["name"].toString());
    QString rtype;
    if(jsonObj["rtype"].toInt()==1)
    {
        rtype = "高级";
    }
    else
    {
        rtype = "普通";
    }
    ui->label_rtype->setText(ui->label_rtype->text()+rtype);
    if(jsonObj.contains("borrowlist"))
    {
        Tool::getInstance()->tableAddData(ui->tv,jsonObj["borrowlist"].toArray());
    }
    if(jsonObj.contains("returnlist"))
    {
        Tool::getInstance()->tableAddData(ui->tv,jsonObj["returnlist"].toArray());
    }
}

void BorrowCompleteDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void BorrowCompleteDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void BorrowCompleteDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void BorrowCompleteDialog::on_btnMenu_Close_clicked()
{
    this->close();
}

void BorrowCompleteDialog::on_btnMenu_Max_clicked()
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

void BorrowCompleteDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void BorrowCompleteDialog::InitStyle()
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
    connect(skin1,&QAction::triggered,this,&BorrowCompleteDialog::changeSkin); //按钮事件
    connect(skin2,&QAction::triggered,this,&BorrowCompleteDialog::changeSkin);
    connect(skin3,&QAction::triggered,this,&BorrowCompleteDialog::changeSkin);
    connect(skin4,&QAction::triggered,this,&BorrowCompleteDialog::changeSkin);
    connect(skin5,&QAction::triggered,this,&BorrowCompleteDialog::changeSkin);
    connect(skin6,&QAction::triggered,this,&BorrowCompleteDialog::changeSkin);
    connect(skin7,&QAction::triggered,this,&BorrowCompleteDialog::changeSkin);
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

void BorrowCompleteDialog::changeSkin()
{
    int type = sender()->property("tag").toInt(0);
    StyleTool::getInstance()->SetStyle((StyleTool::AppStyle)type);
}



void BorrowCompleteDialog::on_pushButton_clicked()
{
    LoginDialog *ld = new LoginDialog;
    ld->show();
    this->close();
}
