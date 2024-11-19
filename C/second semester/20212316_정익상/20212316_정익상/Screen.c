/*테트리스 화면 출력에 관련된 함수들이 정의되어 있음*/

#include "Screen.h"
#include "ConsoleCursor.h"
#include <Windows.h>

// 전역 변수 정의
SCREEN_BUFFER g_screenBuffer = { 0 };
HANDLE g_originalConsoleHandle = NULL;

// 다음 블록 배열 초기화
int NextBlock[6][6] = {
    1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1,
};

// 더블 버퍼링 초기화
void InitScreenBuffer(void) {
    // 원본 콘솔 핸들 저장
    g_originalConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // 새로운 버퍼 생성
    g_screenBuffer.hBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );

    // 버퍼 크기 설정
    g_screenBuffer.bufferSize.X = CONSOLE_WIDTH;
    g_screenBuffer.bufferSize.Y = CONSOLE_HEIGHT;

    // 버퍼 메모리 할당
    g_screenBuffer.bufferData = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * CONSOLE_WIDTH * CONSOLE_HEIGHT);

    // 버퍼 좌표 초기화
    g_screenBuffer.bufferCoord.X = 0;
    g_screenBuffer.bufferCoord.Y = 0;

    // 쓰기 영역 설정
    g_screenBuffer.writeRegion.Left = 0;
    g_screenBuffer.writeRegion.Top = 0;
    g_screenBuffer.writeRegion.Right = CONSOLE_WIDTH - 1;
    g_screenBuffer.writeRegion.Bottom = CONSOLE_HEIGHT - 1;

    // 커서 숨기기
    CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
    SetConsoleCursorInfo(g_screenBuffer.hBuffer, &cursorInfo);
    SetConsoleCursorInfo(g_originalConsoleHandle, &cursorInfo);

    // 초기 화면 지우기
    ClearScreenBuffer();
    FlipScreenBuffer();
}

