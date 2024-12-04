#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// **개발 일지**
// 초기 버전에서는 단순 계산만 가능했지만, 점진적으로 기능을 확장했다.
// 10월 14일: 어휘 분석 설계 시작
// - 숫자와 연산자만 처리하는 기본 버전
// - 공백 처리가 없어서 "1 + 2"같은 입력도 에러났다...
// - 실수 처리를 안 했더니 "1/2"가 0으로 나와서 당황
// - 일단 정수 연산만 구현하고 실수는 다음으로 미룸

// 10월 16일: 괄호와 삼각 함수 추가
// - 괄호 처리 추가 후 "1+(2*3)" 같은 식이 드디어 동작!
// - sin/cos/tan 추가... 근데 라디안 단위를 빼먹어서 한참 헤맴
// - 처음으로 BNF 문법을 도입, 코드가 훨씬 체계적으로 변함

// 11월 19일: 파서 트리 구현
// - 드디어 제대로 된 파스 트리 구현 완료
// - 트리 시각화로 디버깅이 한결 수월해짐
// - valgrind로 메모리 누수 찾아서 수정

// **상수 정의**
#define MAX_RECURSION_DEPTH 100  // 이거 없었을 때 복잡한 수식에서 스택 오버플로우 났었다
#define MAX_INPUT_LENGTH 1000    // 버퍼 오버플로우 방지... 진짜 이거 하
#define MAX_ERROR_MSG_LENGTH 100
#define MAX_NUMBER_LENGTH 32     // float 최대값 고려해서 넉넉히 잡음
#define MAX_TREE_WIDTH 120      // 콘솔 화면 고려한 크기
#define MAX_TREE_HEIGHT 50      // 보통 이정도면 충분
#define MAX_STACK_SIZE 100      // 일반적인 수식은 이정도면 OK

// 유틸리티 매크로
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

// **문법 규칙 정의**
// BNF로 정확히 정의하니까 모호성이 확 줄었다
const char* GRAMMAR_RULES = 
"BNF 문법 규칙:\n"
"<assign>  ::= <id> '=' <expr>\n"
"<expr>    ::= <term> [ ('+'|'-') <term> ]*\n"
"<term>    ::= <factor> [ ('*'|'/') <factor> ]*\n"
"<factor>  ::= <number> | '(' <expr> ')' | <function> '(' <expr> ')' | '-' <factor>\n"
"<function>::= 'sin' | 'cos' | 'tan' | 'sqrt' | 'log' | 'log10'\n"
"<number>  ::= <digit>+\n"
"<digit>   ::= '0'|'1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'|'9'\n"
"<id>      ::= <letter> [ <letter> | <digit> ]*\n";

// **토큰 타입 정의**
// 처음엔 기본 연산자만 있다가 계속 추가됨
enum TokenType {
    NUMBER,     // 숫자 리터럴
    ID,         // 식별자 (변수 기능 추가를 위해 준비)
    PLUS,       // +
    MINUS,      // -
    MULTIPLY,   // *
    DIVIDE,     // /
    ASSIGN,     // = (변수 할당용으로 추가)
    LPAREN,     // (
    RPAREN,     // )
    SIN,        // sin
    COS,        // cos
    TAN,        // tan
    SQRT,       // sqrt (음수 주의!)
    LOG,        // log
    LOG10,      // log10
    END         // 입력 종료
};

// **노드 타입 정의**
// 파스 트리를 위한 노드 타입들
enum NodeType {
    NT_ASSIGN,      // 할당 연산
    NT_ID,          // 식별자
    NT_EXPR,        // 표현식
    NT_TERM,        // 항
    NT_FACTOR,      // 인수
    NT_NUMBER,      // 숫자
    NT_DIGIT,       // 자릿수
    NT_OPERATOR,    // 연산자
    NT_FUNCTION     // 함수
};
// **핵심 구조체 정의**
// 토큰 구조체: 파싱 중인 현재 토큰 정보
struct Token {
    enum TokenType type;          // 토큰 유형
    double value;                 // 숫자값 (NUMBER 타입일 때)
    int position;                 // 에러 위치 표시용
    char text[MAX_NUMBER_LENGTH]; // 원본 텍스트
};

