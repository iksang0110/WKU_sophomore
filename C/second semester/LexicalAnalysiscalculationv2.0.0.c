#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// **개발 일지**
// **10월 14일**: 어휘 분석 설계 시작
// - 초기 구현에서는 숫자와 연산자를 구분하는 간단한 분석기를 작성.
// - 공백 제거 로직을 포함하지 않아 입력이 단순해야만 작동.
// - 잘못된 문자 입력 처리도 없었음 -> 이후 개선 계획.
// - 초기에는 실수(double)를 처리하지 않고 정수만 처리.

// **10월 16일**: 괄호와 삼각 함수 추가
// - 괄호(`(`, `)`)를 처리하여 중첩된 수식의 해석 가능.
// - `sin`, `cos`, `tan` 함수 인식을 위해 문자열 비교를 도입.
// - 기존 단순 연산자만 처리하던 방식을 확장.

// **10월 20일**: 어휘 분석 확장
// - 실수(double) 처리를 위해 `strtod` 함수 사용.
// - 잘못된 문자 입력에 대한 에러 처리를 추가.
// - 공백 처리 로직 추가 -> 입력 전처리가 필요 없어짐.
// - 함수 호출 시 괄호로 감싸지 않은 경우 에러 출력.

// **11월 19일**: 시각화 기능 추가
// - 파스 트리 구현으로 파싱 과정 시각화
// - BNF 문법 규칙 명시적 정의
// - 고민: 트리 노드 구조 설계 (메모리 vs 가독성)
// - 디버깅 용이성을 위한 상세 출력 추가

// **상수 정의**
#define MAX_RECURSION_DEPTH 100
#define MAX_INPUT_LENGTH 1000
#define MAX_ERROR_MSG_LENGTH 100
#define MAX_NUMBER_LENGTH 32
#define MAX_TREE_WIDTH 80
#define MAX_TREE_HEIGHT 20
#define MAX_STACK_SIZE 100

// **문법 규칙 정의**
const char* GRAMMAR_RULES = 
"BNF 문법 규칙:\n"
"<expr>    ::= <term> [ ('+'|'-') <term> ]*\n"
"<term>    ::= <power> [ ('*'|'/') <power> ]*\n"
"<power>   ::= <factor> [ '^' <factor> ]*\n"
"<factor>  ::= <number> | '(' <expr> ')' | <function> '(' <expr> ')' | '-' <factor>\n"
"<function>::= 'sin' | 'cos' | 'tan' | 'sqrt' | 'log' | 'log10'\n"
"<number>  ::= <digit>+ [ '.' <digit>+ ]? [ ('e'|'E') ['+'|'-']? <digit>+ ]?\n"
"<digit>   ::= '0'|'1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'|'9'\n";

// **초기 토큰 타입 정의 (10월 14일)**
/*
enum TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    END
};
*/

// **확장된 토큰 타입 정의**
enum TokenType {
    NUMBER,     // 숫자 리터럴
    PLUS,       // +
    MINUS,      // -
    MULTIPLY,   // *
    DIVIDE,     // /
    POWER,      // ^
    SQRT,       // sqrt
    LPAREN,     // (
    RPAREN,     // )
    SIN,        // sin
    COS,        // cos
    TAN,        // tan
    LOG,        // log
    LOG10,      // log10
    END         // 입력 종료
};

// **노드 타입 정의**
enum NodeType {
    NT_EXPR,
    NT_TERM,
    NT_POWER,
    NT_FACTOR,
    NT_FUNCTION,
    NT_NUMBER,
    NT_OPERATOR,
    NT_DIGIT
};

// **초기 토큰 구조체 (10월 14일)**
/*
struct Token {
    enum TokenType type;
    int value;  // 정수만 저장
};
*/

// **개선된 토큰 구조체**
struct Token {
    enum TokenType type;    // 토큰 유형
    double value;          // 숫자일 경우의 값
    int position;          // 토큰 위치 (에러 표시용)
    char text[MAX_NUMBER_LENGTH];  // 원본 텍스트 (디버깅용)
};

