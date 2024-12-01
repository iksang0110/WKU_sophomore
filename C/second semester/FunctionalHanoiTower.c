#include <stdio.h>
#include <stdlib.h>

// 데이터 구조 정의
typedef struct Node {
    int disk;
    struct Node* next;
} Node;

typedef struct Tower {
    Node* top;
    char name;
} Tower;

// 순수 함수: 새로운 노드 생성
Node* create_node(int disk) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->disk = disk;
    new_node->next = NULL;
    return new_node;
}

// 순수 함수: 새로운 타워 생성
Tower* create_tower(char name) {
    Tower* tower = (Tower*)malloc(sizeof(Tower));
    tower->top = NULL;
    tower->name = name;
    return tower;
}

// 순수 함수: 타워의 복사본 생성
Tower* copy_tower(Tower* src) {
    Tower* new_tower = create_tower(src->name);
    if (src->top == NULL) return new_tower;
    
    Node* current = src->top;
    Node* new_top = create_node(current->disk);
    new_tower->top = new_top;
    
    while (current->next != NULL) {
        current = current->next;
        new_top->next = create_node(current->disk);
        new_top = new_top->next;
    }
    
    return new_tower;
}

// 순수 함수: 디스크 추가
Tower* push_disk(Tower* tower, int disk) {
    Tower* new_tower = copy_tower(tower);
    Node* new_node = create_node(disk);
    new_node->next = new_tower->top;
    new_tower->top = new_node;
    return new_tower;
}

// 순수 함수: 디스크 제거
Tower* pop_disk(Tower* tower) {
    if (tower->top == NULL) return tower;
    
    Tower* new_tower = create_tower(tower->name);
    new_tower->top = tower->top->next;
    return new_tower;
}

// 순수 함수: 디스크 이동
void move_disk(Tower** src, Tower** aux, Tower** dest) {
    if ((*src)->top == NULL) return;
    
    int disk = (*src)->top->disk;
    *src = pop_disk(*src);
    *dest = push_disk(*dest, disk);
    
    printf("원판 %d을(를) %c 기둥에서 %c 기둥으로 이동\n", disk, (*src)->name, (*dest)->name);
}

// 순수 함수: 타워 상태 출력
void print_tower_state(Tower* src, Tower* aux, Tower* dest) {
    printf("\n현재 탑 상태:\n");
    printf("기둥 %c: ", src->name);
    Node* current = src->top;
    while (current != NULL) {
        printf("%d ", current->disk);
        current = current->next;
    }
    
    printf("\n기둥 %c: ", aux->name);
    current = aux->top;
    while (current != NULL) {
        printf("%d ", current->disk);
        current = current->next;
    }
    
    printf("\n기둥 %c: ", dest->name);
    current = dest->top;
    while (current != NULL) {
        printf("%d ", current->disk);
        current = current->next;
    }
    printf("\n");
}

// 재귀적 하노이 해결 함수
void solve_hanoi(int n, Tower** src, Tower** aux, Tower** dest) {
    if (n == 1) {
        move_disk(src, aux, dest);
        print_tower_state(*src, *aux, *dest);
        return;
    }
    
    solve_hanoi(n - 1, src, dest, aux);
    move_disk(src, aux, dest);
    print_tower_state(*src, *aux, *dest);
    solve_hanoi(n - 1, aux, src, dest);
}

// 초기 타워 설정 함수
Tower* initialize_source_tower(int n, char name) {
    Tower* tower = create_tower(name);
    for (int i = n; i >= 1; i--) {
        tower = push_disk(tower, i);
    }
    return tower;
}

// 메모리 해제 함수
void free_tower(Tower* tower) {
    Node* current = tower->top;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(tower);
}

int main() {
    int move_count = 0;  // 이동 횟수를 카운트하기 위한 변수
    int n;
    printf("원판의 개수를 입력하세요: ");
    scanf("%d", &n);
    
    // 초기 타워 생성
    Tower* src = initialize_source_tower(n, 'A');
    Tower* aux = create_tower('B');
    Tower* dest = create_tower('C');
    
    printf("\n초기 상태:");
    print_tower_state(src, aux, dest);
    
    // 하노이 탑 해결
    // 총 이동 횟수 계산 (2^n - 1)
    int total_moves = (1 << n) - 1;
    printf("\n총 %d번의 이동이 필요합니다.\n\n", total_moves);
    
    solve_hanoi(n, &src, &aux, &dest);
    
    printf("\n모든 원판이 성공적으로 이동되었습니다!\n");
    
    // 메모리 해제
    free_tower(src);
    free_tower(aux);
    free_tower(dest);
    
    return 0;
}

typedef struct TowerState {
    Tower* src;
    Tower* aux;
    Tower* dst;
} TowerState;


