#ifndef MYPAINTER_H
#define MYPAINTER_H

#include <QWidget>
#include <QPainter>
#include "GameModel.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>
class GameModel;

class MyPainter : public QWidget
{
    Q_OBJECT
public:
    MyPainter();
    ~MyPainter();
    explicit MyPainter(QWidget *parent = nullptr);
    void initGame(GameType type);            //初始化函数
    void initChess(bool flag);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void drawchessboard();      //画棋盘
    void drawreminder(QPen &pen, QBrush &brush);        //画提示
    void drawchess(QBrush &brush);           //画棋子
    void drawgameover();        //绘制输赢信息

    void chessbypreson();       //落子

private slots:
    //void chessOneByAI();

signals:

    void sendScore(int &Score);
private:
    int gamewidgetwidth  = MARGING*2 + BLOCK_SIZE * BOARD_Num_SIZE;

    GameModel *game;              //定义游戏类
    QPainter *painter;
    GameType m_type = AI;
    bool m_flag = true;

    int m_clickPosRow, m_clickPosCol; //存储将要点击的点
    bool selectPos = false;       //是否移动到合适的位置
    float len[4];                 //实际点到棋盘点的距离
    int flag;                     //len中最值的下标

    int m_score;                  //分数
};

#endif // MYPAINTER_H
