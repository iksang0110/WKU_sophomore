// input.h
#ifndef INPUT_H
#define INPUT_H

#include "game.h"
#include <windows.h>
#include <stdbool.h>

// 키 상태를 저장하는 구조체
typedef struct {
    bool up;         // W or 위쪽 화살표
    bool down;       // S or 아래쪽 화살표
    bool left;       // A or 왼쪽 화살표
    bool right;      // D or 오른쪽 화살표
    bool rotLeft;    // Q or 왼쪽 회전
    bool rotRight;   // E or 오른쪽 회전
    bool escape;     // ESC 키
} InputState;

// 입력 시스템 초기화
void InitInput(InputState* input);

// 입력 상태 업데이트
void UpdateInput(InputState* input, GameState* game);

// 특정 키의 상태 확인 함수들
bool IsKeyPressed(unsigned char key);    // 매개변수 타입 수정
bool IsKeyReleased(unsigned char key);   // 매개변수 타입 수정

// 입력 시스템 정리
void CleanupInput(void);

#endif // INPUT_H