// 버퍼 클리어
void ClearScreenBuffer(void) {
    CHAR_INFO emptyChar = { ' ', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
    for (int i = 0; i < CONSOLE_WIDTH * CONSOLE_HEIGHT; i++) {
        g_screenBuffer.bufferData[i] = emptyChar;
    }
}

// 버퍼 전환
void FlipScreenBuffer(void) {
    WriteConsoleOutput(
        g_screenBuffer.hBuffer,
        g_screenBuffer.bufferData,
        g_screenBuffer.bufferSize,
        g_screenBuffer.bufferCoord,
        &g_screenBuffer.writeRegion
    );
    SetConsoleActiveScreenBuffer(g_screenBuffer.hBuffer);
}

// 버퍼 해제
void ReleaseScreenBuffer(void) {
    if (g_screenBuffer.bufferData) {
        free(g_screenBuffer.bufferData);
        g_screenBuffer.bufferData = NULL;
    }
    if (g_screenBuffer.hBuffer) {
        CloseHandle(g_screenBuffer.hBuffer);
        g_screenBuffer.hBuffer = NULL;
    }
    SetConsoleActiveScreenBuffer(g_originalConsoleHandle);
}

// 버퍼에 문자열 쓰기
void WriteToScreenBuffer(int x, int y, char* str, WORD color) {
    int length = strlen(str);
    int index = y * CONSOLE_WIDTH + x;

    for (int i = 0; i < length && index < CONSOLE_WIDTH * CONSOLE_HEIGHT; i++, index++) {
        g_screenBuffer.bufferData[index].Char.AsciiChar = str[i];
        g_screenBuffer.bufferData[index].Attributes = color;
    }
}

// 색상 설정
void textcolor(int color_number) {
    SetConsoleTextAttribute(g_screenBuffer.hBuffer, color_number);
}

void GameTitle() {
    ClearScreenBuffer();

    int TETRIS_X = 13;
    int TETRIS_Y = 5;
    int picture_X = TETRIS_X + 5 * 2;
    int picture_Y = TETRIS_Y + 7;
    char introduce[20] = { "BY.iksagnJ" };

    int tetris[5][24] = {
        1,1,1,1,1,2,2,2,3,3,3,3,3,4,4,4,4,3,3,3,0,5,5,5,
        0,0,1,0,0,2,0,0,0,0,3,0,0,4,0,0,4,0,3,0,5,0,0,0,
        0,0,1,0,0,2,2,2,0,0,3,0,0,4,4,4,4,0,3,0,0,5,5,0,
        0,0,1,0,0,2,0,0,0,0,3,0,0,4,0,4,0,0,3,0,0,0,0,5,
        0,0,1,0,0,2,2,2,0,0,3,0,0,4,0,0,4,3,3,3,5,5,5,0
    };

    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 24; x++) {
            char blockStr[3] = BLOCK_CHAR;
            WORD color = WHITE;

            if (tetris[y][x] == 0) continue;

            if (tetris[y][x] == 1) color = YELLOW;
            else if (tetris[y][x] == 2) color = BLUE;
            else if (tetris[y][x] == 3) color = GREEN;
            else if (tetris[y][x] == 4) color = SKYBLUE;
            else if (tetris[y][x] == 5) color = RED;

            WriteToScreenBuffer(TETRIS_X + x * 2, TETRIS_Y + y, blockStr, color);
        }
        FlipScreenBuffer();
        Sleep(30);
    }

    // 제작자 정보 출력
    for (int i = 0; i < strlen(introduce); i++) {
        char temp[2] = { introduce[i], '\0' };
        WriteToScreenBuffer(TETRIS_X + 18 + i, 1, temp, WHITE);
        FlipScreenBuffer();
        Sleep(50);
    }
    Sleep(500);

    // 난이도 선택 메뉴 출력
    WriteToScreenBuffer(TETRIS_X, picture_Y, "게임 난이도", WHITE);
    WriteToScreenBuffer(TETRIS_X + 11, picture_Y, "(쉬움 = 0, 보통 = 1, 어려움 = 2, 종료 = 3)", YELLOW);
    WriteToScreenBuffer(TETRIS_X, picture_Y + 1, "난이도 입력 : ", WHITE);
    FlipScreenBuffer();

    // 난이도 입력 받기
    CursorView(true);
    scanf_s("%d", &level);
    CursorView(false);

    if (level == 0)
        gameLevel = easy;
    else if (level == 1)
        gameLevel = normal;
    else if (level == 2)
        gameLevel = hard;
    else if (level == 3)
        exit(false);

    ClearScreenBuffer();
    FlipScreenBuffer();
}

