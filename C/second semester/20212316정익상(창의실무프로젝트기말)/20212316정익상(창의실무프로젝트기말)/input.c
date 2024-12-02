// input.c
#include "input.h"
#include "player.h"
#include <string.h>

// 이전 키 상태를 저장하는 배열
static bool previousKeyState[256] = { false };

void InitInput(InputState* input) {
    // 모든 입력 상태를 false로 초기화
    input->up = false;
    input->down = false;
    input->left = false;
    input->right = false;
    input->rotLeft = false;
    input->rotRight = false;
    input->escape = false;

    // 이전 키 상태 배열 초기화
    memset(previousKeyState, 0, sizeof(previousKeyState));
}

void UpdateInput(InputState* input, GameState* game) {
    // ESC 키 처리
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        game->isRunning = false;
        return;
    }

    // 이동 키 상태 업데이트
    input->up = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000);
    input->down = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000);
    input->left = GetAsyncKeyState('A') & 0x8000;
    input->right = GetAsyncKeyState('D') & 0x8000;
    input->rotLeft = GetAsyncKeyState('Q') & 0x8000 || (GetAsyncKeyState(VK_LEFT) & 0x8000);
    input->rotRight = GetAsyncKeyState('E') & 0x8000 || (GetAsyncKeyState(VK_RIGHT) & 0x8000);

    // 플레이어 이동 처리
    if (input->up) {
        MovePlayer(&game->player, &game->map, MOVE_FORWARD, game->deltaTime);
    }
    if (input->down) {
        MovePlayer(&game->player, &game->map, MOVE_BACKWARD, game->deltaTime);
    }
    if (input->left) {
        MovePlayer(&game->player, &game->map, MOVE_LEFT, game->deltaTime);
    }
    if (input->right) {
        MovePlayer(&game->player, &game->map, MOVE_RIGHT, game->deltaTime);
    }

    // 회전 처리
    if (input->rotLeft) {
        RotatePlayer(&game->player, 1.0, game->deltaTime);
    }
    if (input->rotRight) {
        RotatePlayer(&game->player, -1.0, game->deltaTime);
    }
}

bool IsKeyPressed(unsigned char key) {
    bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
    bool wasPressed = !previousKeyState[key] && currentState;
    previousKeyState[key] = currentState;
    return wasPressed;
}

bool IsKeyReleased(unsigned char key) {
    bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
    bool wasReleased = previousKeyState[key] && !currentState;
    previousKeyState[key] = currentState;
    return wasReleased;
}

void CleanupInput(void) {
    // 입력 시스템 정리
    memset(previousKeyState, 0, sizeof(previousKeyState));
}