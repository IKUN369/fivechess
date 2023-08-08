#ifndef GAMEMODEL_H
#define GAMEMODEL_H

/* ---------- 五子棋游戏模型类 ---------- */
#include <iostream>
#include <vector>

using namespace std;
// 游戏类型
enum GameType
{
    MAN,
    AI
};

// 游戏状态
enum GameStatus
{
    PLAYING,
    WIN,
    DEAD
};

// 棋盘尺寸
const int BOARD_Num_SIZE = 15;  //15*15大小

const int MARGING = 30;     //边缘空隙
const int CHESS_RADIUS = 20;     //棋子半径
const int MARK_SIZE = 10;         //落子标记边长
const int BLOCK_SIZE = 60;       //格子大小
const int POS_OFFSET = BLOCK_SIZE * 0.4;    //鼠标点击的模糊距离上限

const int AI_THINK_TIME = 700;   //AI下棋的思考时间

class GameModel
{
public:
    GameModel(){};
    ~GameModel(){};

    void setType(GameType type);
    void setStatus(GameStatus status);
    void setPlayerFlag(bool flag);
public:
    // 存储当前游戏棋盘和棋子情况，0为空，1为黑子，-1为白子
    vector<vector<int>> gameMapVec;

    // 存储各点评分情况
    vector<vector<int>> scoreMapvec;

    // 标记下棋方，黑方 true， 白方(AI) false
    bool m_playerFlage;

    GameType m_gameType;      //游戏模式
    GameStatus m_gameStatus;  //游戏状态

    void startGame(GameType type);     //启动游戏

    void actionByPerson(int row, int col);      //人下棋
    void actionByAI(int &clickrow, int &clickcol);        //AI下棋
    void updateGameMapVec(int row, int col);    //更新数据
    bool isWin(int row, int col);               //判断是否胜利
    void calculateScore();                      //AI计算评分函数


};
#endif // GAMEMODEL_H
