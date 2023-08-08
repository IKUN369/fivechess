#include <GameModel.h>
#include <QPoint>
#include <time.h>
#include <stdlib.h>

void GameModel::setType(GameType type)
{
    m_gameType = type;
}

void GameModel::setStatus(GameStatus status)
{
    m_gameStatus = status;
}

void GameModel::setPlayerFlag(bool flag)
{
    m_playerFlage = flag;
}

void GameModel::startGame(GameType type)
{
    m_gameType = type;

    //初始化棋盘
    gameMapVec.clear();
    for(int i = 0; i < BOARD_Num_SIZE; i++)
    {
        vector<int> lineBoard;
        for(int j = 0; j < BOARD_Num_SIZE; j++)
        {
            lineBoard.push_back(0);
        }

        gameMapVec.push_back(lineBoard);
    }

    //如果是AI模式，需要初始化评分数组
    if(m_gameType == AI)
    {
        scoreMapvec.clear();
        for(int i = 0; i < BOARD_Num_SIZE; i++)
        {
            vector<int> lineScore;
            for(int j = 0; j < BOARD_Num_SIZE; j++)
            {
                lineScore.push_back(0);
            }

            scoreMapvec.push_back(lineScore);
        }

    }

    //轮到黑方下棋为true  白方下棋为 false
    m_playerFlage = true;
}

void GameModel::actionByPerson(int row, int col)
{
    updateGameMapVec(row, col);
}

