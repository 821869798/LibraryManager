#include "readereditdialog.h"
#include "ui_readereditdialog.h"
#include "styletool.h"
#include "tool.h"
#include <QMenu>
#include <QAction>

ReaderEditDialog::ReaderEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReaderEditDialog)
{
    ui->setupUi(this);
}

ReaderEditDialog::~ReaderEditDialog()
{
    delete ui;
}

void ReaderEditDialog::init()
{

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

}