// 트리 노드 구조체: 파스 트리를 구성하는 노드
// 처음엔 이진 트리로 했다가, 연산자를 중간 노드로 표현하기 위해 삼진 트리로 변경
struct TreeNode {
    char content[32];            // 노드 내용
    enum NodeType type;          // 노드 타입
    int level;                   // 트리 레벨 (시각화용)
    int position;                // 수평 위치 (시각화용)
    struct TreeNode* left;       // 왼쪽 자식
    struct TreeNode* middle;     // 중간 자식 (연산자용)
    struct TreeNode* right;      // 오른쪽 자식
};

// 스택 항목 구조체: 계산 과정 추적용
struct StackItem {
    char content[32];            // 내용 (연산자 또는 숫자)
    enum TokenType type;         // 항목 타입
    double value;                // 숫자값
    int isOperator;              // 연산자 여부
};

// 파서 상태 구조체
// 전역 변수 남발하다가 상태 관리를 위해 하나로 묶음
struct ParserState {
    char *input;                 // 현재 처리 중인 입력 포인터
    char *original_input;        // 원본 입력 (에러 표시용)
    int position;                // 현재 위치
    int recursionDepth;          // 재귀 깊이 추적
    struct Token currentToken;   // 현재 토큰
    char errorMsg[MAX_ERROR_MSG_LENGTH];  // 에러 메시지
    int errorPosition;           // 에러 발생 위치
    int hadError;                // 에러 발생 여부
};

// 계산 스택 구조체: 수식 계산 과정 추적
// 디버깅할 때 수식 계산 과정을 보여주기 위해 추가
struct ParserStack {
    struct StackItem items[MAX_STACK_SIZE];
    int top;                     // 스택 최상단 인덱스
    int showVisualization;       // 시각화 on/off
} stack = {{0}, -1, 1};         // 초기값 설정

// **전역 변수**
struct ParserState state;        // 파서 상태
struct TreeNode* currentTree = NULL;  // 현재 파스 트리
static char nodeBuffer[64];      // 노드 레이블 생성용 버퍼

// **함수 선언**
// 파서의 주요 단계들을 함수로 분리
// 초기화/정리 함수
void initializeState(char *input);
void cleanup(void);

// 토큰 처리 함수
void getNextToken(void);

// 문법 규칙별 처리 함수
// BNF 문법을 그대로 코드로 옮김
double expr(void);      // <expr> 규칙 처리
double term(void);      // <term> 규칙 처리
double factor(void);    // <factor> 규칙 처리

// 에러 처리와 검증
void reportError(const char* message, int position);
double processMathFunction(enum TokenType funcType);
void validateNumber(double value);

// 트리 관련 함수
struct TreeNode* createDetailedNode(const char* content, enum NodeType type);
void visualizeDetailedTree(struct TreeNode* root);
void calculateNodePositions(struct TreeNode* node, int level, int left, int right);
void mapDetailedTreeToArray(struct TreeNode* node, char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH]);
void freeTree(struct TreeNode* node);

