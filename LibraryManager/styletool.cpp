#include "styletool.h"
#include "skinmessagebox.h"
#include "skininputbox.h"

StyleTool* StyleTool::instance = NULL;

StyleTool::StyleTool()
{

}

StyleTool *StyleTool::getInstance()
{
    if(instance==NULL){
        instance = new StyleTool();
    }
    return instance;
}

void StyleTool::SetStyle(AppStyle style)
{
    QString fileStr = ":/image/";
    if(style==StyleTool::Black){
        fileStr += "black.css";
    }else if(style==StyleTool::Blue){
        fileStr += "blue.css";
    }else if(style==StyleTool::Gray){
        fileStr += "gray.css";
    }else if(style==StyleTool::Navy){
        fileStr += "navy.css";
    }else if(style==StyleTool::Brown){
        fileStr += "brown.css";
    }else if(style==StyleTool::Dev){
        fileStr += "dev.css";
    }else if(style==StyleTool::Silvery){
        fileStr += "silvery.css";
    }
    QFile file(fileStr);
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
    qApp->setPalette(QPalette(QColor("#F0F0F0")));
}

void StyleTool::messageBoxInfo(QString info)
{
    SkinMessageBox *msg = new SkinMessageBox;
    msg->SetMessage(info, 0);
    msg->exec();
}

int StyleTool::messageBoxQuesion(QString info)
{
    SkinMessageBox *msg = new SkinMessageBox;
    msg->SetMessage(info, 1);
    return msg->exec();
}

void StyleTool::messageBoxError(QString info)
{
    SkinMessageBox *msg = new SkinMessageBox;
    msg->SetMessage(info, 2);
    msg->exec();
}

void StyleTool::netError()
{
    messageBoxError("网络连接失败！");
}

QString StyleTool::showInputBox(QString info, bool &ok,QString title) {
        SkinInputBox input;
        if(title!="")
        {
            input.setTitle(title);
        }
        input.SetMessage(info);
        ok = input.exec();
        return input.GetValue();
}