// **트리 노드 구조체**
struct TreeNode {
    char content[32];     // 노드 내용
    enum NodeType type;   // 노드 타입
    int level;           // 트리에서의 레벨
    int position;        // 수평 위치
    struct TreeNode* left;
    struct TreeNode* right;
};

// **스택 아이템 구조체**
struct StackItem {
    double value;
    char operator;
    int isOperator;  // 0: 숫자, 1: 연산자
};

// **시각화 스택 구조체**
struct VisualizationStack {
    struct StackItem items[MAX_STACK_SIZE];
    int top;
    int showVisualization;  // 시각화 on/off 플래그
};

// **파서 상태 구조체**
struct ParserState {
    char *input;           // 입력 문자열 포인터
    char *original_input;  // 에러 표시를 위한 원본 입력 보관
    int position;          // 현재 처리 위치
    int recursionDepth;    // 현재 재귀 깊이
    struct Token currentToken;  // 현재 처리 중인 토큰
    char errorMsg[MAX_ERROR_MSG_LENGTH];  // 에러 메시지 버퍼
    int errorPosition;     // 에러 발생 위치
    int hadError;         // 에러 발생 여부 플래그
};

// **전역 변수**
struct ParserState state;
struct VisualizationStack vstack = {{0}, -1, 1};
struct TreeNode* currentTree = NULL;
static char nodeBuffer[64];  // getNodeLabel용 정적 버퍼

// **함수 선언**
void initializeState(char *input);
void cleanup(void);
void getNextToken(void);
double expr(void);
double term(void);
double factor(void);
double power(void);
void reportError(const char* message, int position);
double processMathFunction(enum TokenType funcType);
void validateNumber(double value);
void skipWhitespace(void);
int isValidNumberStart(char c);
int isValidIdentifierStart(char c);
int isValidIdentifierPart(char c);
double parseNumber(void);
enum TokenType identifyFunction(const char *str);
struct TreeNode* createNode(const char* content, enum NodeType type);
void visualizeTree(struct TreeNode* root);
void calculateNodePositions(struct TreeNode* node, int level, int left, int right);
void mapTreeToArray(struct TreeNode* node, char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH]);
void freeTree(struct TreeNode* node);
void visualizeStack(void);
void stackPush(double value, char operator, int isOperator);
void stackPop(void);
void printGrammarRules(void);
// **초기 유틸리티 함수들 (10월 14일)**
/*
void skipWhitespace() {
    while (*input == ' ' || *input == '\t') {
        input++;
    }
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}
*/

// **개선된 유틸리티 함수들**
void skipWhitespace(void) {
    while (*state.input && isspace((unsigned char)*state.input)) {
        state.input++;
        state.position++;
    }
}

int isValidNumberStart(char c) {
    return isdigit((unsigned char)c) || c == '.' || c == '-';
}

int isValidIdentifierStart(char c) {
    return isalpha((unsigned char)c) || c == '_';
}