// 출력 관련 함수
void printGrammarRules(void);
void visualizeStack(void);
void stackPush(const char* content, enum TokenType type, double value, int isOperator);
void stackPop(void);
void printHelp(void);
const char* getDetailedNodeLabel(struct TreeNode* node);
// **초기화 함수**
// 파서 상태 초기화 - 메모리 관리가 중요한 부분
void initializeState(char *input) {
    state.input = input;
    // 원본 입력 복사 (에러 메시지용)
    state.original_input = (char*)malloc(strlen(input) + 1);
    if (state.original_input == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    strcpy(state.original_input, input);
    
    state.position = 0;
    state.recursionDepth = 0;
    state.hadError = 0;
    state.errorPosition = -1;
    memset(state.errorMsg, 0, MAX_ERROR_MSG_LENGTH);
    
    // 토큰 정보 초기화
    state.currentToken.type = END;
    state.currentToken.value = 0;
    state.currentToken.position = 0;
    memset(state.currentToken.text, 0, MAX_NUMBER_LENGTH);
}

// **정리 함수**
// 메모리 누수 방지를 위한 cleanup
void cleanup(void) {
    if (state.original_input != NULL) {
        free(state.original_input);
        state.original_input = NULL;
    }
    if (currentTree != NULL) {
        freeTree(currentTree);
        currentTree = NULL;
    }
}

// **에러 보고 함수**
// 에러 위치를 시각적으로 표시 - 사용자 편의성 고려
void reportError(const char* message, int position) {
    fprintf(stderr, "Error at position %d: %s\n", position, message);
    
    // 입력 문자열과 에러 위치 표시
    fprintf(stderr, "%s\n", state.original_input);
    for (int i = 0; i < position; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "^\n");
    
    state.hadError = 1;
    state.errorPosition = position;
    strncpy(state.errorMsg, message, MAX_ERROR_MSG_LENGTH - 1);
    state.errorMsg[MAX_ERROR_MSG_LENGTH - 1] = '\0';
    
    cleanup();
    exit(1);
}

// **숫자 검증 함수**
// NaN과 무한대 검사 - 수학적 예외 처리
void validateNumber(double value) {
    if (isnan(value)) {
        reportError("수식 결과가 숫자가 아님 (NaN)", state.position);
    }
    if (isinf(value)) {
        reportError("수식 결과가 무한대", state.position);
    }
}

// **스택 처리 함수들**
// 수식 계산 과정 추적용 스택 관리
void stackPush(const char* content, enum TokenType type, double value, int isOperator) {
    if (stack.top >= MAX_STACK_SIZE - 1) return;  // 스택 오버플로우 방지
    
    stack.top++;
    strncpy(stack.items[stack.top].content, content, 31);
    stack.items[stack.top].content[31] = '\0';
    stack.items[stack.top].type = type;
    stack.items[stack.top].value = value;
    stack.items[stack.top].isOperator = isOperator;
    
    // 시각화 옵션이 켜져있을 때만 출력
    if (stack.showVisualization) {
        printf("\n스택 푸시: ");
        if (isOperator) {
            printf("연산자 '%s'", content);
        } else {
            printf("숫자 %.2f", value);
        }
        visualizeStack();
    }
}

void stackPop(void) {
    if (stack.top < 0) return;  // 스택 언더플로우 방지
    
    if (stack.showVisualization) {
        printf("\n스택 팝: ");
        if (stack.items[stack.top].isOperator) {
            printf("연산자 '%s'", stack.items[stack.top].content);
        } else {
            printf("숫자 %.2f", stack.items[stack.top].value);
        }
    }
    
    stack.top--;
    
    if (stack.showVisualization) {
        visualizeStack();
    }
}

// 스택 상태 시각화 - 디버깅에 매우 유용
void visualizeStack(void) {
    printf("\n----- 계산 스택 -----\n");
    printf("바닥 [");
    for (int i = 0; i <= stack.top; i++) {
        if (i > 0) printf("|");
        if (stack.items[i].isOperator) {
            printf(" %s ", stack.items[i].content);
        } else {
            printf(" %.2f ", stack.items[i].value);
        }
    }
    printf("] 최상단\n");
    printf("-------------------\n");
}
// **트리 노드 생성 함수**
// 처음엔 그냥 malloc만 했다가 메모리 초기화 안 해서 한참 고생... 가장 기본인데
// 특히 NULL 포인터 초기화 빼먹어서 세그폴트 엄청 났었다 ㅠㅠ
struct TreeNode* createDetailedNode(const char* content, enum NodeType type) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (!node) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    strncpy(node->content, content, 31);
    node->content[31] = '\0';  // 마지막에 NULL 추가... 이거 빼먹어서 문자열 깨진적도
    node->type = type;
    node->level = 0;
    node->position = 0;
    node->left = NULL;
    node->middle = NULL;  // 이거 하나 빼먹어서 30분 디버깅한 
    node->right = NULL;
    return node;
}

// **노드 레이블 생성 함수**
// 트리 시각화할 때 각 노드의 내용을 어떻게 보여줄지 결정
// snprintf 안 쓰고 sprintf 썼다가 버퍼 오버플로우 난 적도 있다
const char* getDetailedNodeLabel(struct TreeNode* node) {
    switch (node->type) {
        case NT_ASSIGN:
            return "<assign>";  // 아직 구현 안 된 기능
        case NT_ID:
            // 변수 이름과 함께 표시
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<id>\n%s", node->content);
            return nodeBuffer;
        case NT_EXPR:
            return "<expr>";
        case NT_TERM:
            return "<term>";
        case NT_FACTOR:
            return "<factor>";
        case NT_NUMBER:
            // 숫자값도 같이 표시하면 디버깅할 때 편함
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<number>\n%s", node->content);
            return nodeBuffer;
        case NT_DIGIT:
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<digit>\n%s", node->content);
            return nodeBuffer;
        case NT_OPERATOR:
            return node->content;  // 연산자는 그대로 표시
        case NT_FUNCTION:
            // 함수 이름도 표시... sin인지 cos인지 구분 필요
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<function>\n%s", node->content);
            return nodeBuffer;
        default:
            return "???";  // 여기 오면 뭔가 잘못된 거다
    }
}

