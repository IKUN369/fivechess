#include "mypainter.h"
#include <math.h>


MyPainter::MyPainter()
{
    setFixedSize(gamewidgetwidth, gamewidgetwidth);
}

MyPainter::~MyPainter()
{
    delete game;
    delete painter;
}

MyPainter::MyPainter(QWidget *parent) : QWidget(parent)
{
    resize(gamewidgetwidth ,gamewidgetwidth);

    initGame(m_type);
}

//初始化
void MyPainter::initGame(GameType type)
{
    game = new GameModel;
    m_type = type;
    //在数据模型中进行初始化
    game->startGame(type);
    game->setStatus(PLAYING);
    update();
}

void MyPainter::initChess(bool flag)
{
    m_flag = flag;
    game->setPlayerFlag(flag);

}

//事件响应：绘图事件，鼠标移动，鼠标释放
void MyPainter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    painter = new QPainter(this);
    QPen pen;
    pen.setColor(QColor(0, 0, 0));
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);//抗锯齿
    QBrush brush;
    //绘制棋盘
    drawchessboard();

    //绘制落子提示
    drawreminder(pen, brush);

    //绘制棋子
    drawchess(brush);

    //绘制输赢标志
    drawgameover();
}

void MyPainter::mouseMoveEvent(QMouseEvent *event)
{
    //获取当前坐标
    int x = event->x();
    int y = event->y();

    if (x > MARGING + BLOCK_SIZE / 2 &&
            x < gamewidgetwidth - MARGING - BLOCK_SIZE / 2 &&
            y > MARGING + BLOCK_SIZE / 2 &&
            y < gamewidgetwidth - MARGING - BLOCK_SIZE / 2)
    {
        int col = (x - MARGING) / (BLOCK_SIZE);
        int row = (y - MARGING) / (BLOCK_SIZE);

        int leftTopX = MARGING + col * BLOCK_SIZE;
        int leftTopY = MARGING + row * BLOCK_SIZE;

        //初始化最终值
        m_clickPosRow = -1;
        m_clickPosCol = -1;
        selectPos = false;

        //根据距离算出合适的点击位置
        //左上角
        len[0] = sqrtf((x-leftTopX)*(x-leftTopX) + (y-leftTopY)*(y-leftTopY));
        if ( len[0] < POS_OFFSET)
        {
            m_clickPosRow = row;
            m_clickPosCol = col;
            if (game->gameMapVec[m_clickPosRow][m_clickPosCol] == 0)
            {
                selectPos = true;
                flag = 1;
            }
        }
        //右上角
        len[1] = sqrtf((x-leftTopX-BLOCK_SIZE)*(x-leftTopX-BLOCK_SIZE) + (y-leftTopY)*(y-leftTopY));
        if ( len[1] < POS_OFFSET)
        {
            m_clickPosRow = row;
            m_clickPosCol = col + 1;
            if (game->gameMapVec[m_clickPosRow][m_clickPosCol] == 0)
            {
                selectPos = true;
                flag = 2;
            }
        }
        //左下角
        len[2] = sqrtf((x-leftTopX)*(x-leftTopX) + (y-leftTopY-BLOCK_SIZE)*(y-leftTopY-BLOCK_SIZE));
        if ( len[2] < POS_OFFSET)
        {
            m_clickPosRow = row + 1;
            m_clickPosCol = col;
            if (game->gameMapVec[m_clickPosRow][m_clickPosCol] == 0)
            {
                selectPos = true;
                flag = 3;
            }
        }
        //右下角
        len[3] = sqrtf((x-leftTopX-BLOCK_SIZE)*(x-leftTopX-BLOCK_SIZE) +
                       (y-leftTopY-BLOCK_SIZE)*(y-leftTopY-BLOCK_SIZE));
        if ( len[3] < POS_OFFSET)
        {
            m_clickPosRow = row + 1;
            m_clickPosCol = col + 1;
            if (game->gameMapVec[m_clickPosRow][m_clickPosCol] == 0)
            {
                selectPos = true;
                flag = 4;
            }
        }
    }
    //更新画面
    update();
}

