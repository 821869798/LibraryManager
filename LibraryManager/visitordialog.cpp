#include "visitordialog.h"
#include "ui_visitordialog.h"
#include "styletool.h"
#include "logindialog.h"
#include <QMenu>
#include <QAction>

VisitorDialog::VisitorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VisitorDialog)
{
    ui->setupUi(this);
    InitStyle();
    init();
}

VisitorDialog::~VisitorDialog()
{
    delete ui;
}

bool VisitorDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void VisitorDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void VisitorDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void VisitorDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void VisitorDialog::on_btnMenu_Close_clicked()
{
    qApp->quit();
}

void VisitorDialog::on_btnMenu_Max_clicked()
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

void VisitorDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void VisitorDialog::InitStyle()
{
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
        location = this->geometry();
        max = false;
        mousePressed = false;

        //安装事件监听器,让标题栏识别鼠标双击
        ui->lab_Title->installEventFilter(this);
        ui->btnMenu_Close->setIcon(QIcon(":/image/close.png"));
        ui->btnMenu_Max->setIcon(QIcon(":/image/expand.png"));
        ui->btnMenu_Min->setIcon(QIcon(":/image/min.png"));
        ui->btn_Icon->setIcon(QIcon(":/image/home.png"));
        ui->btnMenu->setIcon(QIcon(":/image/skin.png"));

        ui->btnSearch->setIcon(QIcon(":/image/search.png"));
        ui->btnSearch->setIconSize(QSize(40,40));
        ui->btnLogout->setIcon(QIcon(":/image/power.png"));
        ui->btnLogout->setIconSize(QSize(40,40));
        this->setWindowIcon(QIcon(":/image/home.png"));
        this->setWindowTitle("游客窗口");


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
        connect(skin1,&QAction::triggered,this,&VisitorDialog::changeSkin); //按钮事件
        connect(skin2,&QAction::triggered,this,&VisitorDialog::changeSkin);
        connect(skin3,&QAction::triggered,this,&VisitorDialog::changeSkin);
        connect(skin4,&QAction::triggered,this,&VisitorDialog::changeSkin);
        connect(skin5,&QAction::triggered,this,&VisitorDialog::changeSkin);
        connect(skin6,&QAction::triggered,this,&VisitorDialog::changeSkin);
        connect(skin7,&QAction::triggered,this,&VisitorDialog::changeSkin);
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


void VisitorDialog::init()
{
    fs = new FormSwitch;
    BookSearchForm *bsf = new BookSearchForm;
    ui->h1->addWidget(bsf);
    fs->addWidget(bsf,3);
    bsf->show();
}

void VisitorDialog::changeSkin()
{
    int type = sender()->property("tag").toInt(0);
    StyleTool::getInstance()->SetStyle((StyleTool::AppStyle)type);
}


void VisitorDialog::on_btnSearch_clicked()
{
    fs->hideAll();
    BookSearchForm *bsf = (BookSearchForm*)fs->getWidget(3);
    bsf->show();
}


void VisitorDialog::on_pushButton_clicked()
{
    LoginDialog *login = new LoginDialog;
    login->show();
    this->hide();
}

void VisitorDialog::on_btnLogout_clicked()
{
    LoginDialog *login = new LoginDialog;
    login->show();
    this->hide();
}
