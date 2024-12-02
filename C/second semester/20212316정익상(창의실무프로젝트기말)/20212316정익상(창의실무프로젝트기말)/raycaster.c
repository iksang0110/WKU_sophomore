// raycaster.c
#include "raycaster.h"
#include <math.h>

bool InitRaycaster(GameState* game) {
    // Z-버퍼 초기화
    if (game->zBuffer) {
        free(game->zBuffer);
    }
    game->zBuffer = (double*)malloc(sizeof(double) * game->screenWidth);
    if (!game->zBuffer) {
        return false;
    }

    // Z-버퍼 초기값 설정
    for (int i = 0; i < game->screenWidth; i++) {
        game->zBuffer[i] = 0.0;
    }

    return true;
}

void CastRay(GameState* game, int x, RaycastHit* hit) {
    // 카메라 x 좌표 계산 (-1 ~ 1)
    double cameraX = 2 * x / (double)game->screenWidth - 1;

    // 광선의 방향 벡터 계산
    hit->rayDir.x = game->player.direction.x + game->player.plane.x * cameraX;
    hit->rayDir.y = game->player.direction.y + game->player.plane.y * cameraX;

    // 현재 맵 위치
    hit->mapX = (int)game->player.position.x;
    hit->mapY = (int)game->player.position.y;

    // DDA 알고리즘 준비
    PerformDDA(game, hit);

    // 벽까지의 거리 계산
    CalculateWallDistance(hit, &game->player);

    // 화면에 그릴 벽의 높이와 위치 계산
    CalculateWallDrawing(hit, game->screenHeight);

    // Z-버퍼 업데이트
    game->zBuffer[x] = hit->perpWallDist;
}

void PerformDDA(GameState* game, RaycastHit* hit) {
    // deltaDistX와 deltaDistY 계산
    double deltaDistX = fabs(1.0 / hit->rayDir.x);
    double deltaDistY = fabs(1.0 / hit->rayDir.y);

    double sideDistX;
    double sideDistY;
    int stepX;
    int stepY;

    // 광선의 방향에 따른 이동 방향과 초기 사이드 거리 계산
    if (hit->rayDir.x < 0) {
        stepX = -1;
        sideDistX = (game->player.position.x - hit->mapX) * deltaDistX;
    }
    else {
        stepX = 1;
        sideDistX = (hit->mapX + 1.0 - game->player.position.x) * deltaDistX;
    }

    if (hit->rayDir.y < 0) {
        stepY = -1;
        sideDistY = (game->player.position.y - hit->mapY) * deltaDistY;
    }
    else {
        stepY = 1;
        sideDistY = (hit->mapY + 1.0 - game->player.position.y) * deltaDistY;
    }

    // DDA 알고리즘 실행
    int hit_wall = 0;
    while (hit_wall == 0) {
        // 다음 맵 위치로 이동
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            hit->mapX += stepX;
            hit->side = 0;
        }
        else {
            sideDistY += deltaDistY;
            hit->mapY += stepY;
            hit->side = 1;
        }

        // 벽 충돌 체크
        if (IsWall(&game->map, hit->mapX, hit->mapY)) {
            hit_wall = 1;
        }
    }
}

void CalculateWallDistance(RaycastHit* hit, Player* player) {
    if (hit->side == 0) {
        hit->perpWallDist = (hit->mapX - player->position.x +
            (1 - (hit->rayDir.x < 0 ? -1 : 1)) / 2) / hit->rayDir.x;
    }
    else {
        hit->perpWallDist = (hit->mapY - player->position.y +
            (1 - (hit->rayDir.y < 0 ? -1 : 1)) / 2) / hit->rayDir.y;
    }

    // wallX 계산 (텍스처 매핑을 위해)
    if (hit->side == 0) {
        hit->wallX = player->position.y + hit->perpWallDist * hit->rayDir.y;
    }
    else {
        hit->wallX = player->position.x + hit->perpWallDist * hit->rayDir.x;
    }
    hit->wallX -= floor(hit->wallX);
}

void CalculateWallDrawing(RaycastHit* hit, int screenHeight) {
    // 벽의 높이 계산
    hit->lineHeight = (int)(screenHeight / hit->perpWallDist);

    // 화면에 그릴 시작점과 끝점 계산
    hit->drawStart = -hit->lineHeight / 2 + screenHeight / 2;
    if (hit->drawStart < 0) {
        hit->drawStart = 0;
    }

    hit->drawEnd = hit->lineHeight / 2 + screenHeight / 2;
    if (hit->drawEnd >= screenHeight) {
        hit->drawEnd = screenHeight - 1;
    }
}

void DrawVerticalLine(GameState* game, int x, RaycastHit* hit, COLORREF color) {
    // y면에 부딪혔을 경우 색상을 어둡게
    if (hit->side == 1) {
        int r = GetRValue(color) / 2;
        int g = GetGValue(color) / 2;
        int b = GetBValue(color) / 2;
        color = RGB(r, g, b);
    }

    // 천장 그리기 (검은색)
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HPEN oldPen = (HPEN)SelectObject(game->backBuffer, pen);

    for (int y = 0; y < hit->drawStart; y++) {
        SetPixel(game->backBuffer, x, y, RGB(0, 0, 0));
    }

    // 벽 그리기
    for (int y = hit->drawStart; y < hit->drawEnd; y++) {
        SetPixel(game->backBuffer, x, y, color);
    }

    // 바닥 그리기 (회색)
    for (int y = hit->drawEnd; y < game->screenHeight; y++) {
        SetPixel(game->backBuffer, x, y, RGB(64, 64, 64));
    }

    SelectObject(game->backBuffer, oldPen);
    DeleteObject(pen);
}

COLORREF GetWallColor(RaycastHit* hit, Map* map) {
    // 벽 타입에 따른 색상 반환
    int wallType = GetTile(map, hit->mapX, hit->mapY);
    switch (wallType) {
    case 1: return RGB(255, 0, 0);    // 빨간색
    case 2: return RGB(0, 255, 0);    // 초록색
    case 3: return RGB(0, 0, 255);    // 파란색
    case 4: return RGB(255, 255, 0);  // 노란색
    case 5: return RGB(255, 0, 255);  // 보라색
    default: return RGB(128, 128, 128); // 회색
    }
}

void RenderFrame(GameState* game) {
    RaycastHit hit;

    // 화면의 각 수직선에 대해 레이캐스팅 수행
    for (int x = 0; x < game->screenWidth; x++) {
        // 광선 발사
        CastRay(game, x, &hit);

        // 벽 색상 얻기
        COLORREF color = GetWallColor(&hit, &game->map);

        // 수직선 그리기
        DrawVerticalLine(game, x, &hit, color);
    }

    // 백 버퍼를 화면에 표시
    BitBlt(game->deviceContext, 0, 0, game->screenWidth, game->screenHeight,
        game->backBuffer, 0, 0, SRCCOPY);
}

void CleanupRaycaster(GameState* game) {
    if (game->zBuffer) {
        free(game->zBuffer);
        game->zBuffer = NULL;
    }
}