// **트리 노드 위치 계산 함수**
// 이게 은근 까다로웠다. 노드 간격 조절하느라 한참 씨름
// 처음엔 고정 간격으로 했다가 트리가 겹쳐서 보기 흉했음 근데 지금도 흉함
void calculateNodePositions(struct TreeNode* node, int level, int left, int right) {
    if (!node) return;
    
    // 현재 노드의 레벨과 수평 위치 설정
    node->level = level;
    node->position = (left + right) / 2;  // 중앙 배치
    
    // 자식 노드들 사이 간격 계산
    // 4로 나누는게 가장 보기 좋더라... 시행착오 끝에 찾은 값
    int gap = (right - left) / 4;  
    
    if (node->left)
        calculateNodePositions(node->left, level + 1, left, node->position - gap);
    if (node->middle)  // 연산자 위치
        calculateNodePositions(node->middle, level + 1, 
                             node->position - gap/2, node->position + gap/2);
    if (node->right)
        calculateNodePositions(node->right, level + 1, node->position + gap, right);
}

// **트리를 2D 배열로 매핑하는 함수**
// 처음엔 그냥 printf로 바로 출력했다가, 줄 정렬이 엉망이 되서
// 2D 배열에 먼저 그리고 한번에 출력하는 방식으로 변경
void mapDetailedTreeToArray(struct TreeNode* node, 
                          char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH]) {
    if (!node) return;
    
    const char* label = getDetailedNodeLabel(node);
    int len = strlen(label);
    int start = node->position - len/2;  // 레이블 중앙 정렬
    int y_base = node->level * 3;  // 레벨 간 간격은 3이 적당
    
    // 노드 레이블 출력
    for (int i = 0; i < len && start + i < MAX_TREE_WIDTH; i++) {
        if (start + i >= 0 && label[i] != '\n') {
            treeMap[y_base][start + i] = label[i];
        }
    }
    
    // 왼쪽 자식으로의 연결선 (/)
    if (node->left) {
        int x1 = node->position;
        int x2 = node->left->position;
        // 2칸 높이의 사선이 가장 보기 좋았다
        for (int i = 1; i < 3; i++) {
            int x = x1 - i;
            int y = y_base + i;
            if (x >= 0 && x < MAX_TREE_WIDTH && y < MAX_TREE_HEIGHT) {
                treeMap[y][x] = '/';
            }
        }
        mapDetailedTreeToArray(node->left, treeMap);
    }
    
    // 오른쪽 자식으로의 연결선 (\)
    if (node->right) {
        int x1 = node->position;
        int x2 = node->right->position;
        for (int i = 1; i < 3; i++) {
            int x = x1 + i;
            int y = y_base + i;
            if (x >= 0 && x < MAX_TREE_WIDTH && y < MAX_TREE_HEIGHT) {
                treeMap[y][x] = '\\';
            }
        }
        mapDetailedTreeToArray(node->right, treeMap);
    }
    
    // 중간(연산자) 노드로의 연결선 (|)
    if (node->middle) {
        int x = node->position;
        for (int i = 1; i < 3; i++) {
            int y = y_base + i;
            if (y < MAX_TREE_HEIGHT) {
                treeMap[y][x] = '|';
            }
        }
        mapDetailedTreeToArray(node->middle, treeMap);
    }
}
// **트리 시각화 함수**
// 이거 만드느라 반나절 걸렸는데 디버깅할 때 진짜 유용했다
// ASCII 아트로 트리 그리기... 생각보다 어려웠음
void visualizeDetailedTree(struct TreeNode* root) {
    if (!root || !stack.showVisualization) return;
    
    // 2D 맵 초기화 - 처음엔 동적 할당 했다가 메모리 관리가 너무 복잡해져서 정적으로 변경
    char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH];
    for (int i = 0; i < MAX_TREE_HEIGHT; i++) {
        for (int j = 0; j < MAX_TREE_WIDTH; j++) {
            treeMap[i][j] = ' ';
        }
    }
    
    // 노드 위치 계산하고 맵에 그리기
    calculateNodePositions(root, 0, 0, MAX_TREE_WIDTH-1);
    mapDetailedTreeToArray(root, treeMap);
    
    printf("\n----- 파스 트리 -----\n");
    
    // 필요한 부분만 출력 - 처음엔 전체 배열을 출력했다가 너무 지저분해서 수정
    int hasContent;
    int printed = 0;
    for (int i = 0; i < MAX_TREE_HEIGHT; i++) {
        hasContent = 0;
        for (int j = 0; j < MAX_TREE_WIDTH; j++) {
            if (treeMap[i][j] != ' ') hasContent = 1;
        }
        if (!hasContent) continue;
        
        // 각 줄의 앞뒤 공백 제거 - 이거 없었을 때는 오른쪽에 공백이 잔뜩...
        int start = 0, end = MAX_TREE_WIDTH - 1;
        while (start < MAX_TREE_WIDTH && treeMap[i][start] == ' ') start++;
        while (end >= 0 && treeMap[i][end] == ' ') end--;
        
        if (start <= end) {
            printf("    ");  // 보기 좋게 좀 들여쓰기
            for (int j = start; j <= end; j++) {
                printf("%c", treeMap[i][j]);
            }
            printf("\n");
            printed++;
        }
    }
    printf("------------------------\n");
}

