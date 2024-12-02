#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

#define screenWidth 120
#define screenHeight 40
#define mapWidth 32
#define mapHeight 32

// 미로 맵
int worldMap[mapWidth][mapHeight] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// 플레이어 상태
double posX = 1.5, posY = 1.5;    // 시작 위치
double dirX = -1, dirY = 0;       // 시작 방향
double planeX = 0, planeY = 0.60; // 카메라 평면

void render(char screen[screenHeight][screenWidth]) {
    // 화면 초기화
    for (int y = 0; y < screenHeight; y++)
        for (int x = 0; x < screenWidth; x++)
            screen[y][x] = ' ';

    // 레이캐스팅
    for (int x = 0; x < screenWidth; x++) {
        double cameraX = 2 * x / (double)screenWidth - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        double sideDistX, sideDistY;
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        int stepX, stepY;
        int hit = 0;
        int side;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }

        if (side == 0)
            perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(screenHeight * 1.6 / perpWallDist);
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        // 벽 문자 선택
        char wallChar;
        if (worldMap[mapX][mapY] == 2) {
            wallChar = 'X';  // 출구
        }
        else {
            // 거리와 측면에 따른 벽 표현
            if (side == 0) {
                if (perpWallDist < 3) wallChar = '#';
                else if (perpWallDist < 5) wallChar = 'x';
                else if (perpWallDist < 7) wallChar = '+';
                else wallChar = ':';
            }
            else {
                // 측면은 한 단계 어둡게
                if (perpWallDist < 3) wallChar = 'x';
                else if (perpWallDist < 5) wallChar = '+';
                else wallChar = ':';
            }
        }

        // 벽 그리기
        for (int y = drawStart; y < drawEnd; y++) {
            if (y >= 0 && y < screenHeight && x >= 0 && x < screenWidth) {
                screen[y][x] = wallChar;
            }
        }

        // 바닥 그리기
        for (int y = drawEnd; y < screenHeight; y++) {
            if (y >= 0 && y < screenHeight && x >= 0 && x < screenWidth) {
                double currentDist = screenHeight / (2.0 * y - screenHeight);
                if (currentDist < 2.0) screen[y][x] = '+';
                else if (currentDist < 3.0) screen[y][x] = '.';
                else screen[y][x] = ' ';
            }
        }
    }
}

void display(char screen[screenHeight][screenWidth]) {
    printf("\033[H");
    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            char c = screen[y][x];
            if (c == 'X') {
                printf("\033[31;1mX\033[0m");  // 출구 - 빨간색
            }
            else if (c == ' ') {
                printf("\033[40m \033[0m");  // 빈 공간 - 검은색
            }
            else if (y > screenHeight / 2) {
                printf("\033[44m%c\033[0m", c);  // 바닥 - 파란색 배경
            }
            else {
                putchar(c);  // 벽 - 기본 색상
            }
        }
        printf("\n");
    }
}

// movePlayer 함수는 이전과 동일
void movePlayer(int forward) {
    double moveSpeed = 0.15;
    double newPosX = posX + dirX * moveSpeed * forward;
    double newPosY = posY + dirY * moveSpeed * forward;

    if (worldMap[(int)newPosX][(int)posY] == 0) {
        posX = newPosX;
    }
    else if (worldMap[(int)newPosX][(int)posY] == 2) {
        system("cls");
        printf("\033[32m탈출 성공!\033[0m\n");
        printf("아무 키나 누르면 종료됩니다...");
        _getch();
        exit(0);
    }

    if (worldMap[(int)posX][(int)newPosY] == 0) {
        posY = newPosY;
    }
    else if (worldMap[(int)posX][(int)newPosY] == 2) {
        system("cls");
        printf("\033[32m탈출 성공!\033[0m\n");
        printf("아무 키나 누르면 종료됩니다...");
        _getch();
        exit(0);
    }
}

// rotatePlayer 함수는 이전과 동일
void rotatePlayer(int direction) {
    double rotSpeed = 0.08;
    double oldDirX = dirX;
    dirX = dirX * cos(rotSpeed * direction) - dirY * sin(rotSpeed * direction);
    dirY = oldDirX * sin(rotSpeed * direction) + dirY * cos(rotSpeed * direction);
    double oldPlaneX = planeX;
    planeX = planeX * cos(rotSpeed * direction) - planeY * sin(rotSpeed * direction);
    planeY = oldPlaneX * sin(rotSpeed * direction) + planeY * cos(rotSpeed * direction);
}

int main() {
    char screen[screenHeight][screenWidth];
    printf("\033[2J\033[?25l");  // 화면 초기화 및 커서 숨기기

    LARGE_INTEGER frequency, lastTime, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);

    while (1) {
        if (_kbhit()) {
            char input = _getch();
            switch (input) {
            case 'q': goto cleanup;
            case 'a': rotatePlayer(1); break;
            case 'd': rotatePlayer(-1); break;
            }
        }

        if (GetAsyncKeyState('W') & 0x8000) {
            movePlayer(1);
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            movePlayer(-1);
        }

        render(screen);
        display(screen);

        // 60 FPS 유지
        QueryPerformanceCounter(&currentTime);
        double elapsedTime = (double)(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
        if (elapsedTime < 1.0 / 60.0) {
            Sleep((DWORD)((1.0 / 60.0 - elapsedTime) * 1000));
        }
        QueryPerformanceCounter(&lastTime);
    }

cleanup:
    printf("\033[?25h");
    return 0;
}