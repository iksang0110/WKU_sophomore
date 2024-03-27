//v 1.1
//메모리 누수를 방지하기위해 할당된 메모리 반드시 해제하기 이를 해결하기 위한 함수 사용. 참고 자료 (https://gdngy.tistory.com/171)
//배열 크기의 동적 조절을 위해 realloc 함수를 이용해 배열 크기를 증가 시키, 배열의 증가 기준과 증가량은 과제에 있는대로 설정
//포인터 로직 이해하기......................가장 어려웠던 부분
//int **L = 동적 할당 배열 주소를 가리키는 포인터의 주소 저장. 배열의 개별 원소에 접근(즉, *L은 배열 자체를 가리키는 포인터)
//*L을 통해 배열의 새로운 메모리 주소 할당 받기. 여기서 원본 배열의 주소를 변경할 수 있어야 하니까 이중 포인터 필요.
//int *n = 현재 배열에 저장된 원소의 개수를 나타내는 변수 주소를 가리킴. 함수 내에서 이 포인터로 원소개수를 직접 변경 가능. 동적 성질을 관리하기 위해 필요
//int *maxSize = 배열의 최대 크기를 나타내는 변수의 주소를 가리킴. 배열이 가득 찼을때 크기를 조정 해야하는 경우, 이 포인터로 최대크기 업데이트.
//v 1.2
//배열 크기가 늘어난걸 표시해주면 좋을 것 같아 추가
//realloc 함수 말고 malloc 함수를 이용해서 로직 구현
//포인터 타입을 캐스팅하는 것이 좋은 습관이 아니라는 정보 습득, 수정 (https://stackoverflow.com/questions/605845/should-i-cast-the-result-of-malloc)
#include <stdio.h>
#include <stdlib.h> // 동적 메모리 할당을 위해 추가

#define INIT_MAX 6

int insertElement(int **L, int *n, int *maxSize, int x) {
    if (*n == *maxSize) { // 배열이 가득 차면
        *maxSize += 2; // 배열 크기 2 증가시키기
        int *newL = malloc((*maxSize) * sizeof(int)); // 새 배열 크기로 메모리 할당하는 코드
        for (int i = 0; i < *n; i++) {
            newL[i] = (*L)[i]; // 데이터 복사하기
        }
        free(*L); // 기존 배열 메모리 해제하기
        *L = newL; // 새 배열 주소 업데이트하기
        printf("\n배열 크기가 %d로 늘어났습니다.\n", *maxSize);
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
    (*n) += 1; // 원소 삽입 후 사이즈 증가시키기 
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
    (*n) -= 1; // 원소 삭제 후 사이즈 감소시키기 
    return move;
}

int main(void) {
    int *list = malloc(INIT_MAX * sizeof(int)); // 동적 배열 할당
    int move, size = 0, maxSize = INIT_MAX; // 초기 원소 개수 0, 최대 크기 INIT_MAX로 선정

    // 초기 값 할당은 자료대로
    int initialValues[] = {10, 20, 40, 50, 60, 70};
    for (int i = 0; i < 6; i++) {
        insertElement(&list, &size, &maxSize, initialValues[i]);
    }

  printf("\n삽입 전 선형 리스트: ");
    for (int i = 0; i < size; i++) printf("%3d", list[i]);
    printf("\n원소의 개수: %d\n", size);
    move = insertElement(&list, &size, &maxSize, 30);
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

    free(list); // 동적 할당된 메모리 해제시키기
    getchar();
    return 0;
}