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

// **11월 19일**: 지수 연산과 루트 함수 추가
// - pow() 함수를 이용한 지수 연산 구현.
// - sqrt() 함수 추가로 제곱근 계산 가능.
// - 연산자 우선순위 재조정 필요 -> 지수를 가장 높게 설정.
// - 고민: 단항 연산자 처리 방식 (예: -2^2 vs (-2)^2).

// **11월 23일**: 메모리 관리 개선
// - 재귀 호출 깊이 제한 추가로 스택 오버플로우 방지.
// - 동적 메모리 할당 대신 고정 크기 버퍼 사용 -> 메모리 누수 위험 감소.
// - 버퍼 오버플로우 취약점 해결을 위한 입력 크기 검증 추가.

// **11월 28일**: 에러 처리 강화
// - NaN과 Infinity 처리 로직 추가.
// - 순환 참조 감지 (예: 재귀적 함수 호출).
// - 자세한 에러 메시지와 위치 표시 기능.

// **초기 구현 (10월 14일)**
// - 단순한 토큰 구조
/*
enum TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    END
};

struct Token {
    enum TokenType type;
    int value;  // 정수만 저장
};

// 전역 변수 사용
char *input;
struct Token currentToken;
*/

// **개선된 구현 (11월 28일)**
// **상수 정의**
#define MAX_RECURSION_DEPTH 100    // 재귀 깊이 제한
#define MAX_INPUT_LENGTH 1000      // 입력 버퍼 크기
#define MAX_ERROR_MSG_LENGTH 100   // 에러 메시지 버퍼 크기
#define MAX_NUMBER_LENGTH 32       // 숫자 문자열 최대 길이

// **토큰 타입 정의**
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
    ABS,        // abs
    END         // 입력 종료
};

// **토큰 구조체**
struct Token {
    enum TokenType type;    // 토큰 유형
    double value;          // 숫자일 경우의 값
    int position;          // 토큰 위치 (에러 표시용)
    char text[MAX_NUMBER_LENGTH];  // 원본 텍스트 (디버깅용)
};

// **초기 파서 상태 관리 (10월 14일)**
/*
// 전역 변수로 관리
char *input;
struct Token currentToken;
*/

// **개선된 파서 상태 구조체**
struct ParserState {
    char *input;           // 입력 문자열 포인터
    char *original_input;  // 에러 표시를 위한 원본 입력 보관
    int position;          // 현재 처리 위치
    int recursionDepth;    // 현재 재귀 깊이
    struct Token currentToken;  // 현재 처리 중인 토큰
    char errorMsg[MAX_ERROR_MSG_LENGTH];  // 에러 메시지 버퍼
    int errorPosition;     // 에러 발생 위치
    int hadError;         // 에러 발생 여부 플래그
} state;

// 함수 선언
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
    for (int i = 0; i < position; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "^\n");
    
    // 에러 상태 설정
    state.hadError = 1;
    state.errorPosition = position;
    strncpy(state.errorMsg, message, MAX_ERROR_MSG_LENGTH - 1);
    state.errorMsg[MAX_ERROR_MSG_LENGTH - 1] = '\0';
    
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
    state.original_input = strdup(input);  // 원본 입력 보관
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
    free(state.original_input);
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

// **함수 식별**
enum TokenType identifyFunction(const char *str) {
    struct {
        const char *name;
        enum TokenType type;
    } functions[] = {
        {"sin", SIN},
        {"cos", COS},
        {"tan", TAN},
        {"sqrt", SQRT},
        {"log", LOG},
        {"log10", LOG10},
        {"abs", ABS},
        {NULL, END}
    };
    
    for (int i = 0; functions[i].name != NULL; i++) {
        if (strcmp(str, functions[i].name) == 0) {
            return functions[i].type;
        }
    }
    
    return END;
}

// **초기 숫자 파싱 (10월 14일)**
/*
double parseNumber() {
    int value = 0;
    while (isDigit(*input)) {
        value = value * 10 + (*input - '0');
        input++;
    }
    return value;
}
*/