// **트리 메모리 해제 함수**
// 처음엔 이거 없어서 메모리 누수 심각했다... valgrind의 도움이 컸음
void freeTree(struct TreeNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->middle);  // middle도 해제해야 하는거 한참 뒤에 깨달음
    freeTree(node->right);
    free(node);
}

// **토큰 처리 함수**
// 렉서(lexer)의 핵심 부분... 생각보다 복잡한 로직이 필요했다
void getNextToken(void) {
    // 공백 문자 건너뛰기
    // isspace() 안 쓰고 직접 비교하다가 탭문자 때문에 고생...
    while (*state.input && isspace((unsigned char)*state.input)) {
        state.input++;
        state.position++;
    }
    
    // 입력 종료 체크
    if (*state.input == '\0') {
        state.currentToken.type = END;
        state.currentToken.position = state.position;
        return;
    }
    
    // 숫자 처리
    // 실수 처리 추가하면서 복잡해짐... 처음엔 정수만 했었는데
    if (isdigit((unsigned char)*state.input)) {
        state.currentToken.type = NUMBER;
        state.currentToken.position = state.position;
        
        char numStr[MAX_NUMBER_LENGTH] = {0};
        int idx = 0;
        
        // 숫자랑 소수점 수집
        // idx < MAX_NUMBER_LENGTH - 1 이거 빼먹어서 버퍼 오버플로우 났었다...
        while (*state.input && 
               (isdigit((unsigned char)*state.input) || *state.input == '.') && 
               idx < MAX_NUMBER_LENGTH - 1) {
            numStr[idx++] = *state.input++;
            state.position++;
        }
        numStr[idx] = '\0';
        
        // strtod가 atof보다 에러 처리가 좋아서 이걸로 변경
        state.currentToken.value = strtod(numStr, NULL);
        strncpy(state.currentToken.text, numStr, MAX_NUMBER_LENGTH - 1);
        return;
    }
    
    // 식별자와 함수 처리
    // 처음엔 switch문으로 했다가 나중에 문자열 비교로 변경
    if (isalpha((unsigned char)*state.input)) {
        char name[MAX_NUMBER_LENGTH] = {0};
        int idx = 0;
        
        // 알파벳, 숫자, 언더스코어 허용
        while (*state.input && 
               (isalnum((unsigned char)*state.input) || *state.input == '_') && 
               idx < MAX_NUMBER_LENGTH - 1) {
            name[idx++] = *state.input++;
            state.position++;
        }
        name[idx] = '\0';
        
        // 함수 이름 매칭
        // log10은 log보다 먼저 검사해야 함! 순서 중요
        if (strcmp(name, "sin") == 0) state.currentToken.type = SIN;
        else if (strcmp(name, "cos") == 0) state.currentToken.type = COS;
        else if (strcmp(name, "tan") == 0) state.currentToken.type = TAN;
        else if (strcmp(name, "sqrt") == 0) state.currentToken.type = SQRT;
        else if (strcmp(name, "log10") == 0) state.currentToken.type = LOG10;  // 이게 먼저!
        else if (strcmp(name, "log") == 0) state.currentToken.type = LOG;
        else {
            state.currentToken.type = ID;  // 향후 변수 지원을 위해
            strncpy(state.currentToken.text, name, MAX_NUMBER_LENGTH - 1);
        }
        
        state.currentToken.position = state.position - strlen(name);
        return;
    }
    
    // 연산자와 괄호 처리
    // 간단해 보여도 위치 정보 때문에 실수하기 쉬운 부분
    state.currentToken.position = state.position;
    switch (*state.input) {
        case '+': state.currentToken.type = PLUS; break;
        case '-': state.currentToken.type = MINUS; break;
        case '*': state.currentToken.type = MULTIPLY; break;
        case '/': state.currentToken.type = DIVIDE; break;
        case '(': state.currentToken.type = LPAREN; break;
        case ')': state.currentToken.type = RPAREN; break;
        case '=': state.currentToken.type = ASSIGN; break;
        default:
            reportError("인식할 수 없는 문자", state.position);
    }
    
    state.currentToken.text[0] = *state.input;
    state.currentToken.text[1] = '\0';
    state.input++;
    state.position++;
}
// **수학 함수 처리**
// 삼각함수/로그/제곱근 등의 수학 함수 처리
// 함수 인자 범위 체크가 은근 까다로웠다
double processMathFunction(enum TokenType funcType) {
    int functionPosition = state.currentToken.position;
    
    // 괄호 확인 - "sin 30" 같은 잘못된 입력 방지
    if (state.currentToken.type != LPAREN) {
        reportError("함수 호출에 괄호가 필요합니다", state.position);
    }
    
    // 인자 계산 - 재귀적으로 expr() 호출
    getNextToken();
    double arg = expr();  // expr()이 복잡한 수식도 처리해줌 (예: sin(1+2*3))
    
    // 닫는 괄호 확인
    if (state.currentToken.type != RPAREN) {
        reportError("닫는 괄호가 필요합니다", state.position);
    }
    
    getNextToken();
    
    // 함수별 처리 - 특수한 경우(음수, 0 등) 처리 주의
    double result;
    switch (funcType) {
        case SIN: 
            result = sin(arg);  // 라디안 단위! 처음에 이거 모르고 한참 삽질...
            break;
        case COS: 
            result = cos(arg); 
            break;
        case TAN: 
            result = tan(arg);  // π/2 근처에서 발산 주의
            break;
        case SQRT:
            if (arg < 0) {
                // 제곱근의 정의역 체크
                reportError("음수의 제곱근을 계산할 수 없습니다", functionPosition);
            }
            result = sqrt(arg);
            break;
        case LOG:
            if (arg <= 0) {
                // 로그의 정의역 체크
                reportError("0 이하의 로그를 계산할 수 없습니다", functionPosition);
            }
            result = log(arg);  // 자연로그
            break;
        case LOG10:
            if (arg <= 0) {
                reportError("0 이하의 로그를 계산할 수 없습니다", functionPosition);
            }
            result = log10(arg);  // 상용로그
            break;
        default:
            reportError("지원하지 않는 함수입니다", functionPosition);
            return 0;
    }
    
    // NaN이나 Inf 체크 - 이거 빼먹었다가 프로그램이 이상하게 종료되는 경우 있었음
    validateNumber(result);
    return result;
}

