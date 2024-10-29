#include<stdio.h>
#include<stdbool.h>
#include<Windows.h>

#define DINO_BOTTOM_Y 17
#define CACTUS_BOTTOM_Y 25
#define CACTUS_BOTTOM_X 45
#define BUFFER_WIDTH 120
#define BUFFER_HEIGHT 30

// 버퍼 2개 준비
static char front_buffer[BUFFER_HEIGHT][BUFFER_WIDTH];
static char back_buffer[BUFFER_HEIGHT][BUFFER_WIDTH];

void InitBuffer();
void WriteBuffer(int x, int y, const char* str);
void FlipBuffer();
void ClearBuffer();
void CursorView(char show);
void SetConsoleView();
void DrawDino(int dinoY);
void GotoXY(int x, int y);
void DrawCactus(int cactusX);

int main() {
    SetConsoleView();
    InitBuffer();

    while (true) {
        int dinoY = DINO_BOTTOM_Y;
        int cactusX = CACTUS_BOTTOM_X;

        ClearBuffer();
        DrawDino(dinoY);
        DrawCactus(cactusX);
        FlipBuffer();

        Sleep(60);
    }
    return 0;
}

void InitBuffer() {
    for (int i = 0; i < BUFFER_HEIGHT; i++) {
        for (int j = 0; j < BUFFER_WIDTH; j++) {
            front_buffer[i][j] = back_buffer[i][j] = ' ';
        }
    }
}

void WriteBuffer(int x, int y, const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        back_buffer[y][x + i] = str[i];
    }
}

void FlipBuffer() {
    for (int i = 0; i < BUFFER_HEIGHT; i++) {
        COORD pos = { 0, i };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        for (int j = 0; j < BUFFER_WIDTH; j++) {
            if (front_buffer[i][j] != back_buffer[i][j]) {
                printf("%c", back_buffer[i][j]);
                front_buffer[i][j] = back_buffer[i][j];
            }
            else {
                printf("%c", front_buffer[i][j]);
            }
        }
    }
}

void ClearBuffer() {
    for (int i = 0; i < BUFFER_HEIGHT; i++) {
        for (int j = 0; j < BUFFER_WIDTH; j++) {
            back_buffer[i][j] = ' ';
        }
    }
}

void DrawCactus(int cactusX) {
    WriteBuffer(cactusX * 2, CACTUS_BOTTOM_Y, "  $");
    WriteBuffer(cactusX * 2, CACTUS_BOTTOM_Y + 1, "$ $  ");
    WriteBuffer(cactusX * 2, CACTUS_BOTTOM_Y + 2, "$ $ $");
    WriteBuffer(cactusX * 2, CACTUS_BOTTOM_Y + 3, "$$$$$");
    WriteBuffer(cactusX * 2, CACTUS_BOTTOM_Y + 4, "   $  ");
}

void DrawDino(int dinoY) {
    static int legFlag = true;

    WriteBuffer(0, dinoY, "        $$$$$$$ ");
    WriteBuffer(0, dinoY + 1, "       $$ $$$$$$");
    WriteBuffer(0, dinoY + 2, "       $$$$$$$$$");
    WriteBuffer(0, dinoY + 3, "$      $$$      ");
    WriteBuffer(0, dinoY + 4, "$$     $$$$$$$  ");
    WriteBuffer(0, dinoY + 5, "$$$   $$$$$     ");
    WriteBuffer(0, dinoY + 6, " $$  $$$$$$$$$$ ");
    WriteBuffer(0, dinoY + 7, " $$원광대$$$    ");
    WriteBuffer(0, dinoY + 8, "  $$$컴소공$    ");
    WriteBuffer(0, dinoY + 9, "    $$$$$$$$    ");
    WriteBuffer(0, dinoY + 10, "     $$$$$$     ");

    if (legFlag) {
        WriteBuffer(0, dinoY + 11, "     $    $$$   ");
        WriteBuffer(0, dinoY + 12, "     $$         ");
        legFlag = false;
    }
    else {
        WriteBuffer(0, dinoY + 11, "     $$$  $     ");
        WriteBuffer(0, dinoY + 12, "          $$    ");
        legFlag = true;
    }
}

void GotoXY(int x, int y) {
    COORD Pos;
    Pos.X = 2 * x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void CursorView(char show) {
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void SetConsoleView() {
    system("mode con:cols=120 lines=30");
    system("title 창의실무프로젝트 구글 공룡 게임 [by. iksang J]");
    CursorView(false);
}