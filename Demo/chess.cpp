#define _CRT_SECURE_NO_DEPRECATE
#include <bits/stdc++.h>
using namespace std;
enum chessPicesStatus { empty, black, white }; //empty 為0 black 為1 white 為2
#include <iostream>
#include <fstream>
#include <windows.h>

#define SIDE_LENGTH 4

using std::cout;
using std::ofstream;
using std::endl;
using std::string;
using std::cerr;
using std::fstream;
using std::to_string;

string text("");//要存進TXT的大string

struct PicesPos { //存棋盤位置的狀態
    int x, y, z, value;
    chessPicesStatus type;
};
struct flatData {
    int a, b, c, d;
};
struct tar
{
    int x, y, z;
};
typedef vector<PicesPos> PicesPosList;
typedef vector<flatData> PicesFlatDataList;

void setcolor(int color)
{
    HANDLE
    hc = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hc,0|color);
}

void ShellSort(PicesPosList &nodes, int n, vector<int> incrmnt, int numinc)
{
    int incr, i, j, span;
    PicesPos tmp;
    for(incr = 0;incr < numinc; incr++)
    {
        //cout<<"1"<<endl;
        span = incrmnt[incr];
        for(i = span;i < n;i++)
        {
            //cout<<"2"<<endl;
            tmp = nodes[i];
            for(j = i-span; (j >= 0)&&(tmp.value > nodes[j].value); j=j-span)
            {
                //cout<<"3"<<endl;
                nodes[j+span] = nodes[j];
            }
            nodes[j+span] = tmp;

        }

    }
    //return nodes;
}
bool comp(const PicesPos& A, const PicesPos& B) //比較
{
    return A.value > B.value;
}
class ChessBoard
{
public:
    tar usertargetPos, whiteTargetPos;
    int chessBoard[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH];
    void init();
    void printBoard();
    bool valid(int x, int y, int type); //下棋
    bool insertPices(int x, int y, int type); //下棋
    //搜索
    int dfs(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep);
    //遊戲結束判斷
    int isWin(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH]);
    //求每邊評分
    int getSideScore(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], chessPicesStatus side);
    //求平面評分
    int getFlatPicesValue(PicesFlatDataList flats, chessPicesStatus status);
    //求此點分數
    int getPosValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], PicesPos* pos, chessPicesStatus side);
    int findWhiteScoreMaxValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep, int alpha, int beta);
    int findWhiteScoreMinValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep, int alpha, int beta);
    PicesPosList getAvailablePos(int boardboard[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], chessPicesStatus side);
};
bool ChessBoard::valid(int x, int y, int type) //確認位置是否為空的 且沒超出範圍
{
    if (x < 0 || x >3 || y < 0 || y > 3)
        return false;
    //check 0-3th floor
    usertargetPos.x = x;
    usertargetPos.y = y;
    for (int z = 0; z < SIDE_LENGTH; z++)
    {
        if (chessBoard[x][y][z] == chessPicesStatus::empty)
        {
            usertargetPos.z = z;
            text += "user inserted x = "; text += to_string(x);
            text += " y = "; text += to_string(y);
            text += " z = "; text += to_string(z);
            text += "\n";
            return true;
        }
    }
    return false;
}
bool ChessBoard::insertPices(int x, int y, int type) //確認位置是否為空的 且沒超出範圍
{
    if (x < 0 || x >3 || y < 0 || y > 3)
        return false;
    //check 0-3th floor
    for (int z = 0; z < SIDE_LENGTH; z++)
    {
        if (chessBoard[x][y][z] == chessPicesStatus::empty)
        {
            chessBoard[x][y][z] = chessPicesStatus(type);
            return true;
        }
    }

    return false;
}
void ChessBoard::init() //初始化棋盤為empty
{
    for (int z = 0; z < SIDE_LENGTH; z++)
    {
        for (int y = 0; y < SIDE_LENGTH; y++)
        {
            for (int x = 0; x < SIDE_LENGTH; x++)
            {
                chessBoard[x][y][z] = chessPicesStatus::empty;
            }
        }
    }
}
void ChessBoard::printBoard() //印出棋盤
{
    for (int z = 0; z < SIDE_LENGTH ; z++)
    {
        cout << "The " << z << "th floor" << endl;
        for (int y = 0; y < SIDE_LENGTH; y++)
        {
            for (int x = 0; x < SIDE_LENGTH; x++)
            {
                if(chessBoard[x][y][z] == 1) setcolor(4);
                else if(chessBoard[x][y][z] == 2) setcolor(1);
                else setcolor(7);
                cout << chessBoard[x][y][z] << " ";
                setcolor(7);
            }
            cout << endl;
        }
        cout << "\\\\\\\\\\\\\\" << endl;
    }
}
int ChessBoard::dfs(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep)
{
    //cout<<INT_MAX<<" and "<<INT_MIN<<endl;
    return findWhiteScoreMaxValue(board, deep, INT_MAX, INT_MIN);
}
bool ifSimpleFlat(PicesPos pos)
{
    if (pos.x == 1 && pos.y == 0 ||
        pos.x == 2 && pos.y == 0 ||
        pos.x == 0 && pos.y == 1 ||
        pos.x == 3 && pos.y == 1 ||
        pos.x == 0 && pos.y == 2 ||
        pos.x == 3 && pos.y == 2 ||
        pos.x == 1 && pos.y == 3 ||
        pos.x == 2 && pos.y == 3)
        return true; //true 的話代表那個位置不可能有斜的線
    //之後分析就只要考慮該平面就好
    else
        return false;
    //false 代表有可能有斜的
//斜的跟平面都要考量
}
int ChessBoard::isWin(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH]) //return 值為 0 代表 沒人獲勝 1代表黑色獲勝 2代表白色獲勝
{
    PicesFlatDataList flats;

    //總共會存16個平行X軸的column
    for (int y = 0; y < SIDE_LENGTH; y++)
    {
        for (int z = 0; z < SIDE_LENGTH; z++)
        {
            flatData data;
            data.a = board[0][y][z];
            data.b = board[1][y][z];
            data.c = board[2][y][z];
            data.d = board[3][y][z];
            flats.push_back(data);
        }
    }
    //總共會存16個平行Y軸的column
    for (int x = 0; x < SIDE_LENGTH; x++)
    {
        for (int z = 0; z < SIDE_LENGTH; z++)
        {
            flatData data;
            data.a = board[x][0][z];
            data.b = board[x][1][z];
            data.c = board[x][2][z];
            data.d = board[x][3][z];
            flats.push_back(data);
        }
    }
    //總共會存16個平行Z軸的column
    for (int x = 0; x < SIDE_LENGTH; x++)
    {
        for (int y = 0; y < SIDE_LENGTH; y++)
        {
            flatData data;
            data.a = board[x][y][0];
            data.b = board[x][y][1];
            data.c = board[x][y][2];
            data.d = board[x][y][3];
            flats.push_back(data);
        }
    }
    //平行於XZ平面上的對角線
    for (int y = 0; y < SIDE_LENGTH; y++)
    {
        flatData data;
        data.a = board[0][y][0];
        data.b = board[1][y][1];
        data.c = board[2][y][2];
        data.d = board[3][y][3];
        flats.push_back(data);
    }
    //平行於YZ平面上的對角線
    for (int x = 0; x < SIDE_LENGTH; x++)
    {
        flatData data;
        data.a = board[x][0][0];
        data.b = board[x][1][1];
        data.c = board[x][2][2];
        data.d = board[x][3][3];
        flats.push_back(data);
    }
    //平行於XZ平面上的另一個方向的對角線
    for (int y = 0; y < SIDE_LENGTH; y++)
    {
        flatData data;
        data.a = board[0][y][3];
        data.b = board[1][y][2];
        data.c = board[2][y][1];
        data.d = board[3][y][0];
        flats.push_back(data);
    }
    //平行於YZ平面上的另一個方向的對角線
    for (int x = 0; x < SIDE_LENGTH; x++)
    {
        flatData data;
        data.a = board[x][0][3];
        data.b = board[x][1][2];
        data.c = board[x][2][1];
        data.d = board[x][3][0];
        flats.push_back(data);
    }
    //平行於XY平面上的對角線
    for (int z = 0; z < SIDE_LENGTH; z++)
    {
        flatData data;
        data.a = board[0][0][z];
        data.b = board[1][1][z];
        data.c = board[2][2][z];
        data.d = board[3][3][z];
        flats.push_back(data);
    }
    //平行於XY平面上的另一個方向的對角線
    for (int z = 0; z < 4; z++)
    {
        flatData data;
        data.a = board[3][0][z];
        data.b = board[2][1][z];
        data.c = board[1][2][z];
        data.d = board[0][3][z];
        flats.push_back(data);
    }

    //存立方體的四條對頂點線
    flatData data;
    data.a = board[0][0][0];
    data.b = board[1][1][1];
    data.c = board[2][2][2];
    data.d = board[3][3][3];
    flats.push_back(data);

    data.a = board[0][0][3];
    data.b = board[1][1][2];
    data.c = board[2][2][1];
    data.d = board[3][3][0];
    flats.push_back(data);

    data.a = board[3][0][0];
    data.b = board[2][1][1];
    data.c = board[1][2][2];
    data.d = board[0][3][3];
    flats.push_back(data);

    data.a = board[3][0][3];
    data.b = board[2][1][2];
    data.c = board[1][2][1];
    data.d = board[0][3][0];
    flats.push_back(data);



    int whiteNum = 0, blackNum = 0;

    for (auto iter = flats.begin(); iter != flats.end(); iter++)
    {
        whiteNum = 0;
        blackNum = 0;

        if (iter->a == chessPicesStatus::black)
            blackNum++;
        else if (iter->a == chessPicesStatus::white)
            whiteNum++;

        if (iter->b == chessPicesStatus::black)
            blackNum++;
        else if (iter->b == chessPicesStatus::white)
            whiteNum++;

        if (iter->c == chessPicesStatus::black)
            blackNum++;
        else if (iter->c == chessPicesStatus::white)
            whiteNum++;

        if (iter->d == chessPicesStatus::black)
            blackNum++;
        else if (iter->d == chessPicesStatus::white)
            whiteNum++;
        //如果一條線上有4科一樣顏色的子就代表那方贏了
        if (whiteNum == SIDE_LENGTH)
        {
            printBoard();
            text += "\n******AI Win!******\n";
            cout << "******Blue Win!******" << endl;
            return chessPicesStatus::white;
        }
        if (blackNum == SIDE_LENGTH)
        {
            printBoard();
            text += "\n******User Win!******\n";
            cout << "******Red Win!******" << endl;
            return chessPicesStatus::black;
        }
    }
    printBoard();
    return chessPicesStatus::empty;
}
int ChessBoard::getFlatPicesValue(PicesFlatDataList flats, chessPicesStatus status)
{
    //為每條線賦予一個價值 來做為之後要下哪裡比較好的比較依據
    int value = 0, whiteNum = 0, blackNum = 0;
    for (auto iter = flats.begin(); iter != flats.end(); iter++)
    {
        whiteNum = 0;
        blackNum = 0;
        if (iter->a == chessPicesStatus::black)
            blackNum++;
        else if (iter->a == chessPicesStatus::white)
            whiteNum++;
        if (iter->b == chessPicesStatus::black)
            blackNum++;
        else if (iter->b == chessPicesStatus::white)
            whiteNum++;
        if (iter->c == chessPicesStatus::black)
            blackNum++;
        else if (iter->c == chessPicesStatus::white)
            whiteNum++;
        if (iter->d == chessPicesStatus::black)
            blackNum++;
        else if (iter->d == chessPicesStatus::white)
            whiteNum++;
        if (status == chessPicesStatus::white)
        {
            //Calculating White Picess
            if (blackNum == 3)
                value += 300000;
            if (blackNum != 0)
                continue;
            if (whiteNum == 0)
                value += 1;
            else if (whiteNum == 1)
                value += 5;
            else if (whiteNum == 2)
                value += 100;
            else if (whiteNum == 3)
                value += 5000;
            else if (whiteNum == 4)
                value += 100000;
        }
        else
        {
            //Calculating Black Picess

            if (whiteNum != 0)
                continue;
            if (blackNum == 0)
                value += 100000;
            else if (blackNum == 1)
                value += 5000;
            else if (blackNum == 2)
                value += 100;
            else if (blackNum == 3)
                value += 5;
            else if (blackNum == 4)
                value += 100000;
        }

        //cout<<iter->a<<" "<<iter->b<<" "<<iter->c<<" "<<iter->d<<" "<<value<<endl;
    }
    //cout << value<<endl;
    return value;
}
int ChessBoard::getPosValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], PicesPos* pos, chessPicesStatus side)
{
    PicesFlatDataList flatList;
    if (ifSimpleFlat(*pos)) //return 為true 代表不會有斜的線 考慮平面就好
    {
        flatData temp;
        temp.a = board[0][pos->y][pos->z];
        temp.b = board[1][pos->y][pos->z];
        temp.c = board[2][pos->y][pos->z];
        temp.d = board[3][pos->y][pos->z];
        flatList.push_back(temp);

        temp.a = board[pos->x][0][pos->z];
        temp.b = board[pos->x][1][pos->z];
        temp.c = board[pos->x][2][pos->z];
        temp.d = board[pos->x][3][pos->z];
        flatList.push_back(temp);

        temp.a = board[0][pos->y][0];
        temp.b = board[1][pos->y][1];
        temp.c = board[2][pos->y][2];
        temp.d = board[3][pos->y][3];
        flatList.push_back(temp);

        temp.a = board[0][pos->y][3];
        temp.b = board[1][pos->y][2];
        temp.c = board[2][pos->y][1];
        temp.d = board[3][pos->y][0];
        flatList.push_back(temp);

        temp.a = board[pos->x][0][0];
        temp.b = board[pos->x][1][1];
        temp.c = board[pos->x][2][2];
        temp.d = board[pos->x][3][3];
        flatList.push_back(temp);

        temp.a = board[pos->x][0][3];
        temp.b = board[pos->x][1][2];
        temp.c = board[pos->x][2][1];
        temp.d = board[pos->x][3][0];
        flatList.push_back(temp);

        temp.a = board[pos->x][pos->y][0];
        temp.b = board[pos->x][pos->y][1];
        temp.c = board[pos->x][pos->y][2];
        temp.d = board[pos->x][pos->y][3];
        flatList.push_back(temp);

        temp.a = board[0][0][pos->z];
        temp.b = board[1][1][pos->z];
        temp.c = board[2][2][pos->z];
        temp.d = board[3][3][pos->z];
        flatList.push_back(temp);

        temp.a = board[3][0][pos->z];
        temp.b = board[2][1][pos->z];
        temp.c = board[1][2][pos->z];
        temp.d = board[0][3][pos->z];
        flatList.push_back(temp);
    }
    else//return 為false 代表可能有斜的線
    {
        flatData temp;
        temp.a = board[0][pos->y][pos->z];
        temp.b = board[1][pos->y][pos->z];
        temp.c = board[2][pos->y][pos->z];
        temp.d = board[3][pos->y][pos->z];
        flatList.push_back(temp);

        temp.a = board[pos->x][0][pos->z];
        temp.b = board[pos->x][1][pos->z];
        temp.c = board[pos->x][2][pos->z];
        temp.d = board[pos->x][3][pos->z];
        flatList.push_back(temp);

        temp.a = board[0][pos->y][0];
        temp.b = board[1][pos->y][1];
        temp.c = board[2][pos->y][2];
        temp.d = board[3][pos->y][3];
        flatList.push_back(temp);

        temp.a = board[0][pos->y][3];
        temp.b = board[1][pos->y][2];
        temp.c = board[2][pos->y][1];
        temp.d = board[3][pos->y][0];
        flatList.push_back(temp);

        temp.a = board[pos->x][0][0];
        temp.b = board[pos->x][1][1];
        temp.c = board[pos->x][2][2];
        temp.d = board[pos->x][3][3];
        flatList.push_back(temp);

        temp.a = board[pos->x][0][3];
        temp.b = board[pos->x][1][2];
        temp.c = board[pos->x][2][1];
        temp.d = board[pos->x][3][0];
        flatList.push_back(temp);

        temp.a = board[pos->x][pos->y][0];
        temp.b = board[pos->x][pos->y][1];
        temp.c = board[pos->x][pos->y][2];
        temp.d = board[pos->x][pos->y][3];
        flatList.push_back(temp);

        temp.a = board[0][0][pos->z];
        temp.b = board[1][1][pos->z];
        temp.c = board[2][2][pos->z];
        temp.d = board[3][3][pos->z];
        flatList.push_back(temp);

        temp.a = board[3][0][pos->z];
        temp.b = board[2][1][pos->z];
        temp.c = board[1][2][pos->z];
        temp.d = board[0][3][pos->z];
        flatList.push_back(temp);

        if (pos->x == 0 && pos->y == 0 ||
            pos->x == 1 && pos->y == 1 ||
            pos->x == 2 && pos->y == 2 ||
            pos->x == 3 && pos->y == 3)
        {
            flatData temp;
            temp.a = board[0][0][0];
            temp.b = board[1][1][1];
            temp.c = board[2][2][2];
            temp.d = board[3][3][3];
            flatList.push_back(temp);

            temp.a = board[0][0][3];
            temp.b = board[1][1][2];
            temp.c = board[2][2][1];
            temp.d = board[3][3][0];
            flatList.push_back(temp);
        }
        else
        {
            flatData temp;
            temp.a = board[3][0][0];
            temp.b = board[2][1][1];
            temp.c = board[1][2][2];
            temp.d = board[0][3][3];
            flatList.push_back(temp);

            temp.a = board[3][0][3];
            temp.b = board[2][1][2];
            temp.c = board[1][2][1];
            temp.d = board[0][3][0];
            flatList.push_back(temp);
        }

    }
    int val = getFlatPicesValue(flatList, side);

    //比對先前賦予每條線的狀態 所對應的價值
    return val;
}
int ChessBoard::getSideScore(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], chessPicesStatus side)//回傳 side方的分數
{
    return 0;
}
int ChessBoard::findWhiteScoreMaxValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep, int alpha, int beta) //考慮電腦為白方   deep為預測下棋的數量
//現在在max層 要找max節點
//自己找價值最大的節點
//α表示搜索到的最好的值，β表示搜索到的最坏的值。
{
    if (deep <= 0)
        return (getSideScore(board, chessPicesStatus::white) - getSideScore(board, chessPicesStatus::black));
    else
    {
        int maxVal = -1000000;
        PicesPosList nodes = getAvailablePos(board, chessPicesStatus::white); // 找遍整個立方體的empty點 並存有每一點的價值
        //sort(nodes.begin(), nodes.end(), comp); // 照價值排序 由大到小
        //
        vector<int> incrmnt = {5, 3, 1};
        int numinc = 3;
        /*
        for(auto iter = nodes.begin(); iter != nodes.end(); iter++)
        {
            cout<<iter->value<<" ";
        }
        cout<<endl;
        */
        int n = nodes.size();
        ShellSort(nodes, n, incrmnt, numinc);
        /*
        for(auto iter = nodes.begin(); iter != nodes.end(); iter++)
        {
            cout<<iter->value<<" ";
        }
        cout<<endl;
        */
        //
        for (auto iter = nodes.begin(); iter != nodes.end(); iter++)
        {
            board[iter->x][iter->y][iter->z] = chessPicesStatus::white;//選一個價值最高的白子下
            int val;
            if (maxVal > beta)
                val = findWhiteScoreMinValue(board, deep - 1, alpha, maxVal); //假設使用者選價值最小的節點
            else
                val = findWhiteScoreMinValue(board, deep - 1, alpha, beta);   //假設使用者選價值最小的節點
            if (val > maxVal) //比之前的白子選擇好
            {
                maxVal = val;
                whiteTargetPos.x = iter->x;
                whiteTargetPos.y = iter->y;
                whiteTargetPos.z = iter->z;
            }
            board[iter->x][iter->y][iter->z] = chessPicesStatus::empty;
            if (maxVal > alpha) //alpha 剪枝
                break;
        }
        return maxVal;
    }
}
int ChessBoard::findWhiteScoreMinValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep, int alpha, int beta)
{
    if (deep <= 0)

    {
        int score = getSideScore(board, chessPicesStatus::white) -
            getSideScore(board, chessPicesStatus::black);
        return score;
    }
    else
    {
        tar blackTargetPos;
        int minVal = 1000000;
        PicesPosList list = getAvailablePos(board, chessPicesStatus::black);
        for (auto iter = list.begin(); iter != list.end(); iter++)
        {
            board[iter->x][iter->y][iter->z] = chessPicesStatus::black;
            int val;
            if (minVal < alpha)
                val = findWhiteScoreMaxValue(board, deep - 1, minVal, beta);
            else
                val = findWhiteScoreMaxValue(board, deep - 1, alpha, beta);
            if (val < minVal)
            {
                minVal = val;
                blackTargetPos.x = iter->x;
                blackTargetPos.y = iter->y;
                blackTargetPos.z = iter->z;
            }
            board[iter->x][iter->y][iter->z] = chessPicesStatus::empty;
            if (val < beta)
                break;
        }
        return minVal;
    }
}
PicesPosList ChessBoard::getAvailablePos(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], chessPicesStatus side)
{
    PicesPosList availablePos;

    for (int y = 0; y < SIDE_LENGTH; y++)
    {
        for (int x = 0; x < SIDE_LENGTH; x++)
        {
            for (int z = 0; z < SIDE_LENGTH; z++)
            {
                if (board[x][y][z] == chessPicesStatus::empty)
                {
                    PicesPos pos;
                    pos.x = x;
                    pos.y = y;
                    pos.z = z;
                    pos.type = chessPicesStatus::empty;
                    pos.value = getPosValue(board, &pos, side);
                    availablePos.push_back(pos);
                    break;
                }
            }
        }
    }

    return availablePos;
}


