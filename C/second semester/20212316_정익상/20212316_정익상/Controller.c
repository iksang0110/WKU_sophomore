#include "Controller.h"
#include "Screen.h"
#include "ConsoleCursor.h"
#include "AddBlockColor.h"

// 전역 변수 초기화
GAME_STATE gameState = { 0 };
BLOCK_INFO currentBlock = { 0 };
int nkey = 0;
short curShape = 0;
short nexShape = 0;
short BlockCOL = 0;
short BlockROW = 0;
short prevBlockCOL = 0;
short prevBlockROW = 0;
COORD Cursor = { 0, 0 };
COORD previewPoint = { 0, 0 };
bool IsNextBlock = false;
short turn = 0;
int score = 0;
short gameLevel = 0;

// 게임 초기화
void InitGame(void) {
    // 게임 상태 초기화
    gameState.score = 0;
    gameState.isGameOver = false;
    gameState.isPaused = false;
    gameState.lastUpdateTime = GetTickCount();
    gameState.lastMoveTime = GetTickCount();

    // 블록 초기화
    currentBlock.isActive = false;
    currentBlock.rotation = 0;
    currentBlock.x = BlockStartX;
    currentBlock.y = BlockStartY;

    // 기타 초기화
    score = 0;
    turn = 0;
    IsNextBlock = false;

    // 첫 블록 생성
    srand((unsigned int)time(NULL));
    NextShape();
    CurrentShape();
    NextShape();
}

// 블록 회전
void RotateBlock(void) {
    deletePrevBlock();
    turn = (turn + 1) % 4;

    // 회전 후 충돌 검사
    if (IsCollision(curShape, turn, Cursor.X, Cursor.Y)) {
        turn = (turn + 3) % 4; // 회전 취소
    }
}

// 현재 블록 설정
void CurrentShape(void) {
    curShape = nexShape;
}

// 다음 블록 생성
void NextShape(void) {
    nexShape = rand() % BlockType;
}

// 점수 출력
void printScore(void) {
    char scoreStr[32];
    sprintf_s(scoreStr, 32, "★ 점수 : %d", score);
    WriteToScreenBuffer(36, 20, scoreStr, WHITE);
    FlipScreenBuffer();
}
// 최대 높이 초과 검사
bool IsOverHeight(void) {
    BlockROW = BlockStartX / 2;
    BlockCOL = 1;

    for (int y = 0; y < BlockSIZE; y++) {
        for (int x = 0; x < BlockSIZE; x++) {
            if (board[BlockCOL + y][BlockROW + x * 2] == 2 &&
                board[BlockCOL + y + 1][BlockROW + x * 2] >= 3) {
                return true;
            }
        }
    }
    return false;
}

// 충돌 검사
bool IsCollision(int shape, int rotate, int curX, int curY) {
    BlockROW = curX / 2 - BoardX / 2;
    BlockCOL = curY - BoardY;

    for (int y = 0; y < BlockSIZE; y++) {
        for (int x = 0; x < BlockSIZE; x++) {
            if (Blocks[shape][rotate][y][x] == 2) {
                if (board[BlockCOL + y][BlockROW + x] == 1 ||
                    board[BlockCOL + y][BlockROW + x] >= 3) {
                    return true;
                }
            }
        }
    }
    return false;
}
// 완성된 라인 제거
void deleteLine(void) {
    COORD Pos = Cursor = getCursor();

    for (int y = BoardHeight - 2; y > 0; y--) {
        int count = 0;
        for (int x = 1; x < BoardWidth - 1; x++) {
            if (board[y][x] >= 3)
                ++count;
            if (count >= 12) {
                int height = y;
                score += 1000;

                // 라인 지우기 효과
                for (int x = 1; x < BoardWidth - 1; x++) {
                    board[height][x] = 0;
                    WriteToScreenBuffer(x * 2 + BoardX, height + BoardY, EMPTY_CHAR, WHITE);
                }
                FlipScreenBuffer();
                Sleep(30);

                // 깜빡임 효과
                for (int i = 0; i < 6; i++) {
                    for (int x = 1; x < BoardWidth - 1; x++) {
                        if (board[height][x] == 0) {
                            WriteToScreenBuffer(x * 2 + BoardX, height + BoardY,
                                (i % 2 == 0) ? BLOCK_CHAR : EMPTY_CHAR, WHITE);
                        }
                    }
                    FlipScreenBuffer();
                    Sleep(100);
                }

                // 윗줄 내리기
                for (; height > 0; height--) {
                    for (int x = 1; x < BoardWidth - 1; x++) {
                        board[height][x] = board[height - 1][x];
                        if (board[height - 1][x] == 0 || board[height - 1][x] >= 3) {
                            WriteToScreenBuffer(x * 2 + BoardX, height + BoardY, EMPTY_CHAR, WHITE);
                        }
                    }
                }

                // 새로운 블록 배치 표시
                for (height = y; height > 0; height--) {
                    for (int x = 1; x < BoardWidth - 1; x++) {
                        if (board[height][x] >= 3 && board[height][x] <= 9) {
                            WriteToScreenBuffer(x * 2 + BoardX, height + BoardY, BLOCK_CHAR,
                                board[height][x] - 3);
                        }
                        else {
                            WriteToScreenBuffer(x * 2 + BoardX, height + BoardY, EMPTY_CHAR, WHITE);
                        }
                        if (height == 1) board[height][x] = 0;
                    }
                }

                FlipScreenBuffer();
                GotoXY(Cursor.X, Cursor.Y);
                printScore();
                return;
            }
        }
    }

    GotoXY(Cursor.X, Cursor.Y);
    printScore();
}

