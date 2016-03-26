#ifndef STYLETOOL_H
#define STYLETOOL_H
#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <QPushButton>

class StyleTool
{
private:
    static StyleTool *instance;
public:
    enum AppStyle{ //程序皮肤风格
        Black=0,
        Blue,
        Gray,
        Navy,
        Brown,
        Dev,
        Silvery
    };
    StyleTool();
    static StyleTool * getInstance();
    void SetStyle(AppStyle style);
    void messageBoxInfo(QString info); //显示信息框,仅确定按钮
    int messageBoxQuesion(QString info); //显示询问框,确定和取消按钮
    void messageBoxError(QString info);  //显示错误框,仅确定按钮

    QString showInputBox(QString info, bool &ok);
    void netError();
};

#endif // STYLETOOL_H