int main()
{
    ChessBoard cb;
    cb.init();
    cb.printBoard();
    int x, y;
    cout << "please enter the location \n";
    while (cin >> x) //使用者為黑色方
    {
        if (x == -1)//escape
        {
            break;
        }
        cin >> y;

        if (cb.valid(x, y, chessPicesStatus::black) == false) //確定使用者要下的位置能下
        {
            cout << "error\n";
            continue;
        }
        cb.insertPices(x, y, chessPicesStatus::black); //把旗下下去 同時更新usertargetPos 的xyz
        int buf;
        buf = cb.isWin(cb.chessBoard);
        if (buf == 1 || buf == 2)
            break;
        cb.dfs(cb.chessBoard, 4);    //set the deep
        cout << endl << "*******Insert x:" << cb.usertargetPos.x << " y:" << cb.usertargetPos.y << " z:" << cb.usertargetPos.z << "********" << endl << endl;
        cb.insertPices(cb.whiteTargetPos.x, cb.whiteTargetPos.y, chessPicesStatus::white);//DFS裡會找出最好的選項然後設為target pos  這行再真的下下去
        //int whiteScore = cb.getSideScore(cb.chessBoard, chessPicesStatus::white);
        //int blackScore = cb.getSideScore(cb.chessBoard, chessPicesStatus::black);
        //cout << "whiteScore:" << whiteScore << " blackScore:" << blackScore << endl;
        //cb.printBoard();
        cout << "whiteTargetPos x:" << cb.whiteTargetPos.x << " y:" << cb.whiteTargetPos.y << " z:" << cb.whiteTargetPos.z << endl;
        text += "AI   inserted x = "; text += to_string(cb.whiteTargetPos.x);
        text += " y = "; text += to_string(cb.whiteTargetPos.y);
        text += " z = "; text += to_string(cb.whiteTargetPos.z);
        text += "\n";

        //cout << "blackTargetPos x:" << cb.blackTargetPos.x << " y:" << cb.blackTargetPos.y <<" z:" << cb.blackTargetPos.z << endl;
        cb.isWin(cb.chessBoard);
    }

    text += "\nend\n";
    fstream outfile;
    string filename3("chess_step.txt");//TXT檔名
    FILE* o_file = fopen(filename3.c_str(), "w+");
    if (o_file)
        fwrite(text.c_str(), 1, text.size(), o_file);
    else cout<<"hhhhhhhhhhhhhhh"<<endl;
    return  0;
}
