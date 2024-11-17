#include<stdio.h>
#include<Windows.h>
#include<process.h>

CRITICAL_SECTION cs;
void print_num(int a) {
	while (1) {
		EnterCriticalSection(&cs);
		printf("%d번째 함수 시작!\n", a);
		printf("중요한 작업 중\n");
		printf("%d번째 함수 끝!\n", a);
		LeaveCriticalSection(&cs);
		Sleep(1000);
	}
}

int main(void) {
	InitializeCriticalSection(&cs);
	HANDLE thread1 = _beginthreadex(NULL, 0, (_beginthreadex_proc_type)print_num, (void*)1, 0, NULL);
	HANDLE thread2 = _beginthreadex(NULL, 0, (_beginthreadex_proc_type)print_num, (void*)2, 0, NULL);
	WaitForSingleObject(thread1, INFINITE);
	DeleteCriticalSection(&cs);
	return 0;
}