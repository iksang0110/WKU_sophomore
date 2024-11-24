#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TREE_WIDTH 80
#define MAX_TREE_HEIGHT 20

// 토큰 타입 정의
enum TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    END
};

// 트리 노드 정의
struct TreeNode {
    char content[32];
    struct TreeNode* left;
    struct TreeNode* right;
    int level;
    int position;
};

// 전역 변수
const char* GRAMMAR_RULES = 
"현재 계산기에 적용된 문법 (BNF):\n"
"<expr> ::= <term> [ ('+' | '-') <term> ]*\n"
"<term> ::= <factor> [ ('*' | '/') <factor> ]*\n"
"<factor> ::= <number> | '(' <expr> ')'";

struct TreeNode* createNode(const char* content);
void visualizeTree(struct TreeNode* root);
void calculateNodePositions(struct TreeNode* node, int level, int left, int right);
void mapTreeToArray(struct TreeNode* node, char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH]);
void freeTree(struct TreeNode* node);
void printGrammarRules(void);

// 트리 노드 생성
struct TreeNode* createNode(const char* content) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (!node) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    strncpy(node->content, content, sizeof(node->content) - 1);
    node->content[sizeof(node->content) - 1] = '\0';
    node->left = node->right = NULL;
    return node;
}

// 트리 시각화
void visualizeTree(struct TreeNode* root) {
    char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH];
    for (int i = 0; i < MAX_TREE_HEIGHT; i++) {
        for (int j = 0; j < MAX_TREE_WIDTH; j++) {
            treeMap[i][j] = ' ';
        }
    }

    calculateNodePositions(root, 0, 0, MAX_TREE_WIDTH - 1);
    mapTreeToArray(root, treeMap);

    printf("\n----- 파스트리 -----\n");
    for (int i = 0; i < MAX_TREE_HEIGHT; i++) {
        if (treeMap[i][0] == '\0') break;
        printf("%s\n", treeMap[i]);
    }
    printf("--------------------\n");
}

// 트리 노드 위치 계산
void calculateNodePositions(struct TreeNode* node, int level, int left, int right) {
    if (!node) return;
    node->level = level;
    node->position = (left + right) / 2;
    calculateNodePositions(node->left, level + 1, left, node->position);
    calculateNodePositions(node->right, level + 1, node->position, right);
}

// 트리 데이터를 ASCII 배열에 매핑
void mapTreeToArray(struct TreeNode* node, char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH]) {
    if (!node) return;
    int pos = node->position;
    snprintf(treeMap[node->level] + pos, MAX_TREE_WIDTH - pos, "%s", node->content);
    mapTreeToArray(node->left, treeMap);
    mapTreeToArray(node->right, treeMap);
}

// 트리 메모리 해제
void freeTree(struct TreeNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

// 문법 출력
void printGrammarRules(void) {
    printf("\n%s\n", GRAMMAR_RULES);
}

// 메인 함수
int main(void) {
    printf("계산기 - 문법 및 파스트리 출력 테스트\n");
    printf("BNF 문법을 출력합니다:\n");
    printGrammarRules();

    // 간단한 트리 생성 및 출력 테스트
    struct TreeNode* root = createNode("<expr>");
    root->left = createNode("<term>");
    root->right = createNode("+");
    root->right->right = createNode("<term>");
    root->right->right->left = createNode("<factor>");
    root->right->right->left->left = createNode("<number>");
    strncpy(root->right->right->left->left->content, "5", sizeof(root->right->right->left->left->content));

    visualizeTree(root);
    freeTree(root);
    return 0;
}
