#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //登陆界面
    login = new Login;
    login->show();
    connect(login, &Login::mainshow, this, &MainWindow::getLogin);
    connect(login, &Login::cancelLogin, this, &MainWindow::cancelLogin);

    //游戏本体
    setFixedSize(m_Mapsize * 1.4, m_Mapsize* 1.1);
    initButton();
    connect(ui->chessboard, &MyPainter::sendScore, this, &MainWindow::getScore);
    connect(this, &MainWindow::setPVPorPVE, ui->chessboard, &MyPainter::initGame);
    connect(this, &MainWindow::setBlockorWhite, ui->chessboard, &MyPainter::initChess);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete login;
}

void MainWindow::on_PVP_clicked()
{
    emit setPVPorPVE(MAN);
    ui->PVP->setDisabled(true);
    ui->PVE->setDisabled(false);
}

void MainWindow::on_PVE_clicked()
{
    emit setPVPorPVE(AI);
    ui->PVP->setDisabled(false);
    ui->PVE->setDisabled(true);
}

void MainWindow::on_pushButton_clicked() //黑
{
    emit setBlockorWhite(true);
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(false);
}

void MainWindow::on_pushButton_2_clicked()  //白
{
    emit setBlockorWhite(false);
    ui->pushButton->setDisabled(false);
    ui->pushButton_2->setDisabled(true);
}

void MainWindow::initButton()
{
    ui->PVP->setDisabled(false);
    ui->PVE->setDisabled(false);
    ui->pushButton->setDisabled(false);
    ui->pushButton_2->setDisabled(false);
}

void MainWindow::getScore(int &score)
{
    ui->label->setText("分数：" + QString::number(score));
    if (score == 0)
    {
        this->ui->label->setText("分数：0");
    }

}


void MainWindow::on_pushButton_3_clicked()
{
    qApp->quit();
}

void MainWindow::getLogin()
{
    login->close();
    this->show();
}

void MainWindow::cancelLogin()
{
    this->close();
    qApp->quit();
}
