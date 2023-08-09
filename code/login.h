#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QMessageBox>
#include <mainwindow.h>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();


private slots:
    void on_login_clicked();

    void on_cancel_clicked();


signals:
    void mainshow();
    void cancelLogin();


private:
    Ui::Login *ui;
};

#endif // LOGIN_H
