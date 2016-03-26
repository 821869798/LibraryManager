#include "skininputbox.h"
#include "ui_skininputbox.h"
#include "styletool.h"

SkinInputBox::SkinInputBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SkinInputBox)
{
    ui->setupUi(this);
    this->InitStyle();
}

SkinInputBox::~SkinInputBox()
{
    delete ui;
}

void SkinInputBox::InitStyle()
{
    this->setProperty("Form", true);
    this->setProperty("CanMove", true);
    this->setWindowTitle(ui->lab_Title->text());
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    //设置图形字体
    ui->lab_Ico->setIcon(QIcon(":/image/information.png"));
    ui->btnMenu_Close->setIcon(QIcon(":/image/close.png"));
    //关联关闭按钮
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void SkinInputBox::SetMessage(QString title)
{
    ui->labInfo->setText(title);
}

void SkinInputBox::on_btnOk_clicked()
{
    if(ui->txtValue->text().trimmed()!=""){
        value = ui->txtValue->text();
        done(1);
        this->close();
    }
}