void GameModel::actionByAI(int &clickrow, int &clickcol)
{
    //计算评分
    calculateScore();

    //从评分中找出最大值的位置
    int maxScore = 0;
    vector<std::pair<int, int>> maxPoints;

    for (int row = 1; row < BOARD_Num_SIZE; row++)
    {
        for (int col = 1; col < BOARD_Num_SIZE; col++)
        {
            //先判断坐标是否为空
            if (gameMapVec[row][col] == 0)
            {
                if (scoreMapvec[row][col] > maxScore)
                {
                    maxPoints.clear();
                    maxScore = scoreMapvec[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (scoreMapvec[row][col] == maxScore)
                {
                    maxPoints.push_back(std::make_pair(row, col));
                }

            }
        }
    }

    //如果有多个点的话，随机落子
    srand((unsigned) time(0));
    int index = rand() % maxPoints.size();

    std::pair<int, int> maxpoint = maxPoints.at(index);
    clickrow = maxpoint.first;
    clickcol = maxpoint.second;
    updateGameMapVec(clickrow, clickcol);

}

void GameModel::updateGameMapVec(int row, int col)
{
    if(m_playerFlage)   //为黑棋置为1
        gameMapVec[row][col] = 1;
    else                //为白棋置为-1
        gameMapVec[row][col] = -1;

    //换手
    m_playerFlage = !m_playerFlage;
}

bool GameModel::isWin(int row, int col)
{
    //水平方向
    for(int i = 0; i < 5; i++)
    {
        //对当前点开始从右到左滑动遍历
        if(col - i > 0 &&
                col - i + 4 < BOARD_Num_SIZE &&
                gameMapVec[row][col - i] == gameMapVec[row][col - i + 1] &&
                gameMapVec[row][col - i] == gameMapVec[row][col - i + 2] &&
                gameMapVec[row][col - i] == gameMapVec[row][col - i + 3] &&
                gameMapVec[row][col - i] == gameMapVec[row][col - i + 4] ){
            return true;
        }
    }
    //垂直方向
    for(int i = 0; i < 5; i++)
    {
        //对当前点开始从下到上滑动遍
        if(row - i > 0 &&
                row - i + 4 < BOARD_Num_SIZE &&
                gameMapVec[row - i][col] == gameMapVec[row - i + 1][col] &&
                gameMapVec[row - i][col] == gameMapVec[row - i + 2][col] &&
                gameMapVec[row - i][col] == gameMapVec[row - i + 3][col] &&
                gameMapVec[row - i][col] == gameMapVec[row - i + 4][col] ){
            return true;
        }
    }
    //左上到右下
    for(int i = 0; i < 5; i++)
    {
        if(row - i > 0 &&
                row - i + 4 < BOARD_Num_SIZE &&
                col - i > 0 &&
                col - i + 4 < BOARD_Num_SIZE &&
                gameMapVec[row - i][col - i] == gameMapVec[row + 1 - i][col + 1 - i] &&
                gameMapVec[row - i][col - i] == gameMapVec[row + 2 - i][col + 2 - i] &&
                gameMapVec[row - i][col - i] == gameMapVec[row + 3 - i][col + 3 - i] &&
                gameMapVec[row - i][col - i] == gameMapVec[row + 4 - i][col + 4 - i]){
            return true;
        }
    }
    //左下到右上
    for(int i = 0; i < 5; i++)
    {
        if(row - 4 + i  > 0 &&
                row + i< BOARD_Num_SIZE &&
                col - i > 0 &&
                col - i + 4 < BOARD_Num_SIZE &&
                gameMapVec[row + i][col - i] == gameMapVec[row - 1 + i][col + 1 - i] &&
                gameMapVec[row + i][col - i] == gameMapVec[row - 2 + i][col + 2 - i] &&
                gameMapVec[row + i][col - i] == gameMapVec[row - 3 + i][col + 3 - i] &&
                gameMapVec[row + i][col - i] == gameMapVec[row - 4 + i][col + 4 - i]){
            return true;
        }
    }

    return false;
}

void GameModel::calculateScore()
{
    //统计玩家或AI的连子数
    int personNum = 0;      //玩家连子数
    int AINum = 0;          //AI连子数
    int emptyNum = 0;       //各方向空白位的个数

    //清空评分容器
    scoreMapvec.clear();
    for (int i = 0; i < BOARD_Num_SIZE; i++)
    {
        vector<int> lineScore;
        for (int j = 0; j < BOARD_Num_SIZE; j++)
        {
            lineScore.push_back(0);
        }

        scoreMapvec.push_back(lineScore);
    }

    //计分(采用的是完全遍历)
    for (int row = 0; row < BOARD_Num_SIZE; row++)
    {
        for (int col = 0; col < BOARD_Num_SIZE; col++)
        {
            //只考虑空白点
            if (row > 0 && col > 0 && gameMapVec[row][col] == 0)
            {
                //遍历自身周围的八个方向
                for (int y = -1; y <= 1; y++)
                {
                    for (int x = -1; x <= 1; x++)
                    {
                        //重置
                        personNum = 0;
                        AINum = 0;
                        emptyNum = 0;

                        //排除原坐标
                        if (!(y==0 && x==0))
                        {
                            //每个方向延申4个棋子(在这延申的四个棋子中有几个是真人玩家的棋子)
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < BOARD_Num_SIZE &&
                                   col + i * x > 0 && col + i * x < BOARD_Num_SIZE &&
                                   gameMapVec[row + i * y][col + i * x] == 1)    //真人玩家棋子
                                {
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < BOARD_Num_SIZE &&
                                         col + i * x > 0 && col + i * x < BOARD_Num_SIZE &&
                                         gameMapVec[row + i * y][col + i * x] == 0) //空白位
                                {
                                    emptyNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < BOARD_Num_SIZE &&
                                         col + i * x > 0 && col + i * x < BOARD_Num_SIZE &&
                                         gameMapVec[row + i * y][col + i * x] == -1) // 白子
                                {
                                    AINum++;
                                }
                                else
                                    break;
                            }

                            //对黑子评分
                            if (personNum == 1)
                                scoreMapvec[row][col] += 10;       //杀二
                            else if (personNum == 2)               //杀三
                            {
                                if (emptyNum == 1)
                                    scoreMapvec[row][col] += 30;
                                if (emptyNum == 2)
                                    scoreMapvec[row][col] += 40;
                            }
                            else if (personNum == 3)               //杀四
                            {
                                //空位置不一样会影响优先值
                                if(emptyNum ==1)
                                    scoreMapvec[row][col] += 100;
                                else
                                    scoreMapvec[row][col] += 60;
                            }
                            else if (personNum == 4)               //杀五
                                scoreMapvec[row][col] += 5000;

                            //对白子评分
                            if(AINum == 0)
                                scoreMapvec[row][col] += 5;        //普通落子
                            else if (AINum == 1)                   //活二
                                scoreMapvec[row][col] += 10;
                            else if (AINum == 2)
                            {
                                if (emptyNum == 1)                 //死三
                                    scoreMapvec[row][col] += 25;
                                if (emptyNum == 2)                 //活三
                                    scoreMapvec[row][col] += 50;
                            }
                            else if (AINum == 3)
                            {
                                //空位置不一样会影响优先值
                                if(emptyNum ==1)                  //死四
                                    scoreMapvec[row][col] += 100;
                                else                              //活四
                                    scoreMapvec[row][col] += 60;
                            }
                            else if (personNum == 4)              //活五
                                scoreMapvec[row][col] += 5000;

                        }
                    }
                }
            }
        }
    }

}

