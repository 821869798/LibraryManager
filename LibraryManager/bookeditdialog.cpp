#include "bookeditdialog.h"
#include "ui_bookeditdialog.h"
#include "styletool.h"
#include "tool.h"
#include <QMenu>
#include <QAction>

BookEditDialog::BookEditDialog(int editType,BookData *data,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookEditDialog)
{
    ui->setupUi(this);
    InitStyle();
    this->editType = editType;
    this->bookData = data;
    init();
}

BookEditDialog::~BookEditDialog()
{
    if(editType==1)
        delete bookData;
    delete ui;
}

void BookEditDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void BookEditDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void BookEditDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void BookEditDialog::on_btnMenu_Close_clicked()
{
    this->close();
}

void BookEditDialog::on_btnMenu_Max_clicked()
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

void BookEditDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void BookEditDialog::InitStyle()
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
    connect(skin1,&QAction::triggered,this,&BookEditDialog::changeSkin); //按钮事件
    connect(skin2,&QAction::triggered,this,&BookEditDialog::changeSkin);
    connect(skin3,&QAction::triggered,this,&BookEditDialog::changeSkin);
    connect(skin4,&QAction::triggered,this,&BookEditDialog::changeSkin);
    connect(skin5,&QAction::triggered,this,&BookEditDialog::changeSkin);
    connect(skin6,&QAction::triggered,this,&BookEditDialog::changeSkin);
    connect(skin7,&QAction::triggered,this,&BookEditDialog::changeSkin);
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

void BookEditDialog::changeSkin()
{
    int type = sender()->property("tag").toInt(0);
    StyleTool::getInstance()->SetStyle((StyleTool::AppStyle)type);
}

void BookEditDialog::init(){
    ui->addBTn->setIcon(QIcon(":/image/add.png"));
    ui->delBtn->setIcon(QIcon(":/image/min.png"));
    ui->numLabel->setAlignment(Qt::AlignCenter);
    ui->numLabel->setText("1");
    ui->priceEdit->setValidator(new QDoubleValidator(0,10000,2));
    ui->pageEdit->setValidator(new QIntValidator(0,10000));
    ui->bookCaseEdit->setValidator(new QIntValidator(0,1000));
    QStringList sl;
    QString typeStr;
    QMap<QString,BookClassData>::const_iterator it;
    for(it=Tool::getInstance()->bookClass.begin();it!=Tool::getInstance()->bookClass.end();it++)
    {
        sl.append(it.key());
        if(editType==1&&it.value().id==bookData->type){
            typeStr = it.key();
        }
    }
    ui->typeBox->addItems(sl);
    if(editType==1){
        ui->lab_Title->setText("编辑图书窗口");
        this->setWindowTitle("编辑图书窗口");
        ui->idEdit->setEnabled(false);
        ui->btnOk->setText("修改");
        ui->idEdit->setText(bookData->id);
        ui->nameEdit->setText(bookData->name);
        ui->authorEdit->setText(bookData->author);
        ui->typeBox->setCurrentText(typeStr);
        ui->publisherEdit->setText(bookData->publisher);
        ui->dateEdit->setDate(bookData->date);
        ui->priceEdit->setText(QString::number(bookData->price));
        ui->pageEdit->setText(QString::number(bookData->page));
        ui->bookCaseEdit->setText(QString::number(bookData->bookCase));
        ui->numLabel->setText(QString::number(bookData->allNum));
    }
    else if(editType){
        ui->lab_Title->setText("添加图书窗口");
        this->setWindowTitle("添加图书窗口");
        ui->btnOk->setText("提交");
    }
}

void BookEditDialog::on_btnOk_clicked()  //提交按钮
{

    QString id = ui->idEdit->text().trimmed();
    QString name = ui->nameEdit->text().trimmed();
    QString author = ui->authorEdit->text().trimmed();
    int type = Tool::getInstance()->bookClass[ui->typeBox->currentText()].id;
    QString publisher = ui->publisherEdit->text().trimmed();
    QString date = ui->dateEdit->date().toString("yyyy-MM-dd");
    if(id==""||name==""||author==""||publisher==""||ui->priceEdit->text().trimmed()==""||
            ui->pageEdit->text().trimmed()==""||ui->bookCaseEdit->text().trimmed()==""){
        StyleTool::getInstance()->messageBoxError("输入信息不完整！");
    }
    double price = ui->priceEdit->text().trimmed().toDouble();
    int page = ui->pageEdit->text().trimmed().toInt();
    int bookCase = ui->bookCaseEdit->text().trimmed().toInt();
    int num = ui->numLabel->text().toInt();
    QString sql;
    if(editType==1){
        sql = "update 图书 set 书名=:name,图书类型=:type,作者=:author,出版社=:publisher,出版日期=:date,价格=:price,页码=:page,所在书架=:bookCase,馆藏总数=:allNum,在馆数=:nowNum where 图书条形码=:id";
    }else{
        sql = "insert into 图书 values(:id,:name,:type,:author,:publisher,:date,:price,:page,:bookCase,:allNum,:nowNum)";
    }
    QSqlQuery query(Tool::getInstance()->getDb());
    query.prepare(sql);
    query.bindValue(":id",id);
    query.bindValue(":name",name);
    query.bindValue(":type",type);
    query.bindValue(":author",author);
    query.bindValue(":publisher",publisher);
    query.bindValue(":date",date);
    query.bindValue(":price",price);
    query.bindValue(":page",page);
    query.bindValue(":bookCase",bookCase);
    query.bindValue(":allNum",num);
    if(editType==1)
        query.bindValue(":nowNum",bookData->nowNum);
    else
        query.bindValue(":nowNum",num);
    query.exec();
    qDebug()<<query.lastQuery();
    StyleTool::getInstance()->messageBoxInfo("执行成功！");
    this->close();
    done(1);

}

void BookEditDialog::on_btnCancel_clicked()
{
    this->close();
}

void BookEditDialog::on_addBTn_clicked()
{
    int num = ui->numLabel->text().toInt();
    num++;
    if(editType==1){
        bookData->allNum++;
        bookData->nowNum++;
    }
    ui->numLabel->setText(QString::number(num));
}

void BookEditDialog::on_delBtn_clicked()
{
    int num = ui->numLabel->text().toInt();
    if(editType==1){
        if(bookData->nowNum>0&&num>1){
            num--;
            bookData->nowNum--;
            bookData->allNum--;
        }
    }else{
        if(num>1){
            num--;
        }
    }
    ui->numLabel->setText(QString::number(num));
}
