// map.h
#ifndef MAP_H
#define MAP_H

#include "game.h"
#include <stdbool.h>

/**
 * @brief 맵 타일의 종류를 정의하는 열거형
 */
typedef enum {
    TILE_EMPTY = 0,    // 빈 공간
    TILE_WALL = 1,     // 기본 벽
    TILE_WALL2 = 2,    // 다른 종류의 벽
    TILE_WALL3 = 3,    // 또 다른 종류의 벽
    TILE_WALL4 = 4     // 네 번째 종류의 벽
} MapTileType;

/**
 * @brief 맵을 초기화하는 함수
 * @param map 초기화할 맵 구조체 포인터
 * @param width 맵의 너비
 * @param height 맵의 높이
 * @return 초기화 성공 여부
 */
bool InitMap(Map* map, int width, int height);

/**
 * @brief 맵 데이터를 파일에서 로드하는 함수
 * @param map 맵 구조체 포인터
 * @param filename 맵 파일 경로
 * @return 로드 성공 여부
 */
bool LoadMapFromFile(Map* map, const char* filename);

/**
 * @brief 기본 맵 데이터를 생성하는 함수
 * @param map 맵 구조체 포인터
 * @return 생성 성공 여부
 */
bool CreateDefaultMap(Map* map);

/**
 * @brief 특정 위치의 타일이 벽인지 확인하는 함수
 * @param map 맵 구조체 포인터
 * @param x X 좌표
 * @param y Y 좌표
 * @return 벽 여부 (true: 벽, false: 빈 공간)
 */
bool IsWall(Map* map, int x, int y);

/**
 * @brief 맵의 특정 위치의 타일 값을 반환하는 함수
 * @param map 맵 구조체 포인터
 * @param x X 좌표
 * @param y Y 좌표
 * @return 타일 값
 */
int GetTile(Map* map, int x, int y);

/**
 * @brief 맵의 특정 위치에 타일을 설정하는 함수
 * @param map 맵 구조체 포인터
 * @param x X 좌표
 * @param y Y 좌표
 * @param tileType 설정할 타일 종류
 */
void SetTile(Map* map, int x, int y, MapTileType tileType);

/**
 * @brief 맵 자원을 정리하는 함수
 * @param map 정리할 맵 구조체 포인터
 */
void CleanupMap(Map* map);

/**
 * @brief 특정 좌표가 맵 범위 내에 있는지 확인하는 함수
 */
bool IsInMapBounds(Map* map, int x, int y);

/**
 * @brief 특정 좌표가 이동 가능한 위치인지 확인하는 함수
 */
bool IsPassable(Map* map, double x, double y);

/**
 * @brief 맵의 특정 영역을 채우는 함수
 */
void FillMapArea(Map* map, int startX, int startY, int width, int height, MapTileType tileType);

/**
 * @brief 맵을 텍스트 파일로 저장하는 함수
 */
bool SaveMapToFile(Map* map, const char* filename);

/**
 * @brief 특정 위치 주변의 벽 개수를 세는 함수
 */
int CountSurroundingWalls(Map* map, int x, int y);

#endif // MAP_H