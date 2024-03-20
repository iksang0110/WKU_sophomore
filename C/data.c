// #include <stdio.h>
// #define MAX 10

// int insertElement(int L[], int n, int x) {
//     int i, k = 0, move = 0;
//     for(i = 0; i < n - 1; i++) {
//         if(L[i] <= x && x <= L[i+1]) {
//             k = i + 1; break;
//         }
//     }
//     if (i == n - 1) k = n;
//     for (i = n; i > k; i--) {
//         L[i] = L[i -1];
//         move++;
//     }
//     L[k] = x;
//     return move;
// }

// int deleteElement(int L[], int n, int x) {
//     int i, k = n, move = 0;
//     for (i = 0; i < n; i++) {
//         if(L[i] == x) {
//             k = i; break;
//         }
//     }
//     if (i == n) return 0; // 원소를 찾지 못한 경우, 이동 없음
//     for (i = k; i < n - 1; i++) {
//         L[i] = L[i + 1];
//         move++;
//     }
//     return move;
// }

// int main(void) {
//     int list[MAX] = {10, 20, 30, 40, 50, 60, 70};
//     int i, move, size = 7; // 초기 원소의 개수 수정
//     printf("\n삽입 전 선형 리스트: ");
//     for (i = 0; i < size; i++) printf("%3d", list[i]);
//     printf("\n원소의 개수: %3d\n", size);

//     move = insertElement(list, size, 35);
//     size += 1; // 원소 삽입 후 사이즈 증가

//     printf("\n삽입 후 선형 리스트: ");
//     for (i = 0; i < size; i++) printf("%3d", list[i]);
//     printf("\n원소의 개수: %d\n", size);
//     printf("\n자리 이동 횟수: %d\n", move);

//     move = deleteElement(list, size, 30);
//     if (move == 0)
//         printf("\n선형 리스트에 원소가 없어서 삭제할 수 없습니다.\n");
//     else {
//         size -= 1; // 원소 삭제 후 사이즈 감소
//         printf("\n삭제 후 선형 리스트: ");
//         for (i = 0; i < size; i++) printf("%3d", list[i]);
//         printf("\n원소의 개수: %d", size);
//         printf("\n자리 이동 횟수: %d\n", move);
//     }
//     getchar();
//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h> // malloc, realloc, free 함수를 사용하기 위한 헤더 파일

// // insertElement 함수: 선형 리스트에 새로운 원소를 삽입하는 함수
// // L: 선형 리스트를 가리키는 포인터의 주소
// // size: 리스트의 현재 크기를 가리키는 포인터
// // capacity: 리스트의 최대 용량을 가리키는 포인터
// // x: 리스트에 삽입할 원소의 값
// int insertElement(int **L, int *size, int *capacity, int x) {
//     // 리스트의 현재 크기가 최대 용량에 도달한 경우 용량을 2만큼 증가시킴
//     if (*size >= *capacity) {
//         *capacity += 2; // 용량을 2만큼 증가
//         // realloc 함수를 사용하여 리스트의 크기를 재조정
//         *L = (int *)realloc(*L, *capacity * sizeof(int));
//         // 메모리 할당에 실패한 경우 오류 메시지 출력 후 함수 종료
//         if (*L == NULL) {
//             printf("메모리 할당 오류\n");
//             return -1;
//         }
//     }

//     int i, k = 0;
//     // 새로운 원소를 삽입할 위치를 찾기 위한 반복문
//     for (i = 0; i < *size; i++) {
//         if ((*L)[i] <= x && x <= (*L)[i + 1]) {
//             k = i + 1;
//             break;
//         }
//     }
//     if (i == *size) k = *size; // 삽입 위치가 리스트의 끝인 경우

//     // 원소를 삽입하기 위해 기존의 원소들을 뒤로 한 칸씩 이동
//     for (i = *size; i > k; i--) {
//         (*L)[i] = (*L)[i - 1];
//     }
//     (*L)[k] = x; // 새로운 원소를 삽입 위치에 대입
//     (*size)++; // 리스트의 크기를 1 증가
//     return 0;
// }

// // deleteElement 함수: 선형 리스트에서 원소를 삭제하는 함수
// // L: 선형 리스트를 가리키는 포인터의 주소
// // size: 리스트의 현재 크기를 가리키는 포인터
// // x: 리스트에서 삭제할 원소의 값
// int deleteElement(int **L, int *size, int x) {
//     int i, k = -1;
//     // 삭제할 원소의 위치를 찾기 위한 반복문
//     for (i = 0; i < *size; i++) {
//         if ((*L)[i] == x) {
//             k = i;
//             break;
//         }
//     }
//     if (k == -1) return -1; // 삭제할 원소를 찾지 못한 경우

//     // 원소를 삭제하고 나머지 원소들을 앞으로 한 칸씩 이동
//     for (i = k; i < *size - 1; i++) {
//         (*L)[i] = (*L)[i + 1];
//     }
//     (*size)--; // 리스트의 크기를 1 감소
//     return 0;
// }

// // main 함수: 프로그램의 진입점
// int main(void) {
//     int initialCapacity = 6; // 초기 배열의 크기를 6으로 설정
//     int size = 0; // 리스트의 초기 크기를 0으로 설정
//     // malloc 함수를 사용하여 초기 용량의 배열을 동적으로 할당
//     int *list = (int *)malloc(initialCapacity * sizeof(int));
//     if (list == NULL) {
//         printf("메모리 할당 오류\n"); // 메모리 할당 실패 시 오류 메시지 출력
//         return -1;
//     }

//     // 초기 리스트에 원소를 추가하는 예제
//     // insertElement 함수를 여러 번 호출하여 원소를 삽입
//     insertElement(&list, &size, &initialCapacity, 10);
//     insertElement(&list, &size, &initialCapacity, 20);
//     insertElement(&list, &size, &initialCapacity, 30);
//     insertElement(&list, &size, &initialCapacity, 40);
//     insertElement(&list, &size, &initialCapacity, 50);
//     insertElement(&list, &size, &initialCapacity, 60);

//     // 삽입 후의 리스트와 크기를 출력하는 예제
//     printf("\n삽입 후 선형 리스트: ");
//     for (int i = 0; i < size; i++) {
//         printf("%3d", list[i]);
//     }
//     printf("\n");

//     // 25를 삽입하는 예제
//     insertElement(&list, &size, &initialCapacity, 25);
//     printf("25 삽입 후 선형 리스트: ");
//     for (int i = 0; i < size; i++) {
//         printf("%3d", list[i]);
//     }
//     printf("\n");

//     // 30을 삭제하는 예제
//     deleteElement(&list, &size, 30);
//     printf("30 삭제 후 선형 리스트: ");
//     for (int i = 0; i < size; i++) {
//         printf("%3d", list[i]);
//     }
//     printf("\n");

//     free(list); // 동적으로 할당된 메모리를 해제
//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>

// int insertElement(int **L, int *n, int *size, int x) {
//     // 배열 크기가 부족하면, 크기를 2 증가시킨다.
//     if (*n >= *size) {
//         *size += 2;
//         *L = realloc(*L, sizeof(int) * (*size));
//         if (*L == NULL) {
//             fprintf(stderr, "Memory allocation failed\n");
//             exit(EXIT_FAILURE);
//         }
//     }

//     int i = 0;
//     while (i < *n && (*L)[i] < x) i++; // 삽입할 위치를 찾는다.
//     for (int j = *n; j > i; j--) {
//         (*L)[j] = (*L)[j - 1]; // 요소를 뒤로 이동한다.
//     }
//     (*L)[i] = x; // 새로운 요소를 삽입한다.
//     (*n)++;
//     return i; // 이동한 요소의 수가 아니라 삽입한 위치를 반환한다.
// }

// int deleteElement(int **L, int *n, int x) {
//     int i = 0;
//     while (i < *n && (*L)[i] != x) i++;
//     if (i == *n) return -1; // 요소를 찾지 못했다면 -1을 반환한다.

//     for (int j = i; j < *n - 1; j++) {
//         (*L)[j] = (*L)[j + 1]; // 요소를 앞으로 이동한다.
//     }
//     (*n)--;
//     return i; // 삭제한 위치를 반환한다.
// }

// int main(void) {
//     int initialSize = 6; // 초기 배열 크기
//     int *list = malloc(sizeof(int) * initialSize); // 동적 메모리 할당
//     if (list == NULL) {
//         fprintf(stderr, "Memory allocation failed\n");
//         return EXIT_FAILURE;
//     }

//     // 리스트 초기화
//     int size = 6; // 현재 배열에 저장된 요소의 수
//     int currentSize = initialSize; // 현재 할당된 배열의 크기
//     int elements[] = {10, 20, 40, 50, 60, 70}; // 초기 요소들
//     for (int i = 0; i < size; i++) {
//         list[i] = elements[i];
//     }

//     // 요소 삽입
//     printf("Initial list: ");
//     for (int i = 0; i < size; i++) {
//         printf("%d ", list[i]);
//     }
//     printf("\n");

//     // 삽입할 값과 위치 출력
//     int insertValue = 30;
//     int insertPosition = insertElement(&list, &size, &currentSize, insertValue);
//     printf("After inserting %d at position %d: ", insertValue, insertPosition);
//     for (int i = 0; i < size; i++) {
//         printf("%d ", list[i]);
//     }
//     printf("\n");

//     // 요소 삭제
//     int deleteValue = 30;
//     int deletePosition = deleteElement(&list, &size, deleteValue);
//     if (deletePosition != -1) {
//         printf("After deleting %d at position %d: ", deleteValue, deletePosition);
//         for (int i = 0; i < size; i++) {
//             printf("%d ", list[i]);
//         }
//         printf("\n");
//     } else {
//         printf("%d not found in the list.\n", deleteValue);
//     }

//     free(list); // 동적 할당된 메모리 해제
//     return EXIT_SUCCESS;
// }
#include <stdio.h>
#include <stdlib.h> // 동적 메모리 할당을 위해 추가

#define INIT_MAX 6

int insertElement(int **L, int *n, int *maxSize, int x) {
    if (*n == *maxSize) { // 배열이 가득 차면
        *maxSize += 2; // 배열 크기 2 증가
        *L = (int *)realloc(*L, (*maxSize) * sizeof(int)); // 배열 크기 조정
        printf("\n배열 크기가 %d로 늘어났습니다.\n", *maxSize); // 메모리 크기 변화 출력
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
