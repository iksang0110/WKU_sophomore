#include <stdio.h>
#include <stdlib.h> // 동적 메모리 할당을 위해 추가

#define INIT_MAX 6

int insertElement(int **L, int *n, int *maxSize, int x) {
    if (*n == *maxSize) { // 배열이 가득 차면
        *maxSize += 2; // 배열 크기 2 증가
        *L = (int *)realloc(*L, (*maxSize) * sizeof(int)); // 배열 크기 조정
    }

    int i, k = 0, move = 0;
    for (i = 0; i < *n - 1; i++) {
        if ((*L)[i] <= x && x <= (*L)[i + 1]) {
            k = i + 1;
            break;
        }
    }
    if (i == *n - 1) k = *n;
    for (i = *n; i > k; i--) {
        (*L)[i] = (*L)[i - 1];
        move++;
    }
    (*L)[k] = x;
    (*n) += 1; // 원소 삽입 후 사이즈 증가
    return move;
}

int deleteElement(int *L, int *n, int x) {
    int i, k = *n, move = 0;
    for (i = 0; i < *n; i++) {
        if (L[i] == x) {
            k = i;
            break;
        }
    }
    if (i == *n) return 0; // 원소를 찾지 못한 경우, 이동 없음
    for (i = k; i < *n - 1; i++) {
        L[i] = L[i + 1];
        move++;
    }
    (*n) -= 1; // 원소 삭제 후 사이즈 감소
    return move;
}

int main(void) {
    int *list = (int *)malloc(INIT_MAX * sizeof(int)); // 동적 배열 할당
    int move, size = 0, maxSize = INIT_MAX; // 초기 원소 개수 0, 최대 크기 INIT_MAX

    // 초기 값 할당
    int initialValues[] = {10, 20, 30, 40, 50, 60};
    for (int i = 0; i < 6; i++) {
        insertElement(&list, &size, &maxSize, initialValues[i]);
    }

    printf("\n삽입 전 선형 리스트: ");
    for (int i = 0; i < size; i++) printf("%3d", list[i]);
    printf("\n원소의 개수: %3d\n", size);

    move = insertElement(&list, &size, &maxSize, 35);
    printf("\n삽입 후 선형 리스트: ");
    for (int i = 0; i < size; i++) printf("%3d", list[i]);
    printf("\n원소의 개수: %d\n", size);
    printf("\n자리 이동 횟수: %d\n", move);

    move = deleteElement(list, &size, 30);
    if (move == 0)
        printf("\n선형 리스트에 원소가 없어서 삭제할 수 없습니다.\n");
    else {
        printf("\n삭제 후 선형 리스트: ");
        for (int i = 0; i < size; i++) printf("%3d", list[i]);
        printf("\n원소의 개수: %d", size);
        printf("\n자리 이동 횟수: %d\n", move);
    }

    free(list); // 동적 할당된 메모리 해제
    getchar();
    return 0;
}