// **식(expression) 처리 함수**
// 파서의 핵심 - BNF의 <expr> 규칙 구현
// 덧셈과 뺄셈을 처리하는 최상위 파서
double expr(void) {
    // 재귀 깊이 제한 - 이거 없었을 때 스택 오버플로우로 뻗은 적 많음...
    if (++state.recursionDepth > MAX_RECURSION_DEPTH) {
        reportError("수식이 너무 복잡합니다", state.position);
    }

    // 파스 트리 노드 생성
    struct TreeNode* exprNode = createDetailedNode("", NT_EXPR);
    double result = term();  // 먼저 term() 처리 (곱셈/나눗셈)
    
    // 결과를 트리와 스택에 추가
    char valueStr[32];
    snprintf(valueStr, sizeof(valueStr), "%.2f", result);
    exprNode->left = createDetailedNode(valueStr, NT_TERM);
    stackPush(valueStr, NUMBER, result, 0);

    // 덧셈/뺄셈 연산자가 있는 동안 반복
    while (state.currentToken.type == PLUS || state.currentToken.type == MINUS) {
        enum TokenType op = state.currentToken.type;
        char opStr[2] = {op == PLUS ? '+' : '-', '\0'};
        
        // 새 트리 노드 생성 - 기존 트리를 왼쪽 자식으로
        struct TreeNode* newExpr = createDetailedNode("", NT_EXPR);
        struct TreeNode* opNode = createDetailedNode(opStr, NT_OPERATOR);
        
        newExpr->left = exprNode;
        newExpr->middle = opNode;
        
        stackPush(opStr, op, 0.0, 1);
        getNextToken();
        
        // 우측 항 처리
        double rightVal = term();
        snprintf(valueStr, sizeof(valueStr), "%.2f", rightVal);
        newExpr->right = createDetailedNode(valueStr, NT_TERM);
        stackPush(valueStr, NUMBER, rightVal, 0);
        
        // 계산 실행
        stackPop(); // rightVal
        stackPop(); // operator
        stackPop(); // result
        
        result = (op == PLUS) ? result + rightVal : result - rightVal;
        snprintf(valueStr, sizeof(valueStr), "%.2f", result);
        stackPush(valueStr, NUMBER, result, 0);
        
        exprNode = newExpr;
    }

    // 시각화 옵션이 켜져있으면 트리 출력
    if (stack.showVisualization) {
        currentTree = exprNode;
        visualizeDetailedTree(currentTree);
    }

    state.recursionDepth--;
    return result;
}
// **항(term) 처리 함수**
// BNF의 <term> 규칙 구현 - 곱셈과 나눗셈 처리
// 우선순위가 덧셈/뺄셈보다 높아서 expr()보다 먼저 실행됨
double term(void) {
    double result = factor();  // 먼저 factor() 처리 (괄호, 함수 등)
    struct TreeNode* termNode = createDetailedNode("", NT_TERM);
    
    // 결과를 트리에 추가
    char valueStr[32];
    snprintf(valueStr, sizeof(valueStr), "%.2f", result);
    termNode->left = createDetailedNode(valueStr, NT_FACTOR);

    // 곱셈/나눗셈 연산자 처리
    while (state.currentToken.type == MULTIPLY || state.currentToken.type == DIVIDE) {
        enum TokenType op = state.currentToken.type;
        char opStr[2] = {op == MULTIPLY ? '*' : '/', '\0'};
        
        // 새 트리 노드 생성
        struct TreeNode* newTerm = createDetailedNode("", NT_TERM);
        struct TreeNode* opNode = createDetailedNode(opStr, NT_OPERATOR);
        
        newTerm->left = termNode;
        newTerm->middle = opNode;
        
        stackPush(opStr, op, 0.0, 1);
        getNextToken();
        
        // 우측 인수 처리
        double rightVal = factor();
        
        // 0으로 나누기 검사... 이거 한번 빼먹어서 프로그램 크래시났었다
        if (op == DIVIDE && rightVal == 0) {
            reportError("0으로 나눌 수 없습니다", state.position - 1);
        }
        
        snprintf(valueStr, sizeof(valueStr), "%.2f", rightVal);
        newTerm->right = createDetailedNode(valueStr, NT_FACTOR);
        stackPush(valueStr, NUMBER, rightVal, 0);
        
        // 계산 실행
        stackPop(); // rightVal
        stackPop(); // operator
        stackPop(); // result
        
        result = (op == MULTIPLY) ? result * rightVal : result / rightVal;
        snprintf(valueStr, sizeof(valueStr), "%.2f", result);
        stackPush(valueStr, NUMBER, result, 0);
        
        termNode = newTerm;
    }

    return result;
}

