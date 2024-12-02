#pragma once
// raycaster.h
#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "game.h"
#include "map.h"
#include <windows.h>

/**
 * @brief 레이 히트 정보를 저장하는 구조체
 */
typedef struct {
    Vector2D rayDir;         // 광선의 방향 벡터
    double perpWallDist;     // 벽까지의 수직 거리
    int mapX;               // 부딪힌 맵의 X 좌표
    int mapY;               // 부딪힌 맵의 Y 좌표
    int side;               // 부딪힌 벽면 (0: x면, 1: y면)
    double wallX;           // 벽에 부딪힌 정확한 위치
    int lineHeight;         // 화면에 그려질 벽의 높이
    int drawStart;          // 벽 그리기 시작 Y 좌표
    int drawEnd;            // 벽 그리기 종료 Y 좌표
} RaycastHit;

/**
 * @brief 레이캐스터 초기화 함수
 * @param game 게임 상태 구조체 포인터
 * @return 초기화 성공 여부
 */
bool InitRaycaster(GameState* game);

/**
 * @brief 단일 광선 캐스팅 함수
 * @param game 게임 상태 구조체 포인터
 * @param x 스크린 X 좌표
 * @param hit 레이 히트 정보를 저장할 구조체 포인터
 */
void CastRay(GameState* game, int x, RaycastHit* hit);

/**
 * @brief DDA(Digital Differential Analysis) 알고리즘 수행 함수
 * @param game 게임 상태 구조체 포인터
 * @param hit 레이 히트 정보를 저장할 구조체 포인터
 */
void PerformDDA(GameState* game, RaycastHit* hit);

/**
 * @brief 광선과 벽 사이의 거리 계산 함수
 * @param hit 레이 히트 정보 구조체 포인터
 * @param player 플레이어 구조체 포인터
 */
void CalculateWallDistance(RaycastHit* hit, Player* player);

/**
 * @brief 화면에 그릴 벽의 높이와 위치 계산 함수
 * @param hit 레이 히트 정보 구조체 포인터
 * @param screenHeight 화면 높이
 */
void CalculateWallDrawing(RaycastHit* hit, int screenHeight);

/**
 * @brief 화면에 수직선 그리기 함수
 * @param game 게임 상태 구조체 포인터
 * @param x 스크린 X 좌표
 * @param hit 레이 히트 정보 구조체 포인터
 * @param color 벽 색상
 */
void DrawVerticalLine(GameState* game, int x, RaycastHit* hit, COLORREF color);

/**
 * @brief 전체 화면 렌더링 함수
 * @param game 게임 상태 구조체 포인터
 */
void RenderFrame(GameState* game);

/**
 * @brief 벽 색상 결정 함수
 * @param hit 레이 히트 정보 구조체 포인터
 * @param map 맵 구조체 포인터
 * @return 벽 색상
 */
COLORREF GetWallColor(RaycastHit* hit, Map* map);

/**
 * @brief 레이캐스터 정리 함수
 * @param game 게임 상태 구조체 포인터
 */
void CleanupRaycaster(GameState* game);

#endif // RAYCASTER_H