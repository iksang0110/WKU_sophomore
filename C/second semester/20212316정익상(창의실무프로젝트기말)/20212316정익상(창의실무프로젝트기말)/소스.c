#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define MAX_BULLETS 50
#define MAX_ENEMIES 20

// 구조체 정의
typedef struct {
    int x, y;
    int health;
    int score;
} Player;

typedef struct {
    int x, y;
    bool isAlive;
    int pattern;
} Enemy;

typedef struct {
    int x, y;
    bool isActive;
    bool isEnemy;
} Bullet;

// 전역 변수
Player player;
Enemy enemies[MAX_ENEMIES];
Bullet bullets[MAX_BULLETS];
int level = 1;
bool gameOver = false;

// 함수 선언
void initGame();
void movePlayer();
void moveEnemies();
void updateBullets();
void checkCollisions();
void renderScreen();
void gotoxy(int x, int y);
void hideCursor();

// 게임 초기화
void initGame() {
    // 플레이어 초기화
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT - 2;
    player.health = 3;
    player.score = 0;

    // 적 초기화
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = (i % 10) * 8 + 5;
        enemies[i].y = (i / 10) * 2 + 2;
        enemies[i].isAlive = true;
        enemies[i].pattern = i % 3;
    }

    // 총알 초기화
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].isActive = false;
    }

    hideCursor();
}

// 플레이어 이동
void movePlayer() {
    if (GetAsyncKeyState(VK_LEFT) && player.x > 1)
        player.x--;
    if (GetAsyncKeyState(VK_RIGHT) && player.x < SCREEN_WIDTH - 2)
        player.x++;
    if (GetAsyncKeyState(VK_SPACE)) {
        // 총알 발사
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].isActive) {
                bullets[i].x = player.x;
                bullets[i].y = player.y - 1;
                bullets[i].isActive = true;
                bullets[i].isEnemy = false;
                break;
            }
        }
        Sleep(100); // 연속 발사 제한
    }
}

// 적 이동
void moveEnemies() {
    static int moveCounter = 0;
    moveCounter++;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isAlive) {
            switch (enemies[i].pattern) {
            case 0: // 좌우 이동
                enemies[i].x += (moveCounter % 40 < 20) ? 1 : -1;
                break;
            case 1: // 지그재그 이동
                enemies[i].x += (moveCounter % 20 < 10) ? 1 : -1;
                if (moveCounter % 40 == 0) enemies[i].y++;
                break;
            case 2: // 직선 하강
                if (moveCounter % 30 == 0) enemies[i].y++;
                break;
            }

            // 적 총알 발사
            if (rand() % 100 < 2) {
                for (int j = 0; j < MAX_BULLETS; j++) {
                    if (!bullets[j].isActive) {
                        bullets[j].x = enemies[i].x;
                        bullets[j].y = enemies[i].y + 1;
                        bullets[j].isActive = true;
                        bullets[j].isEnemy = true;
                        break;
                    }
                }
            }
        }
    }
}

// 총알 업데이트
void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive) {
            if (!bullets[i].isEnemy)
                bullets[i].y--;
            else
                bullets[i].y++;

            if (bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT)
                bullets[i].isActive = false;
        }
    }
}

// 충돌 검사
void checkCollisions() {
    // 플레이어 총알과 적 충돌
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive && !bullets[i].isEnemy) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].isAlive &&
                    bullets[i].x >= enemies[j].x - 1 &&
                    bullets[i].x <= enemies[j].x + 1 &&
                    bullets[i].y == enemies[j].y) {
                    enemies[j].isAlive = false;
                    bullets[i].isActive = false;
                    player.score += 100;
                }
            }
        }
    }

    // 적 총알과 플레이어 충돌
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive && bullets[i].isEnemy) {
            if (bullets[i].x >= player.x - 1 &&
                bullets[i].x <= player.x + 1 &&
                bullets[i].y == player.y) {
                player.health--;
                bullets[i].isActive = false;
                if (player.health <= 0)
                    gameOver = true;
            }
        }
    }
}

// 화면 렌더링
void renderScreen() {
    system("cls");

    // 점수와 생명력 표시
    gotoxy(0, 0);
    printf("Score: %d  Lives: %d  Level: %d", player.score, player.health, level);

    // 플레이어 렌더링
    gotoxy(player.x, player.y);
    printf("^");

    // 적 렌더링
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isAlive) {
            gotoxy(enemies[i].x, enemies[i].y);
            printf("W");
        }
    }

    // 총알 렌더링
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive) {
            gotoxy(bullets[i].x, bullets[i].y);
            printf(bullets[i].isEnemy ? "v" : "|");
        }
    }
}

// 커서 위치 이동
void gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 커서 숨기기
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// 메인 함수
int main() {
    initGame();

    while (!gameOver) {
        movePlayer();
        moveEnemies();
        updateBullets();
        checkCollisions();
        renderScreen();

        // 레벨 클리어 체크
        bool allEnemiesDead = true;
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].isAlive) {
                allEnemiesDead = false;
                break;
            }
        }

        if (allEnemiesDead) {
            level++;
            initGame(); // 다음 레벨 시작
        }

        Sleep(50); // 게임 속도 조절
    }

    // 게임 오버 화면
    system("cls");
    gotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2);
    printf("GAME OVER\n");
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 1);
    printf("Final Score: %d\n", player.score);

    return 0;
}