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

// **11월 19일**: 파서 트리 구현
// - BNF 문법 기반의 상세한 파스 트리 구현
// - 각 노드가 문법 요소를 정확히 표현하도록 개선
// - 트리 시각화 알고리즘 최적화
// - 메모리 관리 개선

// **상수 정의**
#define MAX_RECURSION_DEPTH 100
#define MAX_INPUT_LENGTH 1000
#define MAX_ERROR_MSG_LENGTH 100
#define MAX_NUMBER_LENGTH 32
#define MAX_TREE_WIDTH 120
#define MAX_TREE_HEIGHT 50
#define MAX_STACK_SIZE 100

// 유틸리티 매크로
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

// **문법 규칙 정의**
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
enum TokenType {
    NUMBER,     // 숫자 리터럴
    ID,         // 식별자
    PLUS,       // +
    MINUS,      // -
    MULTIPLY,   // *
    DIVIDE,     // /
    ASSIGN,     // =
    LPAREN,     // (
    RPAREN,     // )
    SIN,        // sin
    COS,        // cos
    TAN,        // tan
    SQRT,       // sqrt
    LOG,        // log
    LOG10,      // log10
    END         // 입력 종료
};

// **노드 타입 정의**
enum NodeType {
    NT_ASSIGN,      // 할당
    NT_ID,          // 식별자
    NT_EXPR,        // 표현식
    NT_TERM,        // 항
    NT_FACTOR,      // 인수
    NT_NUMBER,      // 숫자
    NT_DIGIT,       // 자릿수
    NT_OPERATOR,    // 연산자
    NT_FUNCTION     // 함수
};

// **토큰 구조체**
struct Token {
    enum TokenType type;
    double value;
    int position;
    char text[MAX_NUMBER_LENGTH];
};

// **트리 노드 구조체**
struct TreeNode {
    char content[32];
    enum NodeType type;
    int level;
    int position;
    struct TreeNode* left;
    struct TreeNode* middle;
    struct TreeNode* right;
};

// **스택 항목 구조체**
struct StackItem {
    char content[32];
    enum TokenType type;
    double value;
    int isOperator;
};

// **파서 상태 구조체**
struct ParserState {
    char *input;
    char *original_input;
    int position;
    int recursionDepth;
    struct Token currentToken;
    char errorMsg[MAX_ERROR_MSG_LENGTH];
    int errorPosition;
    int hadError;
};

// **스택 구조체**
struct ParserStack {
    struct StackItem items[MAX_STACK_SIZE];
    int top;
    int showVisualization;
} stack = {{0}, -1, 1};

// **전역 변수**
struct ParserState state;
struct TreeNode* currentTree = NULL;
static char nodeBuffer[64];

// **함수 선언**
void initializeState(char *input);
void cleanup(void);
void getNextToken(void);
double expr(void);
double term(void);
double factor(void);
void reportError(const char* message, int position);
double processMathFunction(enum TokenType funcType);
void validateNumber(double value);
struct TreeNode* createDetailedNode(const char* content, enum NodeType type);
void visualizeDetailedTree(struct TreeNode* root);
void calculateNodePositions(struct TreeNode* node, int level, int left, int right);
void mapDetailedTreeToArray(struct TreeNode* node, char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH]);
void freeTree(struct TreeNode* node);
void printGrammarRules(void);
void visualizeStack(void);
void stackPush(const char* content, enum TokenType type, double value, int isOperator);
void stackPop(void);
void printHelp(void);
const char* getDetailedNodeLabel(struct TreeNode* node);
// **초기화 함수**
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

