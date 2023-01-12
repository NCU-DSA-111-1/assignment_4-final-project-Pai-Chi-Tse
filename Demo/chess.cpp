#define _CRT_SECURE_NO_DEPRECATE
#include <bits/stdc++.h>
using namespace std;
enum chessPicesStatus { empty, black, white }; //empty ��0 black ��1 white ��2
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

string text("");//�n�s�iTXT���jstring

struct PicesPos { //�s�ѽL��m�����A
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
bool comp(const PicesPos& A, const PicesPos& B) //���
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
    bool valid(int x, int y, int type); //�U��
    bool insertPices(int x, int y, int type); //�U��
    //�j��
    int dfs(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep);
    //�C�������P�_
    int isWin(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH]);
    //�D�C�����
    int getSideScore(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], chessPicesStatus side);
    //�D��������
    int getFlatPicesValue(PicesFlatDataList flats, chessPicesStatus status);
    //�D���I����
    int getPosValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], PicesPos* pos, chessPicesStatus side);
    int findWhiteScoreMaxValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep, int alpha, int beta);
    int findWhiteScoreMinValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep, int alpha, int beta);
    PicesPosList getAvailablePos(int boardboard[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], chessPicesStatus side);
};
bool ChessBoard::valid(int x, int y, int type) //�T�{��m�O�_���Ū� �B�S�W�X�d��
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
bool ChessBoard::insertPices(int x, int y, int type) //�T�{��m�O�_���Ū� �B�S�W�X�d��
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
void ChessBoard::init() //��l�ƴѽL��empty
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
void ChessBoard::printBoard() //�L�X�ѽL
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
        return true; //true ���ܥN���Ӧ�m���i�঳�ת��u
    //������R�N�u�n�Ҽ{�ӥ����N�n
    else
        return false;
    //false �N���i�঳�ת�
//�ת��򥭭����n�Ҷq
}
int ChessBoard::isWin(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH]) //return �Ȭ� 0 �N�� �S�H��� 1�N��¦���� 2�N��զ����
{
    PicesFlatDataList flats;

    //�`�@�|�s16�ӥ���X�b��column
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
    //�`�@�|�s16�ӥ���Y�b��column
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
    //�`�@�|�s16�ӥ���Z�b��column
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
    //�����XZ�����W���﨤�u
    for (int y = 0; y < SIDE_LENGTH; y++)
    {
        flatData data;
        data.a = board[0][y][0];
        data.b = board[1][y][1];
        data.c = board[2][y][2];
        data.d = board[3][y][3];
        flats.push_back(data);
    }
    //�����YZ�����W���﨤�u
    for (int x = 0; x < SIDE_LENGTH; x++)
    {
        flatData data;
        data.a = board[x][0][0];
        data.b = board[x][1][1];
        data.c = board[x][2][2];
        data.d = board[x][3][3];
        flats.push_back(data);
    }
    //�����XZ�����W���t�@�Ӥ�V���﨤�u
    for (int y = 0; y < SIDE_LENGTH; y++)
    {
        flatData data;
        data.a = board[0][y][3];
        data.b = board[1][y][2];
        data.c = board[2][y][1];
        data.d = board[3][y][0];
        flats.push_back(data);
    }
    //�����YZ�����W���t�@�Ӥ�V���﨤�u
    for (int x = 0; x < SIDE_LENGTH; x++)
    {
        flatData data;
        data.a = board[x][0][3];
        data.b = board[x][1][2];
        data.c = board[x][2][1];
        data.d = board[x][3][0];
        flats.push_back(data);
    }
    //�����XY�����W���﨤�u
    for (int z = 0; z < SIDE_LENGTH; z++)
    {
        flatData data;
        data.a = board[0][0][z];
        data.b = board[1][1][z];
        data.c = board[2][2][z];
        data.d = board[3][3][z];
        flats.push_back(data);
    }
    //�����XY�����W���t�@�Ӥ�V���﨤�u
    for (int z = 0; z < 4; z++)
    {
        flatData data;
        data.a = board[3][0][z];
        data.b = board[2][1][z];
        data.c = board[1][2][z];
        data.d = board[0][3][z];
        flats.push_back(data);
    }

    //�s�ߤ��骺�|���ﳻ�I�u
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
        //�p�G�@���u�W��4��@���C�⪺�l�N�N����Ĺ�F
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
    //���C���u�ᤩ�@�ӻ��� �Ӱ�������n�U���̤���n������̾�
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
    if (ifSimpleFlat(*pos)) //return ��true �N���|���ת��u �Ҽ{�����N�n
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
    else//return ��false �N��i�঳�ת��u
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

    //�����e�ᤩ�C���u�����A �ҹ���������
    return val;
}
int ChessBoard::getSideScore(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], chessPicesStatus side)//�^�� side�誺����
{
    return 0;
}
int ChessBoard::findWhiteScoreMaxValue(int board[SIDE_LENGTH][SIDE_LENGTH][SIDE_LENGTH], int deep, int alpha, int beta) //�Ҽ{�q�����դ�   deep���w���U�Ѫ��ƶq
//�{�b�bmax�h �n��max�`�I
//�ۤv����ȳ̤j���`�I
//�\��ܷj���쪺�̦n���ȡA�]��ܷj���쪺�̧����ȡC
{
    if (deep <= 0)
        return (getSideScore(board, chessPicesStatus::white) - getSideScore(board, chessPicesStatus::black));
    else
    {
        int maxVal = -1000000;
        PicesPosList nodes = getAvailablePos(board, chessPicesStatus::white); // ��M��ӥߤ��骺empty�I �æs���C�@�I������
        //sort(nodes.begin(), nodes.end(), comp); // �ӻ��ȱƧ� �Ѥj��p
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
            board[iter->x][iter->y][iter->z] = chessPicesStatus::white;//��@�ӻ��ȳ̰����դl�U
            int val;
            if (maxVal > beta)
                val = findWhiteScoreMinValue(board, deep - 1, alpha, maxVal); //���]�ϥΪ̿���ȳ̤p���`�I
            else
                val = findWhiteScoreMinValue(board, deep - 1, alpha, beta);   //���]�ϥΪ̿���ȳ̤p���`�I
            if (val > maxVal) //�񤧫e���դl��ܦn
            {
                maxVal = val;
                whiteTargetPos.x = iter->x;
                whiteTargetPos.y = iter->y;
                whiteTargetPos.z = iter->z;
            }
            board[iter->x][iter->y][iter->z] = chessPicesStatus::empty;
            if (maxVal > alpha) //alpha �ŪK
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
    while (cin >> x) //�ϥΪ̬��¦��
    {
        if (x == -1)//escape
        {
            break;
        }
        cin >> y;

        if (cb.valid(x, y, chessPicesStatus::black) == false) //�T�w�ϥΪ̭n�U����m��U
        {
            cout << "error\n";
            continue;
        }
        cb.insertPices(x, y, chessPicesStatus::black); //��X�U�U�h �P�ɧ�susertargetPos ��xyz
        int buf;
        buf = cb.isWin(cb.chessBoard);
        if (buf == 1 || buf == 2)
            break;
        cb.dfs(cb.chessBoard, 4);    //set the deep
        cout << endl << "*******Insert x:" << cb.usertargetPos.x << " y:" << cb.usertargetPos.y << " z:" << cb.usertargetPos.z << "********" << endl << endl;
        cb.insertPices(cb.whiteTargetPos.x, cb.whiteTargetPos.y, chessPicesStatus::white);//DFS�̷|��X�̦n���ﶵ�M��]��target pos  �o��A�u���U�U�h
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
    string filename3("chess_step.txt");//TXT�ɦW
    FILE* o_file = fopen(filename3.c_str(), "w+");
    if (o_file)
        fwrite(text.c_str(), 1, text.size(), o_file);
    else cout<<"hhhhhhhhhhhhhhh"<<endl;
    return  0;
}
