#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <wchar.h>

#define PI 3.14159265359
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533

#define MAP_SIZE 50
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 45
#define MAX_ENEMIES 50
#define MAX_TOWERS 20

// 구조체 정의
typedef struct {
    float x, y;
    float angle;
    int health;
    int power;
} Player;

typedef struct {
    float x, y;
    int health;
    int active;
    int pathIndex;
} Enemy;

typedef struct {
    float x, y;
    int type;
    int power;
    int active;
} Tower;

// 전역 변수
Player player = { 2.0f, 2.0f, 0.0f, 100, 10 };
Enemy enemies[MAX_ENEMIES];
Tower towers[MAX_TOWERS];
int worldMap[MAP_SIZE][MAP_SIZE];
int pathMap[MAP_SIZE][MAP_SIZE];
int gameOver = 0;
int wave = 1;
int score = 0;
int enemyCount = 0;
int isPaused = 0;
int showMap = 0;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void loadMap(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Map file not found!\n");
        exit(1);
    }

    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            fscanf(file, "%d", &worldMap[y][x]);
        }
    }
    fclose(file);
}

void loadPathMap(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Path map file not found!\n");
        exit(1);
    }

    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            fscanf(file, "%d", &pathMap[y][x]);
        }
    }
    fclose(file);
}

// 색상에 따른 벽 문자 반환
char getWallChar(int wallType, float dist) {
    if (dist > 16) return ' ';
    if (dist > 8) return '.';

    switch (wallType) {
    case 7:  return '#';  // 회색 벽
    case 10: return '*';  // 적
    case 13: return '@';  // 타워
    case 14: return 'P';  // 플레이어 기지
    default: return '|';
    }
}

// 색상 설정
void setWallColor(int wallType) {
    switch (wallType) {
    case 7:  setColor(8);  break; // 회색
    case 10: setColor(12); break; // 빨강 (적)
    case 13: setColor(14); break; // 노랑 (타워)
    case 14: setColor(11); break; // 하늘색 (플레이어 기지)
    default: setColor(7);  break; // 기본 흰색
    }
}

void renderScene() {
    float rayAngle = player.angle - DR * 30;
    if (rayAngle < 0) rayAngle += 2 * PI;
    if (rayAngle > 2 * PI) rayAngle -= 2 * PI;

    for (int r = 0; r < SCREEN_WIDTH; r++) {
        // 광선 발사 로직
        float aTan = -1 / tan(rayAngle);
        float nTan = -tan(rayAngle);
        float dist = 1000000;
        int wallType = 0;

        // 수평 체크
        float hx = player.x, hy = player.y;
        float distH = 1000000;
        float horiX = player.x, horiY = player.y;

        if (rayAngle > PI) {
            hy = (int)player.y - 0.0001;
            hx = (player.y - hy) * aTan + player.x;
            float yo = -1;
            float xo = -yo * aTan;

            for (int i = 0; i < 32; i++) {
                int mx = (int)hx;
                int my = (int)hy;
                if (mx >= 0 && mx < MAP_SIZE && my >= 0 && my < MAP_SIZE && worldMap[my][mx] > 0) {
                    horiX = hx;
                    horiY = hy;
                    distH = sqrt((hx - player.x) * (hx - player.x) + (hy - player.y) * (hy - player.y));
                    break;
                }
                hx += xo;
                hy += yo;
            }
        }

        // 수직 체크
        float vx = player.x, vy = player.y;
        float distV = 1000000;
        float vertX = player.x, vertY = player.y;

        if (rayAngle > P2 && rayAngle < P3) {
            vx = (int)player.x - 0.0001;
            vy = (player.x - vx) * nTan + player.y;
            float xo = -1;
            float yo = -xo * nTan;

            for (int i = 0; i < 32; i++) {
                int mx = (int)vx;
                int my = (int)vy;
                if (mx >= 0 && mx < MAP_SIZE && my >= 0 && my < MAP_SIZE && worldMap[my][mx] > 0) {
                    vertX = vx;
                    vertY = vy;
                    distV = sqrt((vx - player.x) * (vx - player.x) + (vy - player.y) * (vy - player.y));
                    break;
                }
                vx += xo;
                vy += yo;
            }
        }

        // 더 가까운 거리 선택
        if (distV < distH) {
            dist = distV;
            wallType = worldMap[(int)vertY][(int)vertX];
        }
        else {
            dist = distH;
            wallType = worldMap[(int)horiY][(int)horiX];
        }

        dist = dist * cos(player.angle - rayAngle); // 어안 렌즈 보정

        // 벽 높이와 위치 계산
        float lineH = (MAP_SIZE * SCREEN_HEIGHT) / (dist + 0.0001);
        if (lineH > SCREEN_HEIGHT) lineH = SCREEN_HEIGHT;
        float lineO = (SCREEN_HEIGHT - lineH) / 2;

        // 벽 그리기
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            gotoxy(r, y);
            if (y >= lineO && y <= lineO + lineH) {
                setWallColor(wallType);
                printf("%c", getWallChar(wallType, dist));
            }
            else {
                setColor(0);
                printf(" ");
            }
        }

        rayAngle += DR * (60.0f / SCREEN_WIDTH);
        if (rayAngle < 0) rayAngle += 2 * PI;
        if (rayAngle > 2 * PI) rayAngle -= 2 * PI;
    }
}

