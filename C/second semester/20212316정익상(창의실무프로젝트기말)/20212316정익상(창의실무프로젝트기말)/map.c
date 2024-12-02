// map.c
#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool InitMap(Map* map, int width, int height) {
    // 유효성 검사
    if (width <= 0 || height <= 0) {
        return false;
    }

    // 맵 크기 설정
    map->width = width;
    map->height = height;

    // 맵 데이터를 위한 메모리 할당
    map->data = (int*)malloc(sizeof(int) * width * height);
    if (!map->data) {
        return false;
    }

    // 모든 타일을 빈 공간으로 초기화
    memset(map->data, TILE_EMPTY, sizeof(int) * width * height);

    return true;
}

bool LoadMapFromFile(Map* map, const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0) {
        return false;
    }

    // 맵 크기 읽기
    int width, height;
    if (fscanf_s(file, "%d %d", &width, &height) != 2) {
        fclose(file);
        return false;
    }

    // 맵 초기화
    if (!InitMap(map, width, height)) {
        fclose(file);
        return false;
    }

    // 맵 데이터 읽기
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tile;
            if (fscanf_s(file, "%d", &tile) != 1) {
                fclose(file);
                return false;
            }
            SetTile(map, x, y, (MapTileType)tile);
        }
    }

    fclose(file);
    return true;
}

bool CreateDefaultMap(Map* map) {
    // 기본 맵 크기로 초기화
    if (!InitMap(map, MAP_WIDTH, MAP_HEIGHT)) {
        return false;
    }

    // 기본 맵 데이터 정의
    const int defaultMap[MAP_HEIGHT][MAP_WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    // 맵 데이터 복사
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            SetTile(map, x, y, defaultMap[y][x]);
        }
    }

    return true;
}

bool IsInMapBounds(Map* map, int x, int y) {
    return (x >= 0 && x < map->width && y >= 0 && y < map->height);
}

bool IsWall(Map* map, int x, int y) {
    // 맵 범위 검사
    if (!IsInMapBounds(map, x, y)) {
        return true;    // 맵 밖은 벽으로 처리
    }

    // 해당 위치의 타일이 벽인지 확인
    return map->data[y * map->width + x] != TILE_EMPTY;
}

int GetTile(Map* map, int x, int y) {
    // 맵 범위 검사
    if (!IsInMapBounds(map, x, y)) {
        return TILE_WALL;    // 맵 밖은 벽으로 처리
    }

    return map->data[y * map->width + x];
}

void SetTile(Map* map, int x, int y, MapTileType tileType) {
    // 맵 범위 검사
    if (!IsInMapBounds(map, x, y)) {
        return;    // 맵 밖은 무시
    }

    map->data[y * map->width + x] = tileType;
}

bool IsPassable(Map* map, double x, double y) {
    // 실수 좌표를 정수 좌표로 변환
    int mapX = (int)x;
    int mapY = (int)y;

    // 맵 범위 체크
    if (!IsInMapBounds(map, mapX, mapY)) {
        return false;
    }

    // 해당 위치가 빈 공간인지 확인
    return GetTile(map, mapX, mapY) == TILE_EMPTY;
}

void FillMapArea(Map* map, int startX, int startY, int width, int height, MapTileType tileType) {
    for (int y = startY; y < startY + height && y < map->height; y++) {
        for (int x = startX; x < startX + width && x < map->width; x++) {
            if (IsInMapBounds(map, x, y)) {
                SetTile(map, x, y, tileType);
            }
        }
    }
}

bool SaveMapToFile(Map* map, const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "w");
    if (err != 0) {
        return false;
    }

    // 맵 크기 저장
    fprintf(file, "%d %d\n", map->width, map->height);

    // 맵 데이터 저장
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            fprintf(file, "%d ", GetTile(map, x, y));
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

int CountSurroundingWalls(Map* map, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            if (IsWall(map, x + dx, y + dy)) {
                count++;
            }
        }
    }
    return count;
}

void CleanupMap(Map* map) {
    if (map->data) {
        free(map->data);
        map->data = NULL;
    }
    map->width = 0;
    map->height = 0;
}