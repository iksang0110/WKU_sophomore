// game.h
#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <stdbool.h>

// 게임의 기본 상수 정의
#define SCREEN_WIDTH    640     // 게임 창의 너비 (픽셀)
#define SCREEN_HEIGHT   480     // 게임 창의 높이 (픽셀)
#define MAP_WIDTH       24      // 맵의 너비 (타일 단위)
#define MAP_HEIGHT      24      // 맵의 높이 (타일 단위)
#define MOVE_SPEED      5.0     // 플레이어 이동 속도
#define ROT_SPEED       3.0     // 플레이어 회전 속도
#define WALL_HEIGHT     64      // 벽의 기본 높이
#define FOV            66       // 시야각 (degrees)

/**
 * @brief 2D 벡터를 표현하기 위한 구조체
 * x, y 좌표나 방향을 나타내는데 사용
 */
typedef struct {
    double x;  // x 좌표 또는 x 방향 성분
    double y;  // y 좌표 또는 y 방향 성분
} Vector2D;

/**
 * @brief 게임 맵 정보를 저장하는 구조체
 */
typedef struct {
    int width;          // 맵의 너비
    int height;         // 맵의 높이
    int* data;          // 맵 데이터 배열 (1차원 배열로 2D 맵 표현)
} Map;

/**
 * @brief 플레이어 정보를 저장하는 구조체
 */
typedef struct {
    Vector2D position;   // 현재 위치
    Vector2D direction;  // 바라보는 방향
    Vector2D plane;      // 카메라 평면
    double moveSpeed;    // 이동 속도
    double rotSpeed;     // 회전 속도
} Player;

/**
 * @brief 게임의 전체 상태를 관리하는 구조체
 */
typedef struct {
    // 화면 관련
    int screenWidth;           // 화면 너비
    int screenHeight;          // 화면 높이

    // 게임 객체들
    Player player;             // 플레이어 정보
    Map map;                   // 맵 정보

    // 렌더링 관련
    double* zBuffer;           // 깊이 버퍼 (레이캐스팅용)
    HWND window;              // 윈도우 핸들
    HDC deviceContext;        // 메인 디바이스 컨텍스트
    HDC backBuffer;           // 더블 버퍼링용 백 버퍼
    HBITMAP backBufferBitmap; // 백 버퍼 비트맵

    // 게임 상태 관련
    bool isRunning;           // 게임 실행 상태
    double deltaTime;         // 프레임 간 경과 시간
    double lastTime;          // 이전 프레임의 시간
} GameState;

/**
 * @brief 게임 초기화 함수
 * @param game 초기화할 게임 상태 구조체 포인터
 * @param hInstance 현재 프로그램의 인스턴스 핸들
 * @return 초기화 성공 여부
 */
bool InitGame(GameState* game, HINSTANCE hInstance);

/**
 * @brief 게임 메인 루프 실행 함수
 * @param game 게임 상태 구조체 포인터
 */
void RunGameLoop(GameState* game);

/**
 * @brief 게임 상태 업데이트 함수
 * @param game 업데이트할 게임 상태 구조체 포인터
 */
void UpdateGame(GameState* game);

/**
 * @brief 현재 시간을 초 단위로 반환하는 함수
 * @return 현재 시간 (초)
 */
double GetTime(void);

/**
 * @brief 프레임 간 경과 시간을 계산하는 함수
 * @param game 게임 상태 구조체 포인터
 */
void CalculateDeltaTime(GameState* game);

/**
 * @brief 게임 자원 정리 함수
 * @param game 정리할 게임 상태 구조체 포인터
 */
void CleanupGame(GameState* game);

#endif // GAME_H