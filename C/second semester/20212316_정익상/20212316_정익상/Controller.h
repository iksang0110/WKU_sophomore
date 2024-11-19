#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// 보드 속성
enum boardProperty {
    BoardWidth = 14,  // 보드 가로 길이
    BoardHeight = 22, // 보드 세로 길이
    BoardX = 4,      // 콘솔창에서 출력될 보드 X좌표
    BoardY = 3       // 콘솔창에서 출력될 보드 Y좌표
};

// 키 입력 코드
enum Keys {
    LEFT = 75,     // 키보드 왼쪽 키
    RIGHT = 77,    // 키보드 오른쪽 키
    UP = 72,       // 키보드 위 키
    DOWN = 80,     // 키보드 아래 키
    ARROW = 224,   // 키보드 방향키 판별
    SPACEBAR = 32  // 키보드 스페이스 키
};

// 게임 속도(난이도)
typedef enum gameSpeed {
    easy = 230,    // 게임 난이도 쉬움
    normal = 170,  // 게임 난이도 보통
    hard = 150     // 게임 난이도 어려움
} SPEED;

// 블록 속성
enum blockProperty {
    BlockSIZE = 4,     // 블록의 크기
    BlockRotate = 4,   // 블록의 회전
    BlockType = 7,     // 블록의 종류
    BlockStartX = 14,  // 블록이 커서창에서 시작될 x좌표
    BlockStartY = 4    // 블록이 커서창에서 시작될 y좌표
};

// 게임 상태 관리
typedef struct _GAME_STATE {
    int score;
    int level;
    bool isGameOver;
    bool isPaused;
    DWORD lastUpdateTime;
    DWORD lastMoveTime;
} GAME_STATE;

// 블록 정보 구조체
typedef struct _BLOCK_INFO {
    int shape;
    int rotation;
    int x;
    int y;
    bool isActive;
} BLOCK_INFO;

// 전역 변수 선언
extern int nkey;                  // 사용자가 입력한 특정 방향키 아스키 코드값
extern short curShape;           // 현재 블록 모양
extern short nexShape;           // 다음 블록 모양
extern short BlockCOL;          // 블록의 현재 좌표
extern short BlockROW;
extern short prevBlockCOL;      // 블록의 이전 좌표
extern short prevBlockROW;
extern COORD Cursor;            // 테트리스 보드 내 현재 위치하는 커서
extern COORD previewPoint;     // 미리보기 블록의 좌표
extern bool IsNextBlock;       // 다음 블록 표시 여부
extern short turn;            // 블록 회전 상태
extern int score;            // 게임 점수
extern short gameLevel;      // 게임 난이도
extern GAME_STATE gameState; // 게임 상태
extern BLOCK_INFO currentBlock; // 현재 블록 정보

// 함수 선언
void InitGame(void);              // 게임 초기화
void UpdateGame(void);            // 게임 상태 업데이트
void ProcessInput(void);          // 키 입력 처리
void UpdateBlockPosition(void);    // 블록 위치 업데이트
bool CheckCollision(int shape, int rotate, int x, int y);  // 충돌 검사
void RotateBlock(void);           // 블록 회전
void FixBlock(void);              // 블록 고정
void CheckLines(void);            // 줄 완성 검사
void RemoveLine(int line);        // 줄 제거
bool IsGameOver(void);            // 게임 오버 검사
void UpdateScore(int lines);      // 점수 업데이트
void printScore(void);  // 점수 출력 함수 선언

// 블록 관련 함수
void CurrentShape(void);          // 현재 블록 설정
void NextShape(void);             // 다음 블록 생성
void deletePrevBlock(void);       // 이전 블록 지우기
void newBlock(void);              // 새로운 블록 생성
void blockFixed(int shape, int rotate);  // 블록 고정
bool IsCollision(int shape, int rotate, int curX, int curY);  // 충돌 검사
bool IsMaxLine(void);             // 줄 완성 검사
void deleteLine(void);            // 줄 제거
bool IsOverHeight(void);          // 최대 높이 초과 검사
void previewBlock(int shape, int rotate);  // 미리보기 블록 표시
void GameProcess(void);           // 게임 프로세스 실행

// 추가된 성능 최적화 관련 상수
#define MIN_UPDATE_INTERVAL 16    // 최소 업데이트 간격 (ms)
#define RENDER_INTERVAL 33        // 렌더링 간격 (약 30 FPS)
#define INPUT_CHECK_INTERVAL 10   // 입력 체크 간격 (ms)