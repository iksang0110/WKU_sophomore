#include <stdio.h>
#include <stdlib.h>

char *buffer;

int main() {
	buffer = (char *)malloc(100);
	// 메모리 해제를 하지 않으면 메모리 누수 발생
    //...
	free(buffer); // 잊지말고 메모리 해제 하기
	return 0;
}



