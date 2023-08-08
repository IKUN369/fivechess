#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameModel.h"
#include "mypainter.h"
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Login;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:



private slots:
    void on_PVP_clicked();
    void on_PVE_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void initButton();
    void getScore(int &score);

    void on_pushButton_3_clicked();

    void getLogin();
    void cancelLogin();

private:
    Ui::MainWindow *ui;
    Login *login;

    int m_Mapsize = MARGING*2 + BLOCK_SIZE * BOARD_Num_SIZE;

signals:
    void setPVPorPVE(GameType type);
    void setBlockorWhite(bool flag);


//    MyPainter *paint;
};
#endif // MAINWINDOW_H
