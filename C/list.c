// #include <stdio.h>
// #include <stdlib.h>

// // Node 구조체 정의
// typedef struct Node {
//     int data;
//     struct Node* link;
// } Node;

// // 새 Node를 생성하는 함수
// Node* getNode(int x) {
//     Node* newNode = (Node*)malloc(sizeof(Node));
//     if(newNode == NULL) {
//         printf("메모리 할당 실패\n");
//         exit(1); // 메모리 할당 실패 시 종료
//     }
//     newNode->data = x; // 데이터 부분 설정
//     newNode->link = NULL; // 초기 다음 링크는 NULL
//     return newNode;
// }

// // 리스트의 시작 부분에 새 노드를 삽입하는 함수
// void insertFirstNode(Node** L, int x) {
//     Node* new = getNode(x); // 데이터 x로 새 노드 생성
//     new->link = *L; // 새 노드를 현재 첫 번째 노드에 연결
//     *L = new; // 리스트의 헤드를 새 노드로 업데이트
// }
// void searchNode(Node** L, int x) {
//     Node* new = 
// }

// // 예제 사용
// int main() {
//     Node* L = NULL; // 빈 리스트로 시작

//     // 시작 부분에 노드 삽입
//     insertFirstNode(&L, 10);
//     insertFirstNode(&L, 20);
//     insertFirstNode(&L, 30);

//     // 검증을 위해 리스트 출력
//     Node* current = L;
//     while(current != NULL) {
//         printf("%d -> ", current->data);
//         current = current->link;
//     }
//     printf("NULL\n");

//     return 0;
// }

// in

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode {
    char data[4];
    struct ListNode* link;

}ListNode;

typedef struct {
    listNode* head;

}linkedList_h;

linkedList_h* craeteLinkedList_h(void) {
    linkedList_h* L;
    L= (linkedList_h*)
}
