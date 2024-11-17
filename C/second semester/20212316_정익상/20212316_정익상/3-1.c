#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void size_add(int* s, int* arr);

void main() {
	//size가 3인 배열 생성
	int* Np;
	int i = 0;
	int nSize = 3;
	Np = malloc(sizeof(int) * nSize);

	scanf_s("%d", &Np[0]);

	if (Np[0] != -1) {
		for (i = 1;; i++) {
			scanf_s("%d", &Np[i]);
			if (Np[i] == -1)
				break;
			if (i + 1 >= nSize) {
				//최대로 증가시킬 수 있는 배열의 크기는 20으로 제한
				size_add(&nSize, &Np);
				if (nSize > 20) {
					printf("배열 크기는 20을 초과할 수 없습니다.\n");
					free(Np);
					return 1; //에러 코드 반환
				}
			}

		}

		for (int j = 0; j <= i; j++)
			printf("%d", Np[j]);
	}
}

void size_add(int* s, int** arr) {
	//새로운 Size를 획득한다.
	int nNewSize = *s + 3;
	int* temp;
	temp = malloc(sizeof(int) * nNewSize);

	//temp에 기존의 배열 원소들을 복사한다.
	for (int i = 0; i < *s; i++)
		temp[i] = (*arr)[i];

	//기존의 arr메모리들을 해제한다.
	free(*arr);

	*arr = temp;
	*s = nNewSize;
}