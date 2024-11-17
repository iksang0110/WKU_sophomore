#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

enum boardProperty {
	BoardWidth=14, //보드 가로 길이
	BoardHeight=22, //보드 세로 길이
	BoardX=4, //콘솔창에서 출력될 보드 X좌표
	BoardY=3 //콘솔창에서 출력될 보드 Y좌표
};

enum Keys {
	LEFT=75, //키보드 왼쪽 키
	RIGHT=77, //키보드 오른쪽 키
	UP=72, //키보드 위 키
	DOWN=80, //키보드 아래 키
	ARROW=224, //키보드 방향키 판별
	SPACEBAR=32 //키보드 스페이스 키
};

typedef enum gameSpeed {
	easy=230, //게임 난이도 쉬움
	normal=170, //게임 난이도 보통
	hard=150 //게임 난이도 어려움
}SPPED;

enum blockProperty {
	BlockSIZE=4, //블록의 크기
	BlockRotate=4, //블록의 회전
	BlockType=7, //블록의 종류
	BlockStartX=14, //블록이 커서창에서 시작될 x좌표
	BlockStartY=4 //블록이 커서창에서 시작될 y좌표
};

//사용자가 입력한 특정 방향키 아스키 코드값
int nkey;
//현재 블록 모양
short curShape;
void CurrentShape();
//다음 블록 모양
short nexShape;
void NextShape();
//블록의 현재 좌표
short BlockCOL;
short BlockROW;
//블록의 이전 좌표
short prevBlockCOL;
short prevBlockROW;
//테트리스 보드 내 현재 위치하는 커서
COORD Cursor;
//이전 블록 지우기
void deletePrevBlock();
//스코어
int static score;
void printScore();
//게임 난이도
short gameLevel;
//새로운 블록 종류 생성
void newBlock();

boolean IsNextBlock;

short turn;
void RotateBlock(); //방향키 받을 시 rotate를 통해 방향키 변경

boolean IsCollision(int shape, int rotate, int curX, int curY);
//블록이 bottom에 닿았을 경우
void blockFixed(int shape, int rotate);

boolean IsMaxLine();
void deleteLine();
//처음 생성된 블록이 천장과 닿았는가
boolean IsOverHeight();
//미리보기 블록 만들고 지울 때 해당 좌표 저장해야 한다
COORD previewPoint;
//미리보기 블록, 현재 블록 이후에 나올 블록 화면에 표현
void previewBlock(int shape, int rotate);
//테트리스 돌아가는데 있어 가장 중요한 함수(메인함수급)
void GameProcess();