// **인수(factor) 처리 함수**
// BNF의 <factor> 규칙 구현
// 숫자, 괄호, 단항 연산자, 함수 호출 처리
double factor(void) {
    // 숫자 리터럴 처리
    if (state.currentToken.type == NUMBER) {
        double value = state.currentToken.value;
        struct TreeNode* node = createDetailedNode(state.currentToken.text, NT_NUMBER);
        getNextToken();
        return value;
    }

    // 괄호로 묶인 수식 처리
    if (state.currentToken.type == LPAREN) {
        getNextToken();
        struct TreeNode* node = createDetailedNode("()", NT_FACTOR);
        double result = expr();  // 재귀적으로 expr() 호출
        
        // 괄호 매칭 검사
        if (state.currentToken.type != RPAREN) {
            reportError("닫는 괄호가 필요합니다", state.position);
        }
        
        getNextToken();
        return result;
    }

    // 단항 마이너스 처리
    // -1, -(2+3) 같은 형태 처리 가능
    if (state.currentToken.type == MINUS) {
        getNextToken();
        return -factor();  // 재귀적으로 처리
    }

    // 함수 호출 처리
    if (state.currentToken.type >= SIN && state.currentToken.type <= LOG10) {
        enum TokenType funcType = state.currentToken.type;
        struct TreeNode* node = createDetailedNode(state.currentToken.text, NT_FUNCTION);
        getNextToken();
        return processMathFunction(funcType);
    }

    // 여기까지 왔다면 뭔가 잘못된 것
    reportError("예상치 못한 토큰입니다", state.position);
    return 0;
}

