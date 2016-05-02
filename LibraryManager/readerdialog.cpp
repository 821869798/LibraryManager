#include "readerdialog.h"
#include "ui_readerdialog.h"
#include "styletool.h"
#include <QMenu>
#include <QAction>
#include "booksearchform.h"
#include "settingform.h"
#include "readerinfoform.h"
#include "logindialog.h"

ReaderDialog::ReaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReaderDialog)
{
    ui->setupUi(this);
    this->InitStyle();
    this->init();
}

ReaderDialog::~ReaderDialog()
{
    delete ui;
}

ReaderDialog::init()
{
    fs = new FormSwitch;
    BookSearchForm *bsf = new BookSearchForm;
    SettingForm *sf = new SettingForm;
    BorrowHistoryForm * bhf = new BorrowHistoryForm;
    ReaderInfoForm *rid = new ReaderInfoForm;
    ui->h1->addWidget(bsf);
    ui->h1->addWidget(sf);
    ui->h1->addWidget(bhf);
    ui->h1->addWidget(rid);
    fs->addWidget(bsf,3);
    fs->addWidget(sf,4);
    fs->addWidget(bhf,5);
    fs->addWidget(rid,6);
    fs->hideAll();
    fs->showWidget(3);
}

bool ReaderDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void ReaderDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void ReaderDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void ReaderDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void ReaderDialog::on_btnMenu_Close_clicked()
{
    qApp->quit();
}

void ReaderDialog::on_btnMenu_Max_clicked()
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

void ReaderDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void ReaderDialog::InitStyle()
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
        ui->btnSetting->setIcon(QIcon(":/image/setting.png"));
        ui->btnSetting->setIconSize(QSize(40,40));
        ui->btnBorrow->setIcon(QIcon(":/image/book.png"));
        ui->btnBorrow->setIconSize(QSize(40,40));
        ui->btnInfo->setIcon(QIcon(":/image/login.png"));
        ui->btnInfo->setIconSize(QSize(40,40));
        ui->btnLogout->setIcon(QIcon(":/image/power.png"));
        ui->btnLogout->setIconSize(QSize(40,40));
        this->setWindowIcon(QIcon(":/image/home.png"));
        this->setWindowTitle("读者窗口");

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
        connect(skin1,&QAction::triggered,this,&ReaderDialog::changeSkin); //按钮事件
        connect(skin2,&QAction::triggered,this,&ReaderDialog::changeSkin);
        connect(skin3,&QAction::triggered,this,&ReaderDialog::changeSkin);
        connect(skin4,&QAction::triggered,this,&ReaderDialog::changeSkin);
        connect(skin5,&QAction::triggered,this,&ReaderDialog::changeSkin);
        connect(skin6,&QAction::triggered,this,&ReaderDialog::changeSkin);
        connect(skin7,&QAction::triggered,this,&ReaderDialog::changeSkin);
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

void ReaderDialog::changeSkin()
{
    int type = sender()->property("tag").toInt(0);
    StyleTool::getInstance()->SetStyle((StyleTool::AppStyle)type);
}

void ReaderDialog::on_btnBorrow_clicked()
{
    fs->hideAll();
    fs->showWidget(5);
}

void ReaderDialog::on_btnSetting_clicked()
{
    fs->hideAll();
    fs->showWidget(4);
}

void ReaderDialog::on_btnSearch_clicked()
{
    fs->hideAll();
    fs->showWidget(3);
}

void ReaderDialog::on_btnInfo_clicked()
{
    fs->hideAll();
    fs->showWidget(6);
}

void ReaderDialog::on_btnLogout_clicked()
{
    LoginDialog *login = new LoginDialog;
    login->show();
    this->hide();
}