// **개선된 숫자 파싱**
double parseNumber(void) {
    char numberStr[MAX_NUMBER_LENGTH] = {0};
    int idx = 0;
    int hasDot = 0;
    int hasE = 0;
    
    // 부호 처리
    if (*state.input == '-' || *state.input == '+') {
        numberStr[idx++] = *state.input++;
        state.position++;
    }
    
    // 숫자와 소수점, 지수 처리
    while (*state.input && idx < MAX_NUMBER_LENGTH - 1) {
        char c = *state.input;
        
        if (isdigit((unsigned char)c)) {
            numberStr[idx++] = c;
        }
        else if (c == '.' && !hasDot && !hasE) {
            numberStr[idx++] = c;
            hasDot = 1;
        }
        else if ((c == 'e' || c == 'E') && !hasE) {
            numberStr[idx++] = c;
            hasE = 1;
            // 지수 부호 처리
            if (state.input[1] == '+' || state.input[1] == '-') {
                state.input++;
                state.position++;
                numberStr[idx++] = *state.input;
            }
        }
        else {
            break;
        }
        
        state.input++;
        state.position++;
    }
    
    numberStr[idx] = '\0';
    
    // 문자열을 숫자로 변환
    char *endptr;
    double value = strtod(numberStr, &endptr);
    
    // 변환 실패 검사
    if (endptr == numberStr || *endptr != '\0') {
        reportError("잘못된 숫자 형식", state.position - strlen(numberStr));
    }
    
    // 값의 유효성 검사
    validateNumber(value);
    
    return value;
}
// **초기 토큰 처리 구현 (10월 14일)**
/*
void getNextToken() {
    skipWhitespace();
    
    if (*input == '\0') {
        currentToken.type = END;
        return;
    }
    
    if (isDigit(*input)) {
        currentToken.type = NUMBER;
        currentToken.value = parseNumber();
        return;
    }
    
    switch (*input) {
        case '+': currentToken.type = PLUS; break;
        case '-': currentToken.type = MINUS; break;
        case '*': currentToken.type = MULTIPLY; break;
        case '/': currentToken.type = DIVIDE; break;
        default:
            reportError("잘못된 문자");
    }
    input++;
}
*/

// **개선된 토큰 처리 구현**
void getNextToken(void) {
    // 공백 건너뛰기
    skipWhitespace();
    
    // 입력 종료 확인
    if (*state.input == '\0') {
        state.currentToken.type = END;
        state.currentToken.position = state.position;
        return;
    }
    
    // 숫자 처리
    if (isValidNumberStart(*state.input)) {
        state.currentToken.type = NUMBER;
        state.currentToken.position = state.position;
        state.currentToken.value = parseNumber();
        
        // 숫자 원본 텍스트 저장
        strncpy(state.currentToken.text, 
                state.input - strlen(state.currentToken.text),
                MAX_NUMBER_LENGTH - 1);
        return;
    }
    
    // 함수나 상수 이름 처리
    if (isValidIdentifierStart(*state.input)) {
        char name[MAX_NUMBER_LENGTH] = {0};
        int idx = 0;
        
        // 식별자 수집
        while (*state.input && isValidIdentifierPart(*state.input) && 
               idx < MAX_NUMBER_LENGTH - 1) {
            name[idx++] = *state.input++;
            state.position++;
        }
        name[idx] = '\0';
        
        // 함수 타입 식별
        enum TokenType funcType = identifyFunction(name);
        if (funcType != END) {
            state.currentToken.type = funcType;
            state.currentToken.position = state.position - strlen(name);
            return;
        }
        
        // 인식할 수 없는 식별자
        reportError("알 수 없는 함수 또는 상수 이름", 
                   state.position - strlen(name));
    }
    
    // 연산자와 괄호 처리
    state.currentToken.position = state.position;
    switch (*state.input) {
        case '+': 
            state.currentToken.type = PLUS; 
            break;
        case '-': 
            state.currentToken.type = MINUS; 
            break;
        case '*': 
            state.currentToken.type = MULTIPLY; 
            break;
        case '/': 
            state.currentToken.type = DIVIDE; 
            break;
        case '^': 
            state.currentToken.type = POWER; 
            break;
        case '(': 
            state.currentToken.type = LPAREN; 
            break;
        case ')': 
            state.currentToken.type = RPAREN; 
            break;
        default:
            reportError("인식할 수 없는 문자", state.position);
    }
    
    state.input++;
    state.position++;
}

// **초기 수학 함수 처리 (10월 16일)**
/*
double processMathFunction(enum TokenType type) {
    if (currentToken.type != LPAREN) {
        reportError("함수 호출에 괄호 필요");
    }
    getNextToken();
    double arg = expr();
    if (currentToken.type != RPAREN) {
        reportError("닫는 괄호 필요");
    }
    getNextToken();
    
    switch (type) {
        case SIN: return sin(arg);
        case COS: return cos(arg);
        case TAN: return tan(arg);
        default: reportError("지원하지 않는 함수");
    }
    return 0;
}
*/

// **개선된 수학 함수 처리**
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
        case ABS:
            result = fabs(arg);
            break;
        default:
            reportError("지원하지 않는 함수입니다", functionPosition);
            return 0;  // 도달하지 않음
    }
    
    validateNumber(result);
    return result;
}
// **초기 지수 처리 구현 (11월 19일)**
/*
double power() {
    double base = factor();
    if (currentToken.type == POWER) {
        getNextToken();
        double exponent = factor();
        return pow(base, exponent);
    }
    return base;
}
*/

// **개선된 지수 처리 구현**
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

