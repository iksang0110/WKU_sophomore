//console에 랜덤 별 찍기
#include <stdio.h>
#include <windows.h>

void gotoxy(int x, int y)
{
    COORD Pos;
    Pos.X = x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void CursorView(char show)
{
   
    CONSOLE_CURSOR_INFO ConsoleCursor;

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

int main(void){
    printf("Enter 입력 시 시작 : "); getchar();
    CursorView(0);
    srand((unsigned)time(NULL));
    while (1) {
        gotoxy(rand() % 300, rand() % 25);
        printf("*");
        Sleep(100);
    }
}