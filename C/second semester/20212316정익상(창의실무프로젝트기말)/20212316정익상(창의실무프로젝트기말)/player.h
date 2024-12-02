// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "map.h"
#include <stdbool.h>

// 플레이어 이동 방향
typedef enum {
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT
} MoveDirection;

// 플레이어 관련 함수들
void MovePlayer(Player* player, Map* map, MoveDirection direction, double deltaTime);
void RotatePlayer(Player* player, double angle, double deltaTime);
bool CheckCollision(Player* player, Map* map, double newX, double newY);

#endif // PLAYER_H