// 새로운 블록 생성
void newBlock(void) {
    turn = 0;
    NextShape();
    CurrentShape();
    NextShape();
    Cursor.X = BlockStartX;
    Cursor.Y = BlockStartY;
}

// 메인 게임 프로세스
void GameProcess(void) {
    InitScreenBuffer();
    system("mode con cols=76 lines=28 | title 창의실무프로젝트_테트리스 실습");
    createBoards();
    InitGame();
    InGameFrame();
    ShowNextBlock();

    while (!gameState.isGameOver) {
        DWORD currentTime = GetTickCount();

        // 다음 블록 표시 업데이트
        if (IsNextBlock) {
            IsNextBlock = false;
            DeleteNextBlock();
            ShowNextBlock();
        }

        // 보드 및 현재 블록 표시
        printBoards();
        GotoXY(Cursor.X, Cursor.Y);
        addBlock(curShape, turn);
        previewBlock(curShape, turn);

        // 키 입력 처리
        if (_kbhit()) {
            nkey = _getch();
            if (nkey == SPACEBAR) {
                // 스페이스바 처리 (하드 드롭)
                while (!IsCollision(curShape, turn, Cursor.X, Cursor.Y + 1)) {
                    deletePrevBlock();
                    deleteBlock();
                    GotoXY(Cursor.X, Cursor.Y + 1);
                    addBlock(curShape, turn);
                    if (IsCollision(curShape, turn, Cursor.X, Cursor.Y + 1)) {
                        blockFixed(curShape, turn);
                        break;
                    }
                }
                continue;
            }

            if (nkey == ARROW) {
                nkey = _getch();
                int nTurn;
                switch (nkey) {
                case UP:    // 회전
                    nTurn = turn;
                    if (!IsCollision(curShape, (++nTurn % 4), Cursor.X, Cursor.Y)) {
                        deletePrevBlock();
                        deleteBlock();
                        RotateBlock();
                        addBlock(curShape, turn);
                        previewBlock(curShape, turn);
                    }
                    break;

                case LEFT:  // 왼쪽 이동
                    if (!IsCollision(curShape, turn, Cursor.X - 2, Cursor.Y)) {
                        deletePrevBlock();
                        deleteBlock();
                        GotoXY(Cursor.X - 2, Cursor.Y);
                        addBlock(curShape, turn);
                        previewBlock(curShape, turn);
                    }
                    break;

                case RIGHT: // 오른쪽 이동
                    if (!IsCollision(curShape, turn, Cursor.X + 2, Cursor.Y)) {
                        deletePrevBlock();
                        deleteBlock();
                        GotoXY(Cursor.X + 2, Cursor.Y);
                        addBlock(curShape, turn);
                        previewBlock(curShape, turn);
                    }
                    break;

                case DOWN:  // 아래로 이동
                    if (!IsCollision(curShape, turn, Cursor.X, Cursor.Y + 2)) {
                        deletePrevBlock();
                        deleteBlock();
                        GotoXY(Cursor.X, Cursor.Y + 2);
                        addBlock(curShape, turn);
                        previewBlock(curShape, turn);
                    }
                    break;
                }
            }
        }

        // 자동 낙하
        if (currentTime - gameState.lastMoveTime >= gameLevel) {
            gameState.lastMoveTime = currentTime;
            deleteBlock();

            if (!IsCollision(curShape, turn, Cursor.X, Cursor.Y + 1)) {
                ++Cursor.Y;
            }
            else {
                blockFixed(curShape, turn);
            }
        }

        // 게임 오버 체크
        if (IsOverHeight()) {
            gameState.isGameOver = true;
            EndGameFrame();
            break;
        }

        Sleep(MIN_UPDATE_INTERVAL);  // CPU 사용량 최적화
    }

    ReleaseScreenBuffer();
}