void InGameFrame() {
    ClearScreenBuffer();

    int picture_X = 36;
    int picture_Y = 0;
    int SecPicture[3][18] = {
        0,0,2,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,
        0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,
        0,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0
    };
    int picture[9][20] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,
        0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,2,0,2,2,2,
        0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,2,2,
        0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0,
        0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,2,0,0,2,
        0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,3,5,3,3,3,3,0,0,0,0,3,0,0,0,0,
        4,4,4,4,3,3,3,3,3,3,3,3,3,4,3,3,4,4,4,4,
        4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4
    };

    // 메인 그림 출력
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 20; x++) {
            WORD color = SKYBLUE;
            char blockStr[3] = BLOCK_CHAR;

            if (picture[y][x] == 0) continue;

            if (picture[y][x] == 1) color = WHITE;
            else if (picture[y][x] == 2) color = RED;
            else if (picture[y][x] == 3) color = BLUE;
            else if (picture[y][x] == 4) color = DARK_BLUE;
            else if (picture[y][x] == 5) {
                color = WHITE;
                strcpy_s(blockStr, 3, "♥");
            }

            WriteToScreenBuffer(picture_X + x * 2, picture_Y + y, blockStr, color);
            Sleep(10);
            FlipScreenBuffer();
        }
    }

    // 상단 그림 출력
    for (int y = 0; y < 3; y++) {
        for (int x = 17; x >= 0; x--) {
            char blockStr[3] = BLOCK_CHAR;
            if (SecPicture[y][x] == 0) {
                WriteToScreenBuffer(x * 2, y, blockStr, SKYBLUE);
            }
            else if (SecPicture[y][x] == 1) {
                strcpy_s(blockStr, 3, "★");
                WriteToScreenBuffer(x * 2, y, blockStr, YELLOW);
            }
            else if (SecPicture[y][x] == 2) {
                strcpy_s(blockStr, 3, "☆");
                WriteToScreenBuffer(x * 2, y, blockStr, DARK_YELLOW);
            }
            Sleep(10);
            FlipScreenBuffer();
        }
    }

    // 게임 정보 출력
    char difficultyStr[32];
    if (level == 0)
        strcpy_s(difficultyStr, 32, "▶ 게임 난이도 : 쉬움");
    else if (level == 1)
        strcpy_s(difficultyStr, 32, "▶ 게임 난이도 : 보통");
    else if (level == 2)
        strcpy_s(difficultyStr, 32, "▶ 게임 난이도 : 어려움");

    WriteToScreenBuffer(picture_X, picture_Y + 19, difficultyStr, WHITE);
    WriteToScreenBuffer(picture_X, picture_Y + 21, "▶ 게임 키 설명", WHITE);
    printScore();
    WriteToScreenBuffer(picture_X, picture_Y + 22, "  ↑         : 블록 회전", WHITE);
    WriteToScreenBuffer(picture_X, picture_Y + 23, "← →       : 좌측 이동/우측 이동", WHITE);
    WriteToScreenBuffer(picture_X, picture_Y + 24, "  ↓       : 아래 이동", WHITE);
    WriteToScreenBuffer(picture_X, picture_Y + 25, "■■■■   : Space BAR(맨 아래로...)", WHITE);
    FlipScreenBuffer();

    // 게임 시작 카운트다운
    for (int i = 7; i > 1; i--) {
        char countStr[32];
        sprintf_s(countStr, 32, "게임 시작 %d초 전...!", i / 2);
        WriteToScreenBuffer(46, 14, countStr, RED);
        FlipScreenBuffer();
        Sleep(300);

        WriteToScreenBuffer(46, 14, "                      ", RED);
        FlipScreenBuffer();
        Sleep(200);
    }
}

void ShowNextBlock() {
    WriteToScreenBuffer(52, 0 + 11, "다음블록", DARK_VOILET);

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 6; x++) {
            if (NextBlock[y][x] == 1) {
                WriteToScreenBuffer(42 + 8 + x * 2, 12 + y, WALL_CHAR, DARK_YELLOW);
            }
            if (y > 0 && y < 5) {
                if (x > 0 && x < 5) {
                    if (Blocks[nexShape][0][y - 1][x - 1] == 2) {
                        NextBlock[y][x] = Blocks[nexShape][0][y - 1][x - 1];
                        WriteToScreenBuffer(42 + 8 + x * 2, 12 + y, BLOCK_CHAR, WHITE);
                    }
                }
            }
        }
    }
    FlipScreenBuffer();
}

void DeleteNextBlock() {
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 6; x++) {
            if (y > 0 && y < 5) {
                if (x > 0 && x < 5) {
                    if (NextBlock[y][x] == 2) {
                        WriteToScreenBuffer(42 + 8 + x * 2, 12 + y, EMPTY_CHAR, WHITE);
                    }
                }
            }
        }
    }
    FlipScreenBuffer();
}

void createBoards() {
    for (int i = 0; i < BoardHeight; i++) {
        board[i][0] = 1;
        board[i][BoardWidth - 1] = 1;
        for (int j = 0; j < BoardWidth; j++) {
            if (i == 0)
                board[i][j] = 0;
            board[BoardHeight - 1][j] = 1;
            if (i > 0 && i < BoardHeight - 1)
                if (j > 0 && j < BoardWidth - 1)
                    board[i][j] = 0;
        }
    }
}