void drawMinimap() {
    system("cls");
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if ((int)player.x == x && (int)player.y == y) {
                setColor(11);
                printf("P");
            }
            else {
                setWallColor(worldMap[y][x]);
                switch (worldMap[y][x]) {
                case 0:  printf(" "); break;
                case 7:  printf("#"); break;
                case 10: printf("*"); break;
                case 13: printf("@"); break;
                case 14: printf("B"); break;
                default: printf("."); break;
                }
            }
        }
        printf("\n");
    }
}

void handleInput() {
    if (!_kbhit()) return;

    char key = _getch();
    float moveSpeed = 0.2f;
    float rotSpeed = 0.1f;

    float newX = player.x;
    float newY = player.y;

    switch (key) {
    case 'w':
        newX += cos(player.angle) * moveSpeed;
        newY += sin(player.angle) * moveSpeed;
        break;
    case 's':
        newX -= cos(player.angle) * moveSpeed;
        newY -= sin(player.angle) * moveSpeed;
        break;
    case 'a':
        newX += cos(player.angle - P2) * moveSpeed;
        newY += sin(player.angle - P2) * moveSpeed;
        break;
    case 'd':
        newX += cos(player.angle + P2) * moveSpeed;
        newY += sin(player.angle + P2) * moveSpeed;
        break;
    case 'm':
        showMap = !showMap;
        break;
    case 'p':
        isPaused = !isPaused;
        break;
    case 'r':
        player.angle += PI;
        if (player.angle > 2 * PI) player.angle -= 2 * PI;
        break;
    case ' ':
        // 총알 발사 로직
        break;
    case 27: // ESC
        gameOver = 1;
        break;
    case 75: // Left arrow
        player.angle -= rotSpeed;
        if (player.angle < 0) player.angle += 2 * PI;
        break;
    case 77: // Right arrow
        player.angle += rotSpeed;
        if (player.angle > 2 * PI) player.angle -= 2 * PI;
        break;
    }

    if (worldMap[(int)newY][(int)newX] == 0) {
        player.x = newX;
        player.y = newY;
    }
}

void drawUI() {
    gotoxy(0, SCREEN_HEIGHT);
    setColor(7);
    printf("Health: %d | Score: %d | Wave: %d | ESC: Exit | P: Pause | M: Map",
        player.health, score, wave);
    if (isPaused) {
        gotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2);
        printf("PAUSED");
    }
}

int main() {
    // 콘솔 설정
    system("mode con cols=160 lines=46");
    system("title 3D Tower Defense");

    // 콘솔 폰트 설정
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 8;
    cfi.dwFontSize.Y = 12;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Terminal");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    // 커서 숨기기
    CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    // 게임 초기화
    loadMap("map1.txt");
    loadPathMap("map1_bfs.txt");

    // 게임 루프
    while (!gameOver) {
        if (!isPaused) {
            if (showMap) {
                drawMinimap();
            }
            else {
                renderScene();
            }
            drawUI();
        }
        handleInput();
        Sleep(30);
    }

    // 게임 종료
    system("cls");
    printf("\n\n  Game Over!\n");
    printf("  Final Score: %d\n", score);
    printf("  Wave Reached: %d\n", wave);

    return 0;
}