// **초기 항(term) 처리 구현 (10월 14일)**
/*
double term() {
    double result = factor();
    while (currentToken.type == MULTIPLY || currentToken.type == DIVIDE) {
        enum TokenType op = currentToken.type;
        getNextToken();
        if (op == MULTIPLY)
            result *= factor();
        else
            result /= factor();
    }
    return result;
}
*/

// **개선된 항(term) 처리 구현**
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

// **초기 인수(factor) 처리 구현 (10월 14일)**
/*
double factor() {
    if (currentToken.type == NUMBER) {
        double value = currentToken.value;
        getNextToken();
        return value;
    } else if (currentToken.type == LPAREN) {
        getNextToken();
        double result = expr();
        if (currentToken.type != RPAREN) {
            reportError("닫는 괄호 필요");
        }
        getNextToken();
        return result;
    }
    reportError("예상치 못한 토큰");
    return 0;
}
*/

// **개선된 인수(factor) 처리 구현**
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
        state.currentToken.type == LOG10 ||
        state.currentToken.type == ABS) {
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

// **초기 수식(expression) 처리 구현 (10월 14일)**
/*
double expr() {
    double result = term();
    while (currentToken.type == PLUS || currentToken.type == MINUS) {
        enum TokenType op = currentToken.type;
        getNextToken();
        if (op == PLUS)
            result += term();
        else
            result -= term();
    }
    return result;
}
*/

// **개선된 수식(expression) 처리 구현**
double expr(void) {
    // 재귀 깊이 제한 검사
    if (++state.recursionDepth > MAX_RECURSION_DEPTH) {
        reportError("수식이 너무 복잡합니다 (중첩 깊이 초과)", 
                   state.position);
    }
    
    double result = term();
    
    while (state.currentToken.type == PLUS || 
           state.currentToken.type == MINUS) {
        enum TokenType op = state.currentToken.type;
        int operatorPosition = state.currentToken.position;
        
        getNextToken();
        double rightOperand = term();
        
        if (op == PLUS) {
            result += rightOperand;
        } else {  // MINUS
            result -= rightOperand;
        }
        
        validateNumber(result);
    }
    
    state.recursionDepth--;
    return result;
}

// **메인 함수 최종 구현**
int main(void) {
    char buffer[MAX_INPUT_LENGTH];
    printf("수식을 입력하세요 (끝내려면 빈 줄 입력): \n");
    
    while (1) {
        printf("> ");
        
        // 안전한 입력 처리
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "입력을 읽는 중 오류가 발생했습니다.\n");
            return 1;
        }
        
        // 빈 줄 체크 (종료 조건)
        if (buffer[0] == '\n') {
            break;
        }
        
        // 개행 문자 제거
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
            len--;
        }
        
        // 입력 길이 검증
        if (len >= MAX_INPUT_LENGTH - 1) {
            fprintf(stderr, "입력이 너무 깁니다 (최대 %d 문자)\n", 
                    MAX_INPUT_LENGTH - 1);
            continue;
        }
        
        // 파서 상태 초기화
        initializeState(buffer);
        
        // 계산 수행
        try {
            getNextToken();
            double result = expr();
            
            // 남은 입력 확인
            if (state.currentToken.type != END) {
                reportError("수식 끝에 예상치 못한 토큰이 있습니다", 
                           state.currentToken.position);
            }
            
            // 결과 출력
            printf("= %.10g\n", result);
        } catch (...) {
            fprintf(stderr, "예상치 못한 오류가 발생했습니다\n");
        }
        
        // 메모리 정리
        cleanup();
    }
    
    printf("프로그램을 종료합니다.\n");
    return 0;
}

/*
// **향후 개발 계획**

1. 기능 개선
   - 변수 지원 추가
     * 사용자 정의 변수
     * 상수 (pi, e 등)
   - 사용자 정의 함수 지원
   - 복소수 연산 지원
   - 각도/라디안 변환
   - 진법 변환 (2진수, 8진수, 16진수)

2. 성능 최적화
   - 토큰 캐싱 시스템 구현
   - 자주 사용되는 수식 결과 캐싱
   - 메모리 풀링으로 동적 할당 최소화
   - 재귀 제거를 통한 스택 사용 최적화

3. 사용자 경험 개선
   - 컬러 구문 강조
   - 자동 완성 기능
   - 계산 히스토리 관리
   - 실행 취소/다시 실행
   - 수식 저장 및 불러오기

4. 오류 처리 강화
   - 더 자세한 오류 메시지
   - 오류 복구 메커니즘
   - 문법 제안 기능
   - 디버깅 모드 추가

5. 확장성
   - 플러그인 시스템
   - 사용자 정의 연산자
   - 외부 라이브러리 연동
   - 스크립팅 지원

6. 테스트 강화
   - 단위 테스트 확대
   - 퍼지 테스팅 도입
   - 성능 벤치마크
   - 메모리 누수 검사

7. 문서화
   - API 문서 작성
   - 사용자 매뉴얼 작성
   - 예제 코드 추가
   - 변경 이력 관리
*/