// **문법 규칙 출력 함수**
void printGrammarRules(void) {
    printf("\n%s\n", GRAMMAR_RULES);
}

// **도움말 출력 함수**
void printHelp(void) {
    printf("\n사용 가능한 연산:\n");
    printf("  + - * /   : 기본 산술 연산\n");
    printf("  ()        : 괄호\n");
    printf("  sin cos tan : 삼각함수 (라디안 단위 주의!)\n");
    printf("  sqrt      : 제곱근 (음수 주면 에러)\n");
    printf("  log log10 : 로그함수 (0 이하 주면 에러)\n\n");
    printf("명령어:\n");
    printf("  show      : 파싱 과정 시각화 켜기\n");
    printf("  hide      : 파싱 과정 시각화 끄기\n");
    printf("  grammar   : 문법 규칙 보기\n");
    printf("  help      : 이 도움말 보기\n");
    printf("  quit      : 프로그램 종료\n\n");
}

// **메인 함수**
int main(void) {
    char buffer[MAX_INPUT_LENGTH];
    printf("수식 계산기 v3.0.0 - 파스 트리 시각화 포함\n");
    printf("명령어:\n");
    printf("  help     - 도움말 보기\n");
    printf("  show     - 파싱 과정 시각화 켜기 (느려질 수 있음)\n");
    printf("  hide     - 파싱 과정 시각화 끄기\n");
    printf("  grammar  - 문법 규칙 보기\n");
    printf("  quit     - 프로그램 종료\n\n");

    while (1) {
        printf("\n> ");
        fflush(stdout);  // Windows에서 버퍼링 문제 방지

        // 입력 받기
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "입력 오류\n");
            break;
        }

        // 개행 문자 제거 - 이거 없으면 이상한 에러 발생
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
            len--;
        }

        // 빈 입력 처리
        if (len == 0) continue;

        // 명령어 처리
        if (strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) {
            printf("프로그램을 종료합니다.\n");
            break;
        } else if (strcmp(buffer, "help") == 0) {
            printHelp();
            continue;
        } else if (strcmp(buffer, "show") == 0) {
            stack.showVisualization = 1;
            printf("파싱 과정 시각화가 활성화되었습니다.\n");
            continue;
        } else if (strcmp(buffer, "hide") == 0) {
            stack.showVisualization = 0;
            printf("파싱 과정 시각화가 비활성화되었습니다.\n");
            continue;
        } else if (strcmp(buffer, "grammar") == 0) {
            printGrammarRules();
            continue;
        }

        // 입력 길이 검증
        if (len >= MAX_INPUT_LENGTH - 1) {
            fprintf(stderr, "입력이 너무 깁니다 (최대 %d 문자)\n", MAX_INPUT_LENGTH - 1);
            continue;
        }

        // 계산 수행
        initializeState(buffer);
        state.hadError = 0;
        stack.top = -1;  // 스택 초기화
        
        if (stack.showVisualization) {
            printf("\n입력 수식: %s\n", buffer);
            printf("파싱 시작...\n");
        }

        getNextToken();
        double result = expr();

        // 결과 출력 또는 에러 처리
        if (!state.hadError) {
            if (state.currentToken.type != END) {
                reportError("수식 끝에 예상치 못한 토큰이 있습니다", state.currentToken.position);
            } else {
                printf("= %.10g\n", result);
            }
        }

        cleanup();
    }

    return 0;
}