// **정리 함수**
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
void reportError(const char* message, int position) {
    fprintf(stderr, "Error at position %d: %s\n", position, message);
    
    // 입력 문자열 출력
    fprintf(stderr, "%s\n", state.original_input);
    
    // 에러 위치 표시
    for (int i = 0; i < position; i++) {
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

// **숫자 검증 함수**
void validateNumber(double value) {
    if (isnan(value)) {
        reportError("수식 결과가 숫자가 아님 (NaN)", state.position);
    }
    if (isinf(value)) {
        reportError("수식 결과가 무한대", state.position);
    }
}

// **스택 처리 함수들**
void stackPush(const char* content, enum TokenType type, double value, int isOperator) {
    if (stack.top >= MAX_STACK_SIZE - 1) return;
    
    stack.top++;
    strncpy(stack.items[stack.top].content, content, 31);
    stack.items[stack.top].content[31] = '\0';
    stack.items[stack.top].type = type;
    stack.items[stack.top].value = value;
    stack.items[stack.top].isOperator = isOperator;
    
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
    if (stack.top < 0) return;
    
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

// **트리 노드 생성 및 레이블 함수들**
struct TreeNode* createDetailedNode(const char* content, enum NodeType type) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (!node) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    strncpy(node->content, content, 31);
    node->content[31] = '\0';
    node->type = type;
    node->level = 0;
    node->position = 0;
    node->left = NULL;
    node->middle = NULL;
    node->right = NULL;
    return node;
}

const char* getDetailedNodeLabel(struct TreeNode* node) {
    switch (node->type) {
        case NT_ASSIGN:
            return "<assign>";
        case NT_ID:
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<id>\n%s", node->content);
            return nodeBuffer;
        case NT_EXPR:
            return "<expr>";
        case NT_TERM:
            return "<term>";
        case NT_FACTOR:
            return "<factor>";
        case NT_NUMBER:
            return "<number>";
        case NT_DIGIT:
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<digit>\n%s", node->content);
            return nodeBuffer;
        case NT_OPERATOR:
            return node->content;
        case NT_FUNCTION:
            snprintf(nodeBuffer, sizeof(nodeBuffer), "<function>\n%s", node->content);
            return nodeBuffer;
        default:
            return "???";
    }
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
    printf("  sin cos tan : 삼각함수\n");
    printf("  sqrt      : 제곱근\n");
    printf("  log log10 : 로그함수\n\n");
    printf("명령어:\n");
    printf("  show      : 파싱 과정 시각화 켜기\n");
    printf("  hide      : 파싱 과정 시각화 끄기\n");
    printf("  grammar   : 문법 규칙 보기\n");
    printf("  help      : 이 도움말 보기\n");
    printf("  quit      : 프로그램 종료\n\n");
}
// **파스 트리 시각화 관련 함수들**
void calculateNodePositions(struct TreeNode* node, int level, int left, int right) {
    if (!node) return;
    
    node->level = level;
    node->position = (left + right) / 2;
    
    int gap = (right - left) / 4;  // 자식 노드 간격 조정
    
    if (node->left)
        calculateNodePositions(node->left, level + 1, left, node->position - gap);
    if (node->middle)
        calculateNodePositions(node->middle, level + 1, node->position - gap/2, node->position + gap/2);
    if (node->right)
        calculateNodePositions(node->right, level + 1, node->position + gap, right);
}

void mapDetailedTreeToArray(struct TreeNode* node, char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH]) {
    if (!node) return;
    
    const char* label = getDetailedNodeLabel(node);
    int len = strlen(label);
    int start = node->position - len/2;
    int y_base = node->level * 3;  // 레벨 간 간격 증가
    
    // 노드 레이블 출력
    for (int i = 0; i < len && start + i < MAX_TREE_WIDTH; i++) {
        if (start + i >= 0 && label[i] != '\n') {
            treeMap[y_base][start + i] = label[i];
        }
    }
    
    // 연결선 그리기 - 사선(/, \) 사용
    if (node->left) {
        int x1 = node->position;
        int x2 = node->left->position;
        for (int i = 1; i < 3; i++) {
            int x = x1 - i;
            int y = y_base + i;
            if (x >= 0 && x < MAX_TREE_WIDTH && y < MAX_TREE_HEIGHT) {
                treeMap[y][x] = '/';
            }
        }
        mapDetailedTreeToArray(node->left, treeMap);
    }
    
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

void visualizeDetailedTree(struct TreeNode* root) {
    if (!root || !stack.showVisualization) return;
    
    char treeMap[MAX_TREE_HEIGHT][MAX_TREE_WIDTH];
    for (int i = 0; i < MAX_TREE_HEIGHT; i++) {
        for (int j = 0; j < MAX_TREE_WIDTH; j++) {
            treeMap[i][j] = ' ';
        }
    }
    
    calculateNodePositions(root, 0, 0, MAX_TREE_WIDTH-1);
    mapDetailedTreeToArray(root, treeMap);
    
    printf("\n----- 파스 트리 -----\n");
    int hasContent;
    int printed = 0;
    for (int i = 0; i < MAX_TREE_HEIGHT; i++) {
        hasContent = 0;
        for (int j = 0; j < MAX_TREE_WIDTH; j++) {
            if (treeMap[i][j] != ' ') hasContent = 1;
        }
        if (!hasContent) continue;
        
        // 각 줄의 앞뒤 공백 제거
        int start = 0, end = MAX_TREE_WIDTH - 1;
        while (start < MAX_TREE_WIDTH && treeMap[i][start] == ' ') start++;
        while (end >= 0 && treeMap[i][end] == ' ') end--;
        
        if (start <= end) {
            printf("    ");  // 들여쓰기 추가
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
void freeTree(struct TreeNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->middle);
    freeTree(node->right);
    free(node);
}

// **토큰 처리 함수**
void getNextToken(void) {
    // 공백 문자 건너뛰기
    while (*state.input && isspace((unsigned char)*state.input)) {
        state.input++;
        state.position++;
    }
    
    // 입력 종료 확인
    if (*state.input == '\0') {
        state.currentToken.type = END;
        state.currentToken.position = state.position;
        return;
    }
    
    // 숫자 처리
    if (isdigit((unsigned char)*state.input)) {
        state.currentToken.type = NUMBER;
        state.currentToken.position = state.position;
        
        char numStr[MAX_NUMBER_LENGTH] = {0};
        int idx = 0;
        
        // 숫자 수집
        while (*state.input && (isdigit((unsigned char)*state.input) || 
               *state.input == '.') && idx < MAX_NUMBER_LENGTH - 1) {
            numStr[idx++] = *state.input++;
            state.position++;
        }
        numStr[idx] = '\0';
        
        // 숫자 변환
        state.currentToken.value = strtod(numStr, NULL);
        strncpy(state.currentToken.text, numStr, MAX_NUMBER_LENGTH - 1);
        return;
    }
    
    // 식별자 및 함수 처리
    if (isalpha((unsigned char)*state.input)) {
        char name[MAX_NUMBER_LENGTH] = {0};
        int idx = 0;
        
        while (*state.input && (isalnum((unsigned char)*state.input) || 
               *state.input == '_') && idx < MAX_NUMBER_LENGTH - 1) {
            name[idx++] = *state.input++;
            state.position++;
        }
        name[idx] = '\0';
        
        // 함수 식별
        if (strcmp(name, "sin") == 0) state.currentToken.type = SIN;
        else if (strcmp(name, "cos") == 0) state.currentToken.type = COS;
        else if (strcmp(name, "tan") == 0) state.currentToken.type = TAN;
        else if (strcmp(name, "sqrt") == 0) state.currentToken.type = SQRT;
        else if (strcmp(name, "log") == 0) state.currentToken.type = LOG;
        else if (strcmp(name, "log10") == 0) state.currentToken.type = LOG10;
        else {
            state.currentToken.type = ID;
            strncpy(state.currentToken.text, name, MAX_NUMBER_LENGTH - 1);
        }
        
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
double processMathFunction(enum TokenType funcType) {
    int functionPosition = state.currentToken.position;
    
    if (state.currentToken.type != LPAREN) {
        reportError("함수 호출에 괄호가 필요합니다", state.position);
    }
    
    getNextToken();
    double arg = expr();
    
    if (state.currentToken.type != RPAREN) {
        reportError("닫는 괄호가 필요합니다", state.position);
    }
    
    getNextToken();
    
    double result;
    switch (funcType) {
        case SIN: result = sin(arg); break;
        case COS: result = cos(arg); break;
        case TAN: result = tan(arg); break;
        case SQRT:
            if (arg < 0) {
                reportError("음수의 제곱근을 계산할 수 없습니다", functionPosition);
            }
            result = sqrt(arg);
            break;
        case LOG:
            if (arg <= 0) {
                reportError("0 이하의 로그를 계산할 수 없습니다", functionPosition);
            }
            result = log(arg);
            break;
        case LOG10:
            if (arg <= 0) {
                reportError("0 이하의 로그를 계산할 수 없습니다", functionPosition);
            }
            result = log10(arg);
            break;
        default:
            reportError("지원하지 않는 함수입니다", functionPosition);
            return 0;
    }
    
    validateNumber(result);
    return result;
}

// **식(expr) 처리 함수**
double expr(void) {
    if (++state.recursionDepth > MAX_RECURSION_DEPTH) {
        reportError("수식이 너무 복잡합니다", state.position);
    }

    struct TreeNode* exprNode = createDetailedNode("", NT_EXPR);
    double result = term();
    
    char valueStr[32];
    snprintf(valueStr, sizeof(valueStr), "%.2f", result);
    exprNode->left = createDetailedNode(valueStr, NT_TERM);
    stackPush(valueStr, NUMBER, result, 0);

    while (state.currentToken.type == PLUS || state.currentToken.type == MINUS) {
        enum TokenType op = state.currentToken.type;
        char opStr[2] = {op == PLUS ? '+' : '-', '\0'};
        
        struct TreeNode* newExpr = createDetailedNode("", NT_EXPR);
        struct TreeNode* opNode = createDetailedNode(opStr, NT_OPERATOR);
        
        newExpr->left = exprNode;
        newExpr->middle = opNode;
        
        stackPush(opStr, op, 0.0, 1);
        getNextToken();
        
        double rightVal = term();
        snprintf(valueStr, sizeof(valueStr), "%.2f", rightVal);
        newExpr->right = createDetailedNode(valueStr, NT_TERM);
        stackPush(valueStr, NUMBER, rightVal, 0);
        
        stackPop(); // rightVal
        stackPop(); // operator
        stackPop(); // result
        
        result = (op == PLUS) ? result + rightVal : result - rightVal;
        snprintf(valueStr, sizeof(valueStr), "%.2f", result);
        stackPush(valueStr, NUMBER, result, 0);
        
        exprNode = newExpr;
    }

    if (stack.showVisualization) {
        currentTree = exprNode;
        visualizeDetailedTree(currentTree);
    }

    state.recursionDepth--;
    return result;
}

// **항(term) 처리 함수**
double term(void) {
    double result = factor();
    struct TreeNode* termNode = createDetailedNode("", NT_TERM);
    
    char valueStr[32];
    snprintf(valueStr, sizeof(valueStr), "%.2f", result);
    termNode->left = createDetailedNode(valueStr, NT_FACTOR);

    while (state.currentToken.type == MULTIPLY || state.currentToken.type == DIVIDE) {
        enum TokenType op = state.currentToken.type;
        char opStr[2] = {op == MULTIPLY ? '*' : '/', '\0'};
        
        struct TreeNode* newTerm = createDetailedNode("", NT_TERM);
        struct TreeNode* opNode = createDetailedNode(opStr, NT_OPERATOR);
        
        newTerm->left = termNode;
        newTerm->middle = opNode;
        
        stackPush(opStr, op, 0.0, 1);
        getNextToken();
        
        double rightVal = factor();
        if (op == DIVIDE && rightVal == 0) {
            reportError("0으로 나눌 수 없습니다", state.position - 1);
        }
        
        snprintf(valueStr, sizeof(valueStr), "%.2f", rightVal);
        newTerm->right = createDetailedNode(valueStr, NT_FACTOR);
        stackPush(valueStr, NUMBER, rightVal, 0);
        
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
double factor(void) {
    if (state.currentToken.type == NUMBER) {
        double value = state.currentToken.value;
        struct TreeNode* node = createDetailedNode(state.currentToken.text, NT_NUMBER);
        getNextToken();
        return value;
    }

    if (state.currentToken.type == LPAREN) {
        getNextToken();
        struct TreeNode* node = createDetailedNode("()", NT_FACTOR);
        double result = expr();
        
        if (state.currentToken.type != RPAREN) {
            reportError("닫는 괄호가 필요합니다", state.position);
        }
        
        getNextToken();
        return result;
    }

    if (state.currentToken.type == MINUS) {
        getNextToken();
        return -factor();
    }

    if (state.currentToken.type >= SIN && state.currentToken.type <= LOG10) {
        enum TokenType funcType = state.currentToken.type;
        struct TreeNode* node = createDetailedNode(state.currentToken.text, NT_FUNCTION);
        getNextToken();
        return processMathFunction(funcType);
    }

    reportError("예상치 못한 토큰입니다", state.position);
    return 0;
}

// **메인 함수**
int main(void) {
    char buffer[MAX_INPUT_LENGTH];
    printf("수식 계산기 v3.0.0 - 파스 트리 시각화 포함\n");
    printf("명령어:\n");
    printf("  help     - 도움말 보기\n");
    printf("  show     - 파싱 과정 시각화 켜기\n");
    printf("  hide     - 파싱 과정 시각화 끄기\n");
    printf("  grammar  - 문법 규칙 보기\n");
    printf("  quit     - 프로그램 종료\n\n");

    while (1) {
        printf("\n> ");
        fflush(stdout);

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "입력 오류\n");
            break;
        }

        // 개행 문자 제거
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