void printBoards() {
    // 상단 경계선
    for (int x = 1; x < 13; x++) {
        WriteToScreenBuffer(BoardX + x * 2, BoardY + 1, "__", WHITE);
    }

    // 측면 경계선과 블록들
    for (int y = 0; y < BoardHeight; y++) {
        if (board[y][0] == 1) {
            WriteToScreenBuffer(BoardX, BoardY + y, WALL_CHAR, DARK_YELLOW);
        }

        // 보드 내부의 블록들 출력
        for (int x = 1; x < BoardWidth - 1; x++) {
            if (board[y][x] >= 3) {
                WriteToScreenBuffer(BoardX + x * 2, BoardY + y, BLOCK_CHAR, board[y][x] - 3);
            }
        }

        if (board[y][BoardWidth - 1] == 1) {
            WriteToScreenBuffer(BoardX + BoardWidth * 2 - 2, BoardY + y, WALL_CHAR, DARK_YELLOW);
        }
    }

    // 하단 경계선
    for (int x = 0; x < BoardWidth; x++) {
        if (board[BoardHeight - 1][x] == 1) {
            WriteToScreenBuffer(BoardX + x * 2, BoardY + BoardHeight - 1, WALL_CHAR, DARK_YELLOW);
        }
    }
    FlipScreenBuffer();
}

void addBlock(int shape, int rotate) {
    COORD Pos = Cursor = getCursor();
    BlockROW = Pos.X / 2 - BoardX / 2;
    BlockCOL = Pos.Y - BoardY;

    for (int y = 0; y < BlockSIZE; y++) {
        for (int x = 0; x < BlockSIZE; x++) {
            if (Blocks[shape][rotate][y][x] == 2) {
                board[BlockCOL + y][BlockROW + x] = 2;
                WriteToScreenBuffer(Pos.X + x * 2, Pos.Y + y, BLOCK_CHAR, WHITE);
            }
        }
    }
    FlipScreenBuffer();
    GotoXY(Cursor.X, Cursor.Y);
}

void deleteBlock() {
    COORD Pos = Cursor = getCursor();
    BlockROW = Pos.X / 2 - BoardX / 2;
    BlockCOL = Pos.Y - BoardY;

    for (int y = 0; y < BlockSIZE; y++) {
        for (int x = 0; x < BlockSIZE; x++) {
            if (board[BlockCOL + y][BlockROW + x] == 2) {
                board[BlockCOL + y][BlockROW + x] = 0;
                WriteToScreenBuffer(Pos.X + x * 2, Pos.Y + y, EMPTY_CHAR, WHITE);
            }
        }
    }
    FlipScreenBuffer();
    GotoXY(Cursor.X, Cursor.Y);
}

void EndGameFrame() {
    // 게임 오버 효과
    for (int y = 1; y < BoardHeight - 1; y++) {
        for (int x = 1; x < BoardWidth - 1; x++) {
            if (board[y][x] >= 3) {
                WriteToScreenBuffer(BoardX + x * 2, BoardY + y, "♥", WHITE);
            }
        }
        FlipScreenBuffer();
        Sleep(150 - y * 5);
    }

    // 종료 메시지 출력
    for (int i = 7; i > 1; i--) {
        WriteToScreenBuffer(BlockStartX, BlockStartY + 1, "게임종료", RED);
        char endStr[32];
        sprintf_s(endStr, 32, "시작화면 %d초 전!!", i / 2);
        WriteToScreenBuffer(BlockStartX - 4, BlockStartY + 2, endStr, RED);
        FlipScreenBuffer();
        Sleep(300);

        WriteToScreenBuffer(BlockStartX, BlockStartY + 1, "        ", RED);
        WriteToScreenBuffer(BlockStartX - 4, BlockStartY + 2, "                   ", RED);
        FlipScreenBuffer();
        Sleep(200);
    }

    ClearScreenBuffer();
    FlipScreenBuffer();
}

