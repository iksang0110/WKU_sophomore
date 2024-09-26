/*배열의 크기를 입력받고 중간값에 해당하는 원소를 지운 배열을 재 출력하는 프로그램
✓ 실습 3-2
» 배열의 초기 크기 N을 입력 받습니다. (N은 홀수만 입력 가능하다고 가정합니다.)
» 정수 N개를 저장할 수 있는 메모리를 동적할당하고 이 메모리에 N까지 차례대로 저장
» 정수 N개의 정수에서 중간값에 해당하는 원소를 지웁니다.
» 중간값이 지원진 배열을 재 출력 합니다.
» 크기 M(M=N-1)의 새로운 배열을 만들어 메모리를 동적할당하고 realloc 함수는 절대 쓰지 않는다.*/


#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

void delete_middle(int* s, int** arr);

int main() {
    int N;
    scanf("%d", &N);
    
    // N개의 정수를 저장할 수 있는 메모리를 동적 할당
    int* Np;
    Np = malloc(sizeof(int) * N);
    
    // N개의 정수를 입력받음
    for (int i = 0; i < N; i++) {
        scanf("%d", &Np[i]);
    }
    
    // 배열의 크기 N과 포인터 Np의 주소를 매개변수로 넘김
    delete_middle(&N, &Np);
    
    // 중앙값이 삭제된 배열을 출력
    for (int i = 0; i < N; i++) {
        printf("%d ", Np[i]);
    }
    
    // 메모리 해제
    free(Np);
    
    return 0;
}

void delete_middle(int* s, int** arr) {
    // 중앙값을 삭제하므로 배열의 크기가 1 줄어듦
    // 크기가 1 줄어든 배열을 새로 할당
    int nNewSize = *s - 1;
    int* temp = malloc(sizeof(int) * nNewSize);
    
    // 메모리를 할당한 배열에 기존의 배열값을 복사함
    // 이 때 중앙값은 제외 함
    int middle = *s / 2;
    for (int i = 0, nNewIndex = 0; i < *s; i++) {
        // Np 인덱스를 1 증가시켜서 다음 값으로 넘김 (삭제)
        // 중앙값을 건너 뛴 다음값을 저장
        if (i == middle) continue;
        temp[nNewIndex++] = (*arr)[i];
    }
    
    // 기존의 배열 메모리는 새로운 배열에 복사했으므로
    // 해제하고 새로운 배열 temp를 가리키게 함
    free(*arr);
    *arr = temp;
    *s = nNewSize;
}

// #include <stdio.h>
// #include <stdlib.h>

// void delete_middle(int* s,int** arr);

// void main() {
//     int N;
//     scanf("%d",&N);
//     //N개의 정수를 저장할 배열을 동적할당한다.
//     int* Np;
//     Np = malloc(N*sizeof(int) * N);

//     for (int i = 0; i < N; i++) 
//         scanf("%d",&Np[i]);
//         //배열의 크기 N과 포인터 Np의 주소를 매개변수로 넘김
//         delete_middle(&N,Np);

//         //삭제된 배열을 출력
//         for (int i = 0; i < N; i++) 
//             printf("%d ",Np[i]);
//         //동적할당한 배열을 해제한다.
//         free(Np);
// }
// void delete_middle(int* s, int** arr){
//     //중간값을 삭제하므로 배열의 크기를 1 줄인다.
//     //크기가 1 줄어든 배열을 새로 할당
//     int* temp;
//     temp = malloc((*s-1)*sizeof(int) * (*s-1));
//     //배열의 중간값을 제외한 나머지 값을 새로 할당한 배열에 복사
//     //이때 중간값은 제외함
//     for (int i = 0 ; i <= *s; i++) {
//         //nNweIndeex가 중앙 값을 가리킬 때
//         //Np 인덱스를 1 증가시켜서 다음 값으로 넘김(삭제)
//         //중간값을 건너 뛴 다음값을 저장
//         if (i == *s/2) {
//             i++;
//         }
//         temp[i] = (*arr)[i];
//     }
//     //기존의 배열 메모리는 새로운 배열에 복사 했으므로
//     //해제하고 새로운 배열 temp를 가리키게 한다.
//     free(*arr);
//     *arr = temp;
//     *s = nNewSize;
// }