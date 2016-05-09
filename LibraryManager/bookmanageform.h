#ifndef BOOKMANAGEFORM_H
#define BOOKMANAGEFORM_H

#include <QWidget>
#include "tool.h"

namespace Ui {
class BookManageForm;
}

class BookManageForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookManageForm(QWidget *parent = 0);
    ~BookManageForm();

    void init();
    void show();
private slots:
    void on_seartBtn_clicked();

    void on_bookEditBtn_clicked();

    void on_bookAddBtn_clicked();

    void on_bookDelBtn_clicked();

    void on_typeAddBtn_clicked();

    void on_typeDelBtn_clicked();

    void on_showBookBtn_clicked();

    void on_typeEditBtn_clicked();

    void on_typeSubmitBtn_clicked();

    void on_typeCancelBtn_clicked();

    void finishHttp(QNetworkReply *reply);

    void on_btn_page_clicked();

    void on_btn_front_clicked();

    void on_btn_next_clicked();

    void on_btn_noSort_clicked();

    void on_btn_sortborrow_clicked();

    void on_btn_sortTime_clicked();

    void on_checkBox_clicked();

    void on_btn_borrowinfo_clicked();

private:
    Ui::BookManageForm *ui;
    void initBookData(int page);
    void initBookClass();
    int bookClassState;  //图书类别管理的编辑状态，0表示没有，1表示添加，2表示编辑
    int currentClass;

    int sortType;  //排序类型 sortType->0 默认排序 sortType->1 出版时间排序 sortType->2 借阅次数排序
    bool mutex;
    int httpType;
    QNetworkAccessManager * netManager;
    void initBookClassByArray();
    bool firstShow;
};

#endif // BOOKMANAGEFORM_H