void MyPainter::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(selectPos == false)
        return;
    else
        //在落子前把落子标志置为假
        selectPos = false;

    //人人下棋
    chessbypreson();

    //人机大战（AI）
    if(game->m_gameType == AI)
    {
        //AI下棋
        game->actionByAI(m_clickPosRow, m_clickPosCol);
        update();
    }

    m_score += 100;
    emit sendScore(m_score);
}

//真人落子
void MyPainter::chessbypreson()
{
    if(m_clickPosRow != -1 &&m_clickPosCol != -1 && game->gameMapVec[m_clickPosRow][m_clickPosCol] == 0 )
    {
        //在游戏的数据模型中落子
        game->actionByPerson(m_clickPosRow, m_clickPosCol);
        //播放落子音效, 待实现
        //重绘
        update();
    }
}



//绘制输赢结果
void MyPainter::drawgameover()
{
    if(m_clickPosRow > 0 && m_clickPosRow < BOARD_Num_SIZE &&
       m_clickPosCol > 0 && m_clickPosCol < BOARD_Num_SIZE &&
       (game->gameMapVec[m_clickPosRow][m_clickPosCol] == 1 ||
       game->gameMapVec[m_clickPosRow][m_clickPosCol] == -1))
    {
        if(game->isWin(m_clickPosRow, m_clickPosCol) && game->m_gameStatus == PLAYING)
        {
            QString str;
            game->setStatus(WIN);
            //可以加一些音效动画等其他操作
            if(game->gameMapVec[m_clickPosRow][m_clickPosCol] == 1)
                str = "黑子";
            if(game->gameMapVec[m_clickPosRow][m_clickPosCol] == -1)
                str = "白子";

            m_score = 0;
            emit sendScore(m_score);
            QMessageBox::StandardButton buttonval = QMessageBox::information(this, "决战五子棋", "恭喜╰(*°▽°*)╯！ " + str + "获胜！！！");
            //重置状态
            if(buttonval == QMessageBox::Ok)
            {
                game->setStatus(PLAYING);
                game->startGame(AI);
            }
        }
    }
}

void MyPainter::drawchessboard()
{
    for (int i = 0; i <= BOARD_Num_SIZE; i++)
    {
        //从左到右
        painter->drawLine(MARGING + BLOCK_SIZE * i, MARGING,
                         MARGING + BLOCK_SIZE * i,
                         gamewidgetwidth - MARGING);
        //从上到下
        painter->drawLine(MARGING, MARGING + BLOCK_SIZE * i,
                         gamewidgetwidth - MARGING,
                         MARGING  + BLOCK_SIZE * i);
    }
}

void MyPainter::drawreminder(QPen &pen, QBrush &brush)
{
    brush.setStyle(Qt::SolidPattern);
    if(m_clickPosRow > 0 && m_clickPosRow < BOARD_Num_SIZE &&
       m_clickPosCol > 0 && m_clickPosCol < BOARD_Num_SIZE &&
       game->gameMapVec[m_clickPosRow][m_clickPosCol] == 0)     //落子是否合法
    {
        if(game->m_playerFlage)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);

        pen.setBrush(brush);
        painter->drawRect(MARGING + BLOCK_SIZE * m_clickPosCol - MARK_SIZE/2,
                         MARGING + BLOCK_SIZE * m_clickPosRow - MARK_SIZE/2,
                         MARK_SIZE, MARK_SIZE);
    }

}

void MyPainter::drawchess(QBrush &brush)
{
    for (int i= 0; i < BOARD_Num_SIZE; i++)
    {
        for (int j= 0; j < BOARD_Num_SIZE; j++)
        {

            if(game->gameMapVec[i][j] == 1) {
                brush.setColor(Qt::black);
                painter->setBrush(brush);
                painter->drawEllipse(MARGING+BLOCK_SIZE*j - CHESS_RADIUS ,
                                    MARGING+BLOCK_SIZE*i - CHESS_RADIUS ,
                                    CHESS_RADIUS * 2, CHESS_RADIUS * 2);
            }
            else if(game->gameMapVec[i][j] == -1) {
                brush.setColor(Qt::white);
                painter->setBrush(brush);
                painter->drawEllipse(MARGING+BLOCK_SIZE*j - CHESS_RADIUS ,
                                    MARGING+BLOCK_SIZE*i - CHESS_RADIUS ,
                                    CHESS_RADIUS * 2, CHESS_RADIUS * 2);
            }
        }
    }
}
