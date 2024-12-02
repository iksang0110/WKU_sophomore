// player.c
#include "player.h"
#include <math.h>

#define COLLISION_MARGIN 0.1

void MovePlayer(Player* player, Map* map, MoveDirection direction, double deltaTime) {
    double moveSpeed = player->moveSpeed * deltaTime;
    double newX = player->position.x;
    double newY = player->position.y;

    switch (direction) {
    case MOVE_FORWARD:
        newX += player->direction.x * moveSpeed;
        newY += player->direction.y * moveSpeed;
        break;
    case MOVE_BACKWARD:
        newX -= player->direction.x * moveSpeed;
        newY -= player->direction.y * moveSpeed;
        break;
    case MOVE_LEFT:
        newX += player->direction.y * moveSpeed;
        newY -= player->direction.x * moveSpeed;
        break;
    case MOVE_RIGHT:
        newX -= player->direction.y * moveSpeed;
        newY += player->direction.x * moveSpeed;
        break;
    }

    // 충돌 검사
    if (!CheckCollision(player, map, newX, newY)) {
        player->position.x = newX;
        player->position.y = newY;
    }
}

void RotatePlayer(Player* player, double angle, double deltaTime) {
    double rotSpeed = player->rotSpeed * deltaTime;
    double rotation = angle * rotSpeed;

    double oldDirX = player->direction.x;
    double oldPlaneX = player->plane.x;

    // 방향 벡터 회전
    player->direction.x = player->direction.x * cos(rotation) - player->direction.y * sin(rotation);
    player->direction.y = oldDirX * sin(rotation) + player->direction.y * cos(rotation);

    // 카메라 평면 회전
    player->plane.x = player->plane.x * cos(rotation) - player->plane.y * sin(rotation);
    player->plane.y = oldPlaneX * sin(rotation) + player->plane.y * cos(rotation);
}

bool CheckCollision(Player* player, Map* map, double newX, double newY) {
    // 맵 범위 체크
    int mapX = (int)newX;
    int mapY = (int)newY;

    // 벽과의 충돌 검사
    if (IsWall(map, mapX, mapY)) {
        return true;
    }

    return false;
}