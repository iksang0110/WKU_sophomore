#include<stdio.h>
#include<stdbool.h>
#include<windows.h>
#include<time.h>
#include<string.h>

#define DINO_BOTTOM_Y 17
#define CACTUS_BOTTOM_Y 25
#define CACTUS_BOTTOM_X 45
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 30

// 색상 정의
enum {
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_SKYBLUE,
    DARK_RED,
    DARK_VOILET,
    DAKR_YELLOW,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    SKYBLUE,
    RED,
    VIOLET,
    YELLOW,
    WHITE,
};

// 전역 버퍼 선언
static char screen[2][CONSOLE_HEIGHT][CONSOLE_WIDTH];
static int currentBuffer = 0;

// 함수 선언
void InitScreen();
void WriteBuffer(int x, int y, const char* str);
void FlipBuffer();
void DrawDino(int dinoY);
void DrawCactus(int cactusX);
void SetColor(unsigned short text);
void GotoXY(int x, int y);
int GetKeyDown();
int IsCollision(const int cactusX, const int dinoY);
void DrawGameOver(const int score);
void SetConsoleView();
void CursorView(char show);

// 버퍼 초기화 함수
void InitScreen() {
    for (int buffer = 0; buffer < 2; buffer++) {
        for (int y = 0; y < CONSOLE_HEIGHT; y++) {
            for (int x = 0; x < CONSOLE_WIDTH; x++) {
                screen[buffer][y][x] = ' ';
            }
            screen[buffer][y][CONSOLE_WIDTH - 1] = '\0';
        }
    }
}

// 버퍼에 문자열 쓰기
void WriteBuffer(int x, int y, const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len && x + i < CONSOLE_WIDTH; i++) {
        screen[currentBuffer][y][x + i] = str[i];
    }
}

// 버퍼 전환 및 화면 출력
void FlipBuffer() {
    COORD pos = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int y = 0; y < CONSOLE_HEIGHT; y++) {
        pos.Y = y;
        SetConsoleCursorPosition(console, pos);
        printf("%s", screen[currentBuffer][y]);
    }

    currentBuffer = !currentBuffer;

    // 새 버퍼 초기화
    for (int y = 0; y < CONSOLE_HEIGHT; y++) {
        for (int x = 0; x < CONSOLE_WIDTH; x++) {
            screen[currentBuffer][y][x] = ' ';
        }
        screen[currentBuffer][y][CONSOLE_WIDTH - 1] = '\0';
    }
}

// 공룡 그리기
void DrawDino(int dinoY) {
    SetColor(SKYBLUE);
    static int legFlag = true;

    const char* dino[] = {
        "        $$$$$$$ ",
        "       $$ $$$$$",
        "       $$$$$$$$$$",
        "$      $$$      ",
        "$$     $$$$$$$  ",
        "$$$    $$$$$    ",
        " $$  $$$$$$$$$$$ ",
        " $$원광대",
        "  $$$    ",
        "검소공 $    ",
        "        $$$$$$$$  ",
        "        $$$$$$   "
    };

    for (int i = 0; i < 12; i++) {
        WriteBuffer(0, dinoY + i, dino[i]);
    }

    if (legFlag) {
        WriteBuffer(0, dinoY + 12, "        $   $$$   ");
        WriteBuffer(0, dinoY + 13, "        $$    ");
    }
    else {
        WriteBuffer(0, dinoY + 12, "        $$$  $     ");
        WriteBuffer(0, dinoY + 13, "             $$    ");
    }
    legFlag = !legFlag;
}

// 선인장 그리기
void DrawCactus(int cactusX) {
    SetColor(GREEN);
    const char* cactus[] = {
        "$",
        "$ $ ",
        "$$$",
        "$$$$$",
        " $ "
    };

    for (int i = 0; i < 5; i++) {
        WriteBuffer(cactusX * 2, CACTUS_BOTTOM_Y + i, cactus[i]);
    }
}

// 색상 설정
void SetColor(unsigned short text) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

// 커서 위치 이동
void GotoXY(int x, int y) {
    COORD Pos;
    Pos.X = 2 * x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 키 입력 감지
int GetKeyDown() {
    if (_kbhit() != 0) {
        return _getch();
    }
    return 0;
}

// 충돌 감지
int IsCollision(const int cactusX, const int dinoY) {
    if (cactusX <= 8 && cactusX >= 2 && dinoY > 12) {
        return true;
    }
    return false;
}

// 게임오버 화면
void DrawGameOver(const int score) {
    system("cls");
    SetColor(YELLOW);
    int x = 22, y = 8;

    GotoXY(x, y);
    printf("===========================");
    GotoXY(x, y + 1);
    printf("======G A M E O V E R=====");
    GotoXY(x, y + 2);
    printf("===========================");
    GotoXY(x, y + 5);
    printf("SCORE : %d", score);

    printf("\nWnWnWnWnWnWnWnWnWn");
    system("pause");
    system("cls");
}

// 커서 설정
void CursorView(char show) {
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

// 콘솔 설정
void SetConsoleView() {
    system("mode con:cols=120 lines=30");
    system("title 창의실무프로젝트 구글 공룡 게임 [By. YongWun Kim]");
    CursorView(false);
}

// 메인 함수
int main() {
    SetConsoleView();
    InitScreen();

    while (true) {
        // 게임 시작시 초기화
        bool isJumping = false;
        bool isBottom = true;
        const int gravity = 3;

        int dinoY = DINO_BOTTOM_Y;
        int cactusX = CACTUS_BOTTOM_X;

        int score = 0;
        clock_t start, curr;
        start = clock();

        while (true) {
            // 충돌체크
            if (IsCollision(cactusX, dinoY))
                break;

            // 점프 키 감지
            if (GetKeyDown() == ' ' && isBottom) {
                isJumping = true;
                isBottom = false;
            }

            // 점프 처리
            if (isJumping) {
                dinoY -= gravity;
            }
            else {
                dinoY += gravity;
            }

            // 바닥 충돌 처리
            if (dinoY >= DINO_BOTTOM_Y) {
                dinoY = DINO_BOTTOM_Y;
                isBottom = true;
            }

            // 점프 최고점 처리
            if (dinoY <= 3) {
                isJumping = false;
            }

            // 장애물 이동
            cactusX -= 2;
            if (cactusX <= 0) {
                cactusX = CACTUS_BOTTOM_X;
            }

            // 그리기
            DrawDino(dinoY);
            DrawCactus(cactusX);

            // 점수 처리
            curr = clock();
            if (((curr - start) / CLOCKS_PER_SEC) >= 1) {
                score++;
                start = clock();
            }

            // 점수 표시
            char scoreStr[20];
            sprintf(scoreStr, "Score : %d", score);
            SetColor(WHITE);
            WriteBuffer(44, 0, scoreStr);
            WriteBuffer(40, 2, "점프 : Space Key");

            // 화면 업데이트
            FlipBuffer();
            Sleep(60);
        }

        // 게임 오버
        DrawGameOver(score);
    }

    return 0;
}