/*메인 함수 정의*/

#include "Blocks.h"
#include "Controller.h"
#include "Screen.h"
#include "ConsoleCursor.h"

void main()
{
	CursorView(false);
	system("mode con cols=76 lines=28 | title 창의실무프로젝트_테트리스 실습");
	
	/*
	아래의 PlaySound 메서드는 .wav 음악 파일을 재생시켜주는 함수다.
	Controller.h 헤더파일에 include한 mmsystem.h 헤더파일을 통해 .wav음악을 출력시키는데,
	음악을 출력할 경우 해당 경료를 Controller.h에 BGM의 경로를 문자열로 지정하면 된다.
	그 후, PlaySound(TEXT(BGM), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); 해제
	*/

	while (true)
	{
		GameTitle();
		GameProcess();
	}
}