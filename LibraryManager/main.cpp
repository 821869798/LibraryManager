#include <QApplication>
#include "logindialog.h"
#include "styletool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StyleTool::getInstance()->SetStyle(StyleTool::Navy);
    LoginDialog ld;
    ld.show();
    return a.exec();
}
