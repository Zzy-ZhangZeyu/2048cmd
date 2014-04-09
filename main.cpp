#include <iostream>
#include <stdlib.h>
#include <random>
#include <math.h>
#include <time.h>
#include <windows.h>
using namespace std;
int board[4][4];
int greatest = 2;
bool quitflag = 0;
bool boardchange = 1;
mt19937 random;
uniform_int_distribution<mt19937::result_type> distribution(0,3);
int lastpressed;
HANDLE hStdout;
HWND cmdhwnd;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
void setboard(int y, int x, int val){
    board[y][x] = val;
    boardchange = 1;
}
void moveup(){
    for(int y = 3; y > 0; y--){
        for(int x = 0; x < 4; x++){
            if(board[y-1][x] == board[y][x] || board[y-1][x] == 0){
                board[y-1][x] = board[y-1][x] + board[y][x];
                if(board[y-1][x] > greatest) greatest = board[y-1][x];
                board[y][x] = 0;
                boardchange = 1;
            }
        }
    }
}
void movedown(){
    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 4; x++){
            if(board[y+1][x] == board[y][x] || board[y+1][x] == 0){
                board[y+1][x] = board[y+1][x] + board[y][x];
                board[y][x] = 0;
                if(board[y+1][x] > greatest) greatest = board[y+1][x];
                boardchange = 1;
            }
        }
    }
}
void moveright(){
    for(int x = 0; x < 3; x++){
        for(int y = 0; y<4; y++){
            if(board[y][x+1] == board[y][x] || board[y][x+1] == 0){
                board[y][x+1] = board[y][x+1] + board[y][x];
                if(board[y][x+1] > greatest) greatest = board[y][x+1];
                board[y][x] = 0;
                boardchange = 1;
            }
        }
    }
}
void moveleft(){
    for(int x = 3; x > 0; x--){
        for(int y = 0; y<4; y++){
            if(board[y][x-1] == board[y][x] || board[y][x-1] == 0){
                board[y][x-1] = board[y][x-1] + board[y][x];
                if(board[y][x-1] > greatest) greatest = board[y][x-1];
                board[y][x] = 0;
                boardchange = 1;
            }
        }
    }
}
void initgame(){
    random.seed(GetTickCount());
    lastpressed = GetTickCount();
    board[distribution(random)][distribution(random)] = 2;
    board[distribution(random)][distribution(random)] = 2;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    //get the info about the command line
    GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
    char newwindowtitle[1024];
    char oldwindowtitle[1024];
    //obtain the title of the command line
    GetConsoleTitle(oldwindowtitle, 1024);
    //replace it with one that should be unique
    wsprintf(newwindowtitle, "%d/%d", GetTickCount(), GetCurrentThreadId());
    SetConsoleTitle(newwindowtitle);
    //get the window handle by searching for the unique title
    cmdhwnd = FindWindow(NULL, newwindowtitle);
    //restore the old title
    SetConsoleTitle(oldwindowtitle);
}
void generatenum(){
    uint8_t emptyval = 1;
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(board[y][x] == 0){
                emptyval = 0;
                goto superbreak;
            }
        }
    }
    superbreak:
    if(emptyval) return;
    while(true){
        int x;
        int y;
        srand((unsigned int)GetTickCount()-1);
        x = distribution(random);
        srand((unsigned int)GetTickCount());
        y = distribution(random);
        if(board[y][x] == 0){
            srand((unsigned int) GetTickCount()+1);
            int putthere = distribution(random);
            if(putthere == 0){
                board[y][x] = 2;
            }else{
                board[y][x] = 4;
            }
            return;
        }
    }
}
void printbuffer(){
    if(boardchange){
        system("cls");
        csbiInfo.dwCursorPosition.X = 0;
        csbiInfo.dwCursorPosition.Y = 0;
        SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition);
        for(unsigned int x = 0; x < 4; x++){
            for(unsigned int y = 0; y < 4; y++){
                cout<<board[x][y]<<"    ";
            }
            cout<<endl<<endl<<endl;
        }
        boardchange = 0;
    }
}
//0x41 == A key
//0x57 == W key
//0x53 == S key
//0x44 == D key
void processkey(){
    if((GetTickCount() - lastpressed) > 125){
        if(GetKeyState(0x57) & 0x8000 || GetKeyState(VK_UP) & 0x8000){
            moveup();
        }else if(GetKeyState(0x53) & 0x8000 || GetKeyState(VK_DOWN) & 0x8000){
            movedown();
        }else if(GetKeyState(0x41) & 0x8000 || GetKeyState(VK_LEFT) & 0x8000){
            moveleft();
        }else if(GetKeyState(0x44) & 0x8000 || GetKeyState(VK_RIGHT) & 0x8000){
                moveright();
        }
        lastpressed = GetTickCount();
    }
}
void youwin(){
    system("cls");
    cout<<"congratulations, you've won!"<<endl;
    cout<<"press enter to leave the game, your free now"<<endl;
    cin.get();
    exit(0);
}
int main()
{
    cout<<"This is the command line version of 2048!"<<endl;
    cout<<"Version 1.02"<<endl;
    cout<<"combine the numbers until one adds up to 2048"<<endl;
    cout<<"Numbers of the same value combine, if the combined result can"<<endl<<"can combine again it will"<<endl;
    cout<<"use WASD to move the numbers"<<endl;
    cout<<"press enter to start"<<endl;
    cin.get();
    system("cls");
    initgame();
    while(!quitflag){
        printbuffer();
        processkey();
        if(greatest == 2048) youwin();
        if(boardchange) generatenum();
        Sleep(33);
    }
    return 0;
}
