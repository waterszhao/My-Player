#include "mydialog.h"
#include "ui_mydialog.h"

MyDialog::MyDialog(QWidget *parent,QString title) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);
    ok = false;
    connect(ui->buttonBox,&QDialogButtonBox::accepted,[=](){
        menuName = ui->lineEdit->text();
        ok = true;
        this->close();
    });
    connect(ui->buttonBox,&QDialogButtonBox::rejected,[=](){
        this->close();
    });
}

MyDialog::~MyDialog()
{
    delete ui;
}