int isValidIdentifierPart(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

// **초기 에러 처리 (10월 14일)**
/*
void reportError(const char* message) {
    printf("Error: %s\n", message);
    exit(1);
}
*/

// **개선된 에러 처리**
void reportError(const char* message, int position) {
    fprintf(stderr, "Error at position %d: %s\n", position, message);
    
    // 입력 문자열 출력
    fprintf(stderr, "%s\n", state.original_input);
    
    // 에러 위치 표시
    int i;
    for (i = 0; i < position; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "^\n");
    
    // 에러 상태 설정
    state.hadError = 1;
    state.errorPosition = position;
    strncpy(state.errorMsg, message, MAX_ERROR_MSG_LENGTH - 1);
    state.errorMsg[MAX_ERROR_MSG_LENGTH - 1] = '\0';
    
    cleanup();
    exit(1);
}

// **초기 상태 초기화 (10월 14일)**
/*
void initializeParser(char *inputText) {
    input = inputText;
    currentToken.type = END;
    currentToken.value = 0;
}
*/

// **개선된 상태 초기화**
void initializeState(char *input) {
    state.input = input;
    // 원본 입력 복사
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
    
    // 초기 토큰 정보 초기화
    state.currentToken.type = END;
    state.currentToken.value = 0;
    state.currentToken.position = 0;
    memset(state.currentToken.text, 0, MAX_NUMBER_LENGTH);
}

// **메모리 정리 함수**
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

// **스택 시각화 함수들**
void stackPush(double value, char operator, int isOperator) {
    if (vstack.top >= MAX_STACK_SIZE - 1) return;
    
    vstack.top++;
    vstack.items[vstack.top].value = value;
    vstack.items[vstack.top].operator = operator;
    vstack.items[vstack.top].isOperator = isOperator;
    
    if (vstack.showVisualization) {
        printf("\n스택 푸시: ");
        if (isOperator) {
            printf("연산자 '%c'", operator);
        } else {
            printf("숫자 %.2f", value);
        }
        visualizeStack();
    }
}

void stackPop(void) {
    if (vstack.top < 0) return;
    
    if (vstack.showVisualization) {
        printf("\n스택 팝: ");
        if (vstack.items[vstack.top].isOperator) {
            printf("연산자 '%c'", vstack.items[vstack.top].operator);
        } else {
            printf("숫자 %.2f", vstack.items[vstack.top].value);
        }
    }
    
    vstack.top--;
    
    if (vstack.showVisualization) {
        visualizeStack();
    }
}

void visualizeStack(void) {
    printf("\n----- 현재 스택 상태 -----\n");
    printf("바닥 [ ");
    int i;
    for (i = 0; i <= vstack.top; i++) {
        if (vstack.items[i].isOperator) {
            printf("'%c' ", vstack.items[i].operator);
        } else {
            printf("%.2f ", vstack.items[i].value);
        }
    }
    printf("] top\n");
    printf("-------------------------\n");
}

// **노드 생성 함수**
struct TreeNode* createNode(const char* content, enum NodeType type) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (node == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    
    strncpy(node->content, content, 31);
    node->content[31] = '\0';
    node->type = type;
    node->level = 0;
    node->position = 0;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

// **노드 레이블 생성 함수**
const char* getNodeLabel(struct TreeNode* node) {
    switch (node->type) {
        case NT_EXPR:
            strcpy(nodeBuffer, "<expr>");
            break;
        case NT_TERM:
            strcpy(nodeBuffer, "<term>");
            break;
        case NT_POWER:
            strcpy(nodeBuffer, "<power>");
            break;
        case NT_FACTOR:
            strcpy(nodeBuffer, "<factor>");
            break;
        case NT_FUNCTION:
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<function>:%s", node->content);
            break;
        case NT_NUMBER:
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<number>:%s", node->content);
            break;
        case NT_OPERATOR:
            strcpy(nodeBuffer, node->content);
            break;
        case NT_DIGIT:
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<digit>:%s", node->content);
            break;
        default:
            strcpy(nodeBuffer, "???");
    }
    return nodeBuffer;
}
// **파스 트리 시각화 함수들**
void calculateNodePositions(struct TreeNode* node, int level, int left, int right) {
    if (!node) return;
    
    node->level = level;
    node->position = (left + right) / 2;
    
    if (node->left)
        calculateNodePositions(node->left, level + 1, left, node->position - 1);
    if (node->right)
        calculateNodePositions(node->right, level + 1, node->position + 1, right);
}

void mapTreeToArray(struct TreeNode* node, char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH]) {
    if (!node) return;
    
    const char* label = getNodeLabel(node);
    int len = strlen(label);
    int start = node->position - len/2;
    int i;
    
    // 노드 레이블 출력
    for (i = 0; i < len && start + i < MAX_TREE_WIDTH; i++) {
        if (start + i >= 0) {
            treeMap[node->level * 3][start + i] = label[i];
        }
    }
    
    // 연결선 그리기
    if (node->left) {
        int x1 = node->position;
        int x2 = node->left->position;
        int y = node->level * 3 + 1;
        int min_x = (x1 < x2) ? x1 : x2;
        int max_x = (x1 > x2) ? x1 : x2;
        
        for (i = min_x; i <= max_x && i < MAX_TREE_WIDTH; i++) {
            if (i >= 0) {
                treeMap[y][i] = (i == x1 || i == x2) ? '|' : '-';
            }
        }
        mapTreeToArray(node->left, treeMap);
    }
    
    if (node->right) {
        int x1 = node->position;
        int x2 = node->right->position;
        int y = node->level * 3 + 1;
        int min_x = (x1 < x2) ? x1 : x2;
        int max_x = (x1 > x2) ? x1 : x2;
        
        for (i = min_x; i <= max_x && i < MAX_TREE_WIDTH; i++) {
            if (i >= 0) {
                treeMap[y][i] = (i == x1 || i == x2) ? '|' : '-';
            }
        }
        mapTreeToArray(node->right, treeMap);
    }
}

void visualizeTree(struct TreeNode* root) {
    if (!root || !vstack.showVisualization) return;
    
    char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH];
    int i, j;
    
    // 배열 초기화
    for (i = 0; i < MAX_TREE_HEIGHT; i++) {
        for (j = 0; j < MAX_TREE_WIDTH; j++) {
            treeMap[i][j] = ' ';
        }
    }
    
    calculateNodePositions(root, 0, 0, MAX_TREE_WIDTH-1);
    mapTreeToArray(root, treeMap);
    
    printf("\n----- 파스 트리 -----\n");
    for (i = 0; i < MAX_TREE_HEIGHT; i++) {
        int hasContent = 0;
        for (j = 0; j < MAX_TREE_WIDTH; j++) {
            if (treeMap[i][j] != ' ') hasContent = 1;
        }
        if (!hasContent) break;
        
        for (j = 0; j < MAX_TREE_WIDTH; j++) {
            printf("%c", treeMap[i][j]);
        }
        printf("\n");
    }
    printf("--------------------\n");
}

