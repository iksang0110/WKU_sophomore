#include "Controller.h"
#include "Screen.h"
#include "ConsoleCursor.h"
#include "AddBlockColor.h"

void RotateBlock() {
    turn = ++turn;
    if (turn == 4) turn = turn % 4;
}
void CurrentShape() { curShape = nexShape; }
void NextShape() {
    srand(time(NULL));
    nexShape = rand() % BlockType;
}
void printScore() {
    GotoXY(36, 20);
    printf("⊙ 점수 : %d", score);
}
boolean IsOverHeight() {
    BlockROW = BlockStartX / 2;
    BlockCOL = 1;
    for (int y = 0; y < BlockSIZE; y++) {
        for (int x = 0; x < BlockSIZE; x++) {
            if (board[BlockCOL + y][BlockROW + x * 2] == 2 &&
                board[BlockCOL + y + 1][BlockROW + x * 2] >= 3)
                return true;
        }
    }
    return false;
}

boolean IsCollision(int shape, int rotate, int curX, int curY) {
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

void deletePrevBlock() {
    for (int y = 0; y < BlockSIZE; y++) {
        for (int x = 0; x < BlockSIZE; x++) {
            if (board[prevBlockCOL + y][prevBlockROW + x] == 2) {
                GotoXY(previewPoint.X + x * 2, previewPoint.Y + y);
                printf("  ");
            }
        }
    }
    GotoXY(Cursor.X, Cursor.Y);
}

void previewBlock(int shape, int rotate) {
    COORD Pos = Cursor = getCursor();
    while (!IsCollision(curShape, turn, Pos.X, Pos.Y + 1)) {
        ++Pos.Y;
        if (IsCollision(curShape, turn, Pos.X, Pos.Y + 1) == true) {
            previewPoint.X = Pos.X;
            previewPoint.Y = Pos.Y;
            prevBlockROW = BlockROW = Pos.X / 2 - BoardX / 2;
            prevBlockCOL = BlockCOL = Pos.Y - BoardY;
            for (int y = 0; y < BlockSIZE; y++) {
                for (int x = 0; x < BlockSIZE; x++) {
                    if (Blocks[shape][rotate][y][x] == 2) {
                        board[BlockCOL + y][BlockROW + x] = 2;
                        GotoXY(Pos.X + x * 2, Pos.Y + y);
                        prevAddBlockColor();
                    }
                }
            }
            GotoXY(Cursor.X, Cursor.Y);
            return;
        }
    }
}

void blockFixed(int shape, int rotate) {
    COORD Pos = getCursor();
    BlockROW = Pos.X / 2 - BoardX / 2;
    BlockCOL = Pos.Y - BoardY;
    for (int y = 0; y < BlockSIZE; y++) {
        for (int x = 0; x < BlockSIZE; x++) {
            if ((Blocks[shape][rotate][y][x] == 2)) {
                if (board[BlockCOL + y][BlockROW + x] == 2) {
                    board[BlockCOL + y][BlockROW + x] = shape + 3;
                    GotoXY(Pos.X + x * 2, Pos.Y + y);
                    addCurrentBlockColor(shape);
                }
            }
        }
    }
    while (true)
    {
        if (IsMaxLine()) deleteLine();
        else break;
    }
    turn = 0;
    CurrentShape();
    NextShape();
    Cursor.X = BlockStartX;
    Cursor.Y = BlockStartY;
    GotoXY(Cursor.X, Cursor.Y);
    addBlock(curShape, turn);
    deleteBlock();
    IsNextBlock = true;
}

boolean IsMaxLine() {
    for (int y = BoardHeight - 2; y > 1; y--) {
        int count = 0;
        for (int x = 1; x < BoardWidth - 1; x++) {
            if (board[y][x] >= 3)
                ++count;
            if (count >= 12)
                return true;
        }
    }
    return false;
}

void deleteLine() {
    COORD Pos = Cursor = getCursor();
    for (int y = BoardHeight - 2; y > 0; y--) {
        int count = 0;
        for (int x = 1; x < BoardWidth - 1; x++) {
            if (board[y][x] >= 3)
                ++count;
            if (count >= 12) {
                int height = y;
                score += 1000;

                for (int x = 1; x < BoardWidth - 1; x++) {
                    board[height][x] = 0;
                    if (board[height][x] == 0) {
                        GotoXY(x * 2 + BoardX, height + BoardY);
                        printf("  "); Sleep(30);
                    }
                }
                for (int i = 0; i < 6; i++) {
                    for (int x = 1; x < BoardWidth - 1; x++) {
                        if (board[height][x] == 0) {
                            GotoXY(x * 2 + BoardX, height + BoardY);
                            if (i % 2 == 0) printf("□");
                            else printf("  ");
                        }
                    }
                    Sleep(100);
                }
                for (height; height > 0; height--) {
                    for (int x = 1; x < BoardWidth - 1; x++) {
                        board[height][x] = board[height - 1][x];
                        if (board[height - 1][x] == 0 || board[height - 1][x] >= 3) {
                            GotoXY(x * 2 + BoardX, height + BoardY); printf("  ");
                        }
                    }
                }
                for (height = y; height > 0; height--) {
                    for (int x = 1; x < BoardWidth - 1; x++) {
                        if (board[height][x] >= 3 && board[height][x] <= 9) {
                            GotoXY(x * 2 + BoardX, height + BoardY);
                            colorRetention(board[height][x] - 3);
                        }
                        else {
                            GotoXY(x * 2 + BoardX, height + BoardY); printf("  ");
                        }
                        if (height == 1) board[height][x] = 0;
                    }
                }
                GotoXY(Cursor.X, Cursor.Y);
                printScore();
                return;
            }
        }
    }
    GotoXY(Cursor.X, Cursor.Y);
    printScore();
}

void newBlock() {
    turn = 0;
    NextShape();
    CurrentShape();
    NextShape();
    Cursor.X = BlockStartX;
    Cursor.Y = BlockStartY;
}

void GameProcess() {
    system("mode con cols=76 lines=28 | title 창의실무프로젝트_테트리스 실습");
    createBoards();
    srand(time(NULL));
    newBlock();
    InGameFrame();
    ShowNextBlock();

    while (true) {
        if (IsNextBlock == true) {
            IsNextBlock = false;
            DeleteNextBlock();
            ShowNextBlock();
        }
        printBoards();
        GotoXY(Cursor.X, Cursor.Y);
        addBlock(curShape, turn);
        previewBlock(curShape, turn);
        if (_kbhit()) {
            nkey = _getch();
            if (nkey == SPACEBAR) {
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
                case UP:
                    nTurn = turn;
                    if (!IsCollision(curShape, (++nTurn % 4), Cursor.X, Cursor.Y)) {
                        deletePrevBlock();
                        deleteBlock();
                        RotateBlock();
                        addBlock(curShape, turn);
                        previewBlock(curShape, turn);
                        continue;
                    }
                    break;
                case LEFT:
                    if (!IsCollision(curShape, turn, Cursor.X - 2, Cursor.Y)) {
                        deletePrevBlock();
                        deleteBlock();
                        GotoXY(Cursor.X - 2, Cursor.Y);
                        addBlock(curShape, turn);
                        previewBlock(curShape, turn);
                        continue;
                    }
                    break;
                case RIGHT:
                    if (!IsCollision(curShape, turn, Cursor.X + 2, Cursor.Y)) {
                        deletePrevBlock();
                        deleteBlock();
                        GotoXY(Cursor.X + 2, Cursor.Y);
                        addBlock(curShape, turn);
                        previewBlock(curShape, turn);
                        continue;
                    }
                    break;
                case DOWN:
                    if (!IsCollision(curShape, turn, Cursor.X, Cursor.Y + 2)) {
                        deletePrevBlock();
                        deleteBlock();
                        GotoXY(Cursor.X, Cursor.Y + 2);
                        addBlock(curShape, turn);
                        previewBlock(curShape, turn);
                        continue;
                    }
                    break;
                }
            }
        }
        Sleep(gameLevel);
        deleteBlock(curShape, turn);

        if (!IsCollision(curShape, turn, Cursor.X, Cursor.Y + 1)) ++Cursor.Y;
        else blockFixed(curShape, turn);

        if (IsOverHeight()) {
            EndGameFrame(); return;
        }
    }
}