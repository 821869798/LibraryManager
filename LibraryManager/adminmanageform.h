#ifndef ADMINMANAGEFORM_H
#define ADMINMANAGEFORM_H

#include <QWidget>

namespace Ui {
class AdminManageForm;
}

class AdminManageForm : public QWidget
{
    Q_OBJECT

public:
    explicit AdminManageForm(QWidget *parent = 0);
    ~AdminManageForm();

    void init();
private slots:
    void on_delAdminBtn_clicked();

    void on_addAdminBtn_clicked();

    void on_showAllBtn_clicked();

    void on_searchBtn_clicked();

    void on_addBookPermissionBtn_clicked();

    void on_delBookPermissionbtn_clicked();

    void on_addReaderPermissionBtn_clicked();

    void on_delReaderPermissionBtn_clicked();

private:
    Ui::AdminManageForm *ui;
};

#endif // ADMINMANAGEFORM_H
