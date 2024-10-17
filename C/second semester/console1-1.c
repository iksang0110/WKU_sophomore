#include <stdio.h>
#include <windows.h>

int main(void)
{
    COORD Pos;
    Pos.X = 20;
    Pos.Y = 20;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
    printf("Hello, World!");
}