// **숫자 유효성 검증**
void validateNumber(double value) {
    if (isnan(value)) {
        reportError("수식 결과가 숫자가 아님 (NaN)", state.position);
    }
    if (isinf(value)) {
        reportError("수식 결과가 무한대", state.position);
    }
}

// **토큰 처리 함수**
void getNextToken(void) {
    skipWhitespace();
    
    if (*state.input == '\0') {
        state.currentToken.type = END;
        state.currentToken.position = state.position;
        return;
    }
    
    // 숫자 처리
    if (isValidNumberStart(*state.input)) {
        state.currentToken.type = NUMBER;
        state.currentToken.position = state.position;
        
        char numStr[MAX_NUMBER_LENGTH] = {0};
        size_t idx = 0;
        
        // 부호 처리
        if (*state.input == '-' || *state.input == '+') {
            numStr[idx++] = *state.input++;
            state.position++;
        }
        
        // 숫자와 소수점, 지수 처리
        while (*state.input && idx < MAX_NUMBER_LENGTH - 1) {
            if (isdigit((unsigned char)*state.input) || 
                *state.input == '.' ||
                *state.input == 'e' || 
                *state.input == 'E' ||
                ((*state.input == '+' || *state.input == '-') && 
                 (*(state.input-1) == 'e' || *(state.input-1) == 'E'))) {
                numStr[idx++] = *state.input++;
                state.position++;
            } else {
                break;
            }
        }
        numStr[idx] = '\0';
        
        // 문자열을 숫자로 변환
        char *endptr;
        state.currentToken.value = strtod(numStr, &endptr);
        
        if (endptr == numStr || *endptr != '\0') {
            reportError("잘못된 숫자 형식", state.position - strlen(numStr));
        }
        
        validateNumber(state.currentToken.value);
        return;
    }
    
    // 함수 이름 처리
    if (isValidIdentifierStart(*state.input)) {
        char name[MAX_NUMBER_LENGTH] = {0};
        size_t idx = 0;
        
        while (*state.input && isValidIdentifierPart(*state.input) && 
               idx < MAX_NUMBER_LENGTH - 1) {
            name[idx++] = *state.input++;
            state.position++;
        }
        name[idx] = '\0';
        
        // 함수 타입 판별
        if (strcmp(name, "sin") == 0) state.currentToken.type = SIN;
        else if (strcmp(name, "cos") == 0) state.currentToken.type = COS;
        else if (strcmp(name, "tan") == 0) state.currentToken.type = TAN;
        else if (strcmp(name, "sqrt") == 0) state.currentToken.type = SQRT;
        else if (strcmp(name, "log") == 0) state.currentToken.type = LOG;
        else if (strcmp(name, "log10") == 0) state.currentToken.type = LOG10;
        else reportError("알 수 없는 함수 또는 상수 이름", 
                        state.position - strlen(name));
        
        state.currentToken.position = state.position - strlen(name);
        return;
    }
    
    // 연산자와 괄호 처리
    state.currentToken.position = state.position;
    switch (*state.input) {
        case '+': state.currentToken.type = PLUS; break;
        case '-': state.currentToken.type = MINUS; break;
        case '*': state.currentToken.type = MULTIPLY; break;
        case '/': state.currentToken.type = DIVIDE; break;
        case '^': state.currentToken.type = POWER; break;
        case '(': state.currentToken.type = LPAREN; break;
        case ')': state.currentToken.type = RPAREN; break;
        default:
            reportError("인식할 수 없는 문자", state.position);
    }
    
    state.input++;
    state.position++;
}
// **수학 함수 처리**
double processMathFunction(enum TokenType funcType) {
    int functionPosition = state.currentToken.position;
    
    // 괄호 확인
    if (state.currentToken.type != LPAREN) {
        reportError("함수 호출에 괄호가 필요합니다", state.position);
    }
    
    getNextToken();  // '(' 건너뛰기
    double arg = expr();
    
    // 닫는 괄호 확인
    if (state.currentToken.type != RPAREN) {
        reportError("닫는 괄호가 필요합니다", state.position);
    }
    
    getNextToken();  // ')' 건너뛰기
    
    // 함수별 처리
    double result;
    switch (funcType) {
        case SIN:
            result = sin(arg);
            break;
        case COS:
            result = cos(arg);
            break;
        case TAN:
            result = tan(arg);
            break;
        case SQRT:
            if (arg < 0) {
                reportError("음수의 제곱근을 계산할 수 없습니다", 
                          functionPosition);
            }
            result = sqrt(arg);
            break;
        case LOG:
            if (arg <= 0) {
                reportError("0 이하의 로그를 계산할 수 없습니다", 
                          functionPosition);
            }
            result = log(arg);
            break;
        case LOG10:
            if (arg <= 0) {
                reportError("0 이하의 로그를 계산할 수 없습니다", 
                          functionPosition);
            }
            result = log10(arg);
            break;
        default:
            reportError("지원하지 않는 함수입니다", functionPosition);
            return 0;  // 도달하지 않음
    }
    
    validateNumber(result);
    return result;
}

