#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->label->setText("五子棋登陆界面");
}

Login::~Login()
{
    delete ui;
}

void Login::on_login_clicked()
{
    if (ui->username->text() == "root" && ui->password->text() == "000000")
    {
        this->hide();
        emit mainshow();
    }
    else if((ui->username->text() == "") || (ui->username->text() == ""))
    {
        QMessageBox::warning(this, tr("提示"), tr("用户名或密码不能为空"), QMessageBox::Ok);
        ui->username->clear();
        ui->username->setFocus();
    }

    else if(ui->username->text() != "root")
    {
        QMessageBox::warning(this, tr("提示"), tr("用户名不存在，请重新输入"), QMessageBox::Ok);
        ui->username->clear();
        ui->username->setFocus();
    }

    else if((ui->username->text() == "root") && (ui->username->text()!= "000000")){
        QMessageBox::warning(this, tr("提示"), tr("用户名密码不匹配，请重新输入"), QMessageBox::Ok);
        ui->username->setFocus();
    }
}

void Login::on_cancel_clicked()
{
    emit cancelLogin();
}
