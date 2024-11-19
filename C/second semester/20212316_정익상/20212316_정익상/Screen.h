#pragma once
#include "Blocks.h"
#include "Controller.h"
#include <Windows.h>

#ifndef SCREEN_H
#define SCREEN_H

// 콘솔 화면 버퍼 관련 구조체
typedef struct _SCREEN_BUFFER {
    HANDLE hBuffer;           // 버퍼 핸들
    CHAR_INFO* bufferData;    // 버퍼 데이터
    COORD bufferSize;         // 버퍼 크기
    COORD bufferCoord;        // 버퍼 좌표
    SMALL_RECT writeRegion;   // 쓰기 영역
} SCREEN_BUFFER;

// 전역 변수 선언
extern SCREEN_BUFFER g_screenBuffer;
extern HANDLE g_originalConsoleHandle;

// 더블 버퍼링 관련 함수 선언
void InitScreenBuffer(void);              // 스크린 버퍼 초기화
void ClearScreenBuffer(void);             // 스크린 버퍼 클리어
void FlipScreenBuffer(void);              // 스크린 버퍼 전환
void ReleaseScreenBuffer(void);           // 스크린 버퍼 해제
void WriteToScreenBuffer(int x, int y, char* str, WORD color);  // 버퍼에 문자열 쓰기

// 화면 크기 상수
#define CONSOLE_WIDTH 76
#define CONSOLE_HEIGHT 28

// 블록 문자 상수
#define BLOCK_CHAR "■"
#define EMPTY_CHAR "  "
#define WALL_CHAR "▩"

// 색상 열거형
typedef enum Color {
    BLACK,           //0
    DARK_BLUE,       //1
    DARK_GREEN,      //2
    DARK_SKYBLUE,    //3
    DARK_RED,        //4
    DARK_VOILET,     //5
    DARK_YELLOW,     //6
    GRAY,            //7
    DARK_GRAY,       //8
    BLUE,            //9
    GREEN,           //10
    SKYBLUE,         //11
    RED,             //12
    VIOLET,          //13
    YELLOW,          //14
    WHITE            //15
} COLOR;

// 기존 변수들
int static level;
extern int NextBlock[6][6];
int static Rotation = 0;

// 기존 함수들
void textcolor(int color_number);
void GameTitle(void);
void InGameFrame(void);
void ShowNextBlock(void);
void DeleteNextBlock(void);
void EndGameFrame(void);
void createBoards(void);
void printBoards(void);
void addBlock(int shape, int rotate);
void deleteBlock(void);

// 게임 보드
extern int board[BoardHeight][BoardWidth];

#endif // SCREEN_H