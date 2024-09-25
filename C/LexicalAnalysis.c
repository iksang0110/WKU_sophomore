#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

// 토큰 구조체
struct Token {
    enum TokenType type;
    double value;
};

// 전역 변수
char *input;
struct Token currentToken;

// 함수 선언
void getNextToken();
double expr();
double term();
double factor();

// 다음 토큰 가져오기
void getNextToken() {
    while (*input && isspace(*input)) input++;

    if (*input == '\0') {
        currentToken.type = END;
        return;
    }

    if (isdigit(*input) || *input == '.') {
        char *endptr;
        currentToken.type = NUMBER;
        currentToken.value = strtod(input, &endptr);
        input = endptr;
        return;
    }

    switch (*input) {
        case '+': currentToken.type = PLUS; break;
        case '-': currentToken.type = MINUS; break;
        case '*': currentToken.type = MULTIPLY; break;
        case '/': currentToken.type = DIVIDE; break;
        case '(': currentToken.type = LPAREN; break;
        case ')': currentToken.type = RPAREN; break;
        default:
            fprintf(stderr, "잘못된 문자에요: %c\n", *input);
            exit(1);
    }
    input++;
}

// 수식 파싱
double expr() {
    double result = term();
    while (currentToken.type == PLUS || currentToken.type == MINUS) {
        if (currentToken.type == PLUS) {
            getNextToken();
            result += term();
        } else {
            getNextToken();
            result -= term();
        }
    }
    return result;
}

// 항 파싱
double term() {
    double result = factor();
    while (currentToken.type == MULTIPLY || currentToken.type == DIVIDE) {
        if (currentToken.type == MULTIPLY) {
            getNextToken();
            result *= factor();
        } else {
            getNextToken();
            double divisor = factor();
            if (divisor == 0) {
                fprintf(stderr, "0으로 나누기\n");
                exit(1);
            }
            result /= divisor;
        }
    }
    return result;
}

// 인수 파싱
double factor() {
    if (currentToken.type == NUMBER) {
        double value = currentToken.value;
        getNextToken();
        return value;
    } else if (currentToken.type == LPAREN) {
        getNextToken();
        double result = expr();
        if (currentToken.type != RPAREN) {
            fprintf(stderr, "예상하는 닫는 괄호\n");
            exit(1);
        }
        getNextToken();
        return result;
    }
    fprintf(stderr, "예상치 못한 토큰\n");
    exit(1);
}

int main() {
    char buffer[1000];
    printf("표현식 입력: ");
    fgets(buffer, sizeof(buffer), stdin);
    
    input = buffer;
    getNextToken();
    double result = expr();
    
    if (currentToken.type != END) {
        fprintf(stderr, "입력 마지막에 예상치 못한 토큰이 있어요\n");
        return 1;
    }
    
    printf("결과: %.6f\n", result);
    return 0;
}