// **지수 처리**
double power(void) {
    double result = factor();
    
    while (state.currentToken.type == POWER) {
        int powerPosition = state.currentToken.position;
        getNextToken();  // '^' 건너뛰기
        
        // 우측 결합성을 위해 power() 재귀 호출
        double exponent = power();
        
        // 특수 케이스 처리
        if (result == 0.0 && exponent <= 0.0) {
            reportError("0의 0승 또는 음수 승은 정의되지 않습니다", 
                       powerPosition);
        }
        
        result = pow(result, exponent);
        validateNumber(result);
    }
    
    return result;
}

// **항(term) 처리**
double term(void) {
    double result = power();  // power()가 factor()보다 우선순위 높음
    
    while (state.currentToken.type == MULTIPLY || 
           state.currentToken.type == DIVIDE) {
        enum TokenType op = state.currentToken.type;
        int operatorPosition = state.currentToken.position;
        
        getNextToken();
        double rightOperand = power();
        
        if (op == MULTIPLY) {
            result *= rightOperand;
        } else {  // DIVIDE
            if (rightOperand == 0.0) {
                reportError("0으로 나눌 수 없습니다", operatorPosition);
            }
            result /= rightOperand;
        }
        
        validateNumber(result);
    }
    
    return result;
}

// **인수(factor) 처리**
double factor(void) {
    // 숫자 처리
    if (state.currentToken.type == NUMBER) {
        double value = state.currentToken.value;
        getNextToken();
        return value;
    }
    
    // 괄호 처리
    if (state.currentToken.type == LPAREN) {
        int openPosition = state.currentToken.position;
        getNextToken();  // '(' 건너뛰기
        
        double result = expr();
        
        if (state.currentToken.type != RPAREN) {
            reportError("닫는 괄호가 필요합니다", state.position);
        }
        
        getNextToken();  // ')' 건너뛰기
        return result;
    }
    
    // 수학 함수 처리
    if (state.currentToken.type == SIN || 
        state.currentToken.type == COS || 
        state.currentToken.type == TAN ||
        state.currentToken.type == SQRT ||
        state.currentToken.type == LOG ||
        state.currentToken.type == LOG10) {
        enum TokenType funcType = state.currentToken.type;
        getNextToken();
        return processMathFunction(funcType);
    }
    
    // 단항 마이너스 처리
    if (state.currentToken.type == MINUS) {
        getNextToken();
        return -factor();
    }
    
    // 단항 플러스 처리 (선택적)
    if (state.currentToken.type == PLUS) {
        getNextToken();
        return factor();
    }
    
    reportError("예상치 못한 토큰입니다", state.position);
    return 0;  // 도달하지 않음
}

