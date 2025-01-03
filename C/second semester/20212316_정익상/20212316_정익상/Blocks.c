/*테트리스 7개의 블록에 대한 배열이 정의되어 있음*/

#include "Blocks.h"

/*
block.h과 소스파일은 테트리스의 7가지 블록에 대해서 회전할 수 있는 4가지 경우를 4차원 배열로 표현했다.
Blocks 배열
- 1번째, index [BlockType]에 따라서 블록의 형태를 지정할 수 있다.
- 2번째, index [BlockRotate]에 따라서 블록이 회전할 수 있는 4가지 형태를 지정할 수 있다.
- 3번째, 4번째 인덱스(4*4) 형태를 출력하면 원하는 블록의 형태를 얻을 수 있다.
*/

int Blocks[BlockType][BlockRotate][BlockSIZE][BlockSIZE] =
{
	 0,0,0,0 //Block
	,0,2,2,0 // ■■ 
	,0,2,2,0 // ■■
	,0,0,0,0
	,0,0,0,0
	,0,2,2,0
	,0,2,2,0
	,0,0,0,0
	,0,0,0,0
	,0,2,2,0
	,0,2,2,0
	,0,0,0,0
	,0,0,0,0
	,0,2,2,0
	,0,2,2,0
	,0,0,0,0

	,0,0,0,0 //Block
	,2,2,2,2 //■■■■
	,0,0,0,0
	,0,0,0,0
	,0,0,2,0
	,0,0,2,0
	,0,0,2,0
	,0,0,2,0
	,0,0,0,0
	,2,2,2,2
	,0,0,0,0
	,0,0,0,0
	,0,0,2,0
	,0,0,2,0
	,0,0,2,0
	,0,0,2,0

	,0,0,0,0 //Block
	,0,0,2,2 //   ■■
	,0,2,2,0 // ■■ 
	,0,0,0,0
	,0,0,2,0
	,0,0,2,2
	,0,0,0,2
	,0,0,0,0
	,0,0,0,0
	,0,0,2,2
	,0,2,2,0
	,0,0,0,0
	,0,0,2,0
	,0,0,2,2
	,0,0,0,2
	,0,0,0,0

	,0,0,0,0 //Block
	,0,2,2,0 // ■■ 
	,0,0,2,2 //   ■■
	,0,0,0,0
	,0,0,0,2
	,0,0,2,2
	,0,0,2,0
	,0,0,0,0
	,0,0,0,0
	,0,2,2,0
	,0,0,2,2
	,0,0,0,0
	,0,0,0,2
	,0,0,2,2
	,0,0,2,0
	,0,0,0,0

	,0,0,0,0 //Block
	,0,2,2,2 // ■■■
	,0,2,0,0 // ■  
	,0,0,0,0
	,0,0,2,0
	,0,0,2,0
	,0,0,2,2
	,0,0,0,0
	,0,0,0,2
	,0,2,2,2
	,0,0,0,0
	,0,0,0,0
	,0,2,2,0
	,0,0,2,0
	,0,0,2,0
	,0,0,0,0

	,0,0,0,0 //Block
	,2,2,2,0 //■■■ 
	,0,0,2,0 //    ■ 
	,0,0,0,0
	,0,0,2,2
	,0,0,2,0
	,0,0,2,0
	,0,0,0,0
	,0,2,0,0
	,0,2,2,2
	,0,0,0,0
	,0,0,0,0
	,0,0,2,0
	,0,0,2,0
		 ,0,2,2,0
		 ,0,0,0,0

		,0,0,0,0 //Block
		,0,2,2,2 // ■■■
		,0,0,2,0 //   ■ 
		,0,0,0,0
		,0,0,2,0
		,0,0,2,2
		,0,0,2,0
		,0,0,0,0
		,0,0,2,0
		,0,2,2,2
		,0,0,0,0
		,0,0,0,0
		,0,0,2,0
		,0,2,2,0
		,0,0,2,0
		,0,0,0,0
};