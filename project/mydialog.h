#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog(QWidget *parent = nullptr,QString title="添加歌单");
    ~MyDialog();

    QString menuName;
    bool ok;

private:
    Ui::MyDialog *ui;
};

#endif // MYDIALOG_H