// **수식(expression) 처리**
double expr(void) {
    struct TreeNode* node = createNode("", NT_EXPR);
    
    if (++state.recursionDepth > MAX_RECURSION_DEPTH) {
        reportError("수식이 너무 복잡합니다 (중첩 깊이 초과)", 
                   state.position);
    }
    
    node->left = createNode("", NT_TERM);
    double result = term();
    
    while (state.currentToken.type == PLUS || 
           state.currentToken.type == MINUS) {
        enum TokenType op = state.currentToken.type;
        char operator = (op == PLUS) ? '+' : '-';
        struct TreeNode* opNode = createNode(&operator, NT_OPERATOR);
        opNode->left = node;
        
        getNextToken();
        opNode->right = createNode("", NT_TERM);
        double right = term();
        
        result = (op == PLUS) ? result + right : result - right;
        node = opNode;
    }
    
    if (vstack.showVisualization) {
        currentTree = node;
        visualizeTree(currentTree);
    }
    
    state.recursionDepth--;
    return result;
}
// **트리 메모리 해제**
void freeTree(struct TreeNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

// **문법 규칙 출력**
void printGrammarRules(void) {
    printf("\n%s\n", GRAMMAR_RULES);
}

// **도움말 출력**
void printHelp(void) {
    printf("\n사용 가능한 명령어:\n");
    printf("  show     - 파싱 과정 시각화 켜기\n");
    printf("  hide     - 파싱 과정 시각화 끄기\n");
    printf("  stack    - 현재 스택 상태 보기\n");
    printf("  grammar  - 문법 규칙 보기\n");
    printf("  help     - 이 도움말 보기\n");
    printf("  quit     - 프로그램 종료\n\n");
    printf("사용 가능한 수학 함수:\n");
    printf("  sin(x)   - 사인 함수\n");
    printf("  cos(x)   - 코사인 함수\n");
    printf("  tan(x)   - 탄젠트 함수\n");
    printf("  sqrt(x)  - 제곱근\n");
    printf("  log(x)   - 자연로그\n");
    printf("  log10(x) - 상용로그\n\n");
    printf("연산자 우선순위:\n");
    printf("  1. 괄호 ()\n");
    printf("  2. 함수 호출\n");
    printf("  3. 지수 ^\n");
    printf("  4. 곱셈 *, 나눗셈 /\n");
    printf("  5. 덧셈 +, 뺄셈 -\n\n");
}

// **메인 함수**
int main(void) {
    char buffer[MAX_INPUT_LENGTH];
    printf("수식 계산기 (도움말을 보려면 'help' 입력)\n");
    printf("수식을 입력하세요 (종료하려면 'quit' 입력)\n\n");
    
    while (1) {
        printf("> ");
        fflush(stdout);
        
        // 안전한 입력 처리
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "입력을 읽는 중 오류가 발생했습니다.\n");
            break;
        }
        
        // 개행 문자 제거
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
            len--;
        }
        
        // 빈 입력 무시
        if (len == 0) {
            continue;
        }
        
        // 명령어 처리
        if (strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) {
            printf("프로그램을 종료합니다.\n");
            break;
        }
        else if (strcmp(buffer, "help") == 0) {
            printHelp();
            continue;
        }
        else if (strcmp(buffer, "show") == 0) {
            vstack.showVisualization = 1;
            printf("파싱 과정 시각화가 활성화되었습니다.\n");
            continue;
        }
        else if (strcmp(buffer, "hide") == 0) {
            vstack.showVisualization = 0;
            printf("파싱 과정 시각화가 비활성화되었습니다.\n");
            continue;
        }
        else if (strcmp(buffer, "grammar") == 0) {
            printGrammarRules();
            continue;
        }
        else if (strcmp(buffer, "stack") == 0) {
            visualizeStack();
            continue;
        }
        
        // 입력 길이 검증
        if (len >= MAX_INPUT_LENGTH - 1) {
            fprintf(stderr, "입력이 너무 깁니다 (최대 %d 문자)\n", 
                    MAX_INPUT_LENGTH - 1);
            continue;
        }
        
        // 계산 수행
        initializeState(buffer);
        state.hadError = 0;
        getNextToken();
        
        if (vstack.showVisualization) {
            printf("\n입력 수식: %s\n", buffer);
            printf("파싱 시작...\n");
        }
        
        double result = expr();
        
        if (!state.hadError) {
            // 남은 입력 확인
            if (state.currentToken.type != END) {
                reportError("수식 끝에 예상치 못한 토큰이 있습니다", 
                           state.currentToken.position);
            }
            
            // 결과 출력
            printf("= %.10g\n", result);
            
            if (vstack.showVisualization) {
                printf("\n계산 완료!\n");
            }
        }
        
        // 정리
        cleanup();
        vstack.top = -1;  // 스택 초기화
    }
    
    return 0;
}

