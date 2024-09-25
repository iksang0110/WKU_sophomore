/*초기배열의 크기를 넘어선 입력 시 배열의 크기를 증가 시키는 프로그램
✓ 실습 3-1
» 사용자로부터-1을 입력받기 전까지 계속 정수를 입력받는다.
» 초기 배열의 크기는 3으로 설정하되, 사용자가 계속 입력하면 길이를 3씩 증가 시킨다.
» 길이를 증가시킬 때, 새로운 배열을 하나 만들어서 증가시킨다.
» 사용자가-1을 입력하면 배열의 값을 모두 출력한다.
» 동적할당(malloc함수)를 이용하고, realloc 함수는 절대 쓰지 않는다.
» 최대로 증가시킬 수 있는 배열의 크기는 20으로 제안한다.*/


// #pragma warning(disable:4996)
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// void size_add(int* s, int** arr);

// int main() {
//     // size가 3인 배열 생성
//     int* Np;
//     int i = 0;
//     int nSize = 3;
//     Np = malloc(sizeof(int) * nSize);

//     // 사용자로부터 -1을 입력받기 전까지 계속 정수를 입력받는다.
//     while (1) {
//         scanf("%d", &Np[i]);
//         if (Np[i] == -1) {
//             break;
//         }
//         i++;
//         // 배열의 크기를 초과하면 size_add 함수 호출
//         if (i >= nSize) {
//             size_add(&nSize, &Np);
//         }
//     }

//     // 입력받은 숫자들 출력
//     for (int j = 0; j < i; j++) {
//         printf(" %d", Np[j]);
//     }

//     free(Np);
//     return 0;
// }

// void size_add(int* s, int** arr) {
//     // 새로운 Size를 할당한다.
//     int nNewSize = *s + 3;
    
//     // temp에 기존의 배열 원소들을 복사한다.
//     int* temp = malloc(sizeof(int) * nNewSize);
//     memcpy(temp, *arr, sizeof(int) * (*s));
    
//     // 기존의 arr메모리를 해제한다.
//     free(*arr);
    
//     *arr = temp;
//     *s = nNewSize;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void size_add(int* s, int* arr);   

void main() {
    //size가 3인 배열 생성
    int* Np;
    int i = 0;
    int nSize = 3;
    Np = malloc (sizeof(int) * nSize);

    scanf("%d", &Np[0]);
    if (Np[0] != -1){
        for (i = 1;; i++) {
            scanf("%d", &Np[i]);
            if (Np[i] == -1) 
                break;
            if (i + 1 >= nSize) {
                size_add(&nSize, Np);
            }
            for (int j = 0; j <= i; j++) {
                printf("%d ", Np[j]);
            }
        }
    }
}
void size_add(int* s, int* arr) {
    int nNewSize = *s + 3;
    int* temp = malloc(sizeof(int) * nNewSize);
    memcpy(temp, arr, sizeof(int) * (*s));
    free(arr);
    arr = temp;
    *s = nNewSize;
}
    