/*
// **향후 개발 계획**

1. 기능 확장
   - 변수 지원 추가 (메모리 기능)
   - 사용자 정의 함수 지원
   - 2진수, 8진수, 16진수 지원
   - 삼각함수의 역함수 (arcsin, arccos, arctan)
   - 쌍곡선함수 (sinh, cosh, tanh)
   - 복소수 연산 지원
   - 단위 변환 기능

2. 사용자 경험 개선
   - 컬러 출력 지원
   - 수식 히스토리 관리
   - 자동 완성 기능
   - 수식 편집 기능
   - 도움말 시스템 확장
   - 대화형 튜토리얼

3. 성능 최적화
   - 토큰 캐싱 구현
   - 메모리 풀링 도입
   - 반복적인 계산 최적화
   - 긴 수식의 분할 처리

4. 안정성 강화
   - 메모리 누수 검사 강화
   - 경계 조건 테스트 확대
   - 예외 처리 개선
   - 부동 소수점 정밀도 개선
   - 입력 유효성 검사 강화

5. 시각화 개선
   - ASCII 아트 품질 향상
   - 계산 단계별 시각화
   - 그래프 출력 기능
   - 수식 구조 다이어그램

6. 코드 구조 개선
   - 모듈화 강화
   - 설정 파일 지원
   - 플러그인 시스템
   - 테스트 케이스 확장
   - 문서화 개선
*/
