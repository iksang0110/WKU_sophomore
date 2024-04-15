/* Report 02
     * 중위 표기식을 exprss에 입력하여 후위 표기식으로 변환하고 연산할 수 있게
     * 프로그램을 작성하시오.
     * 중위 표기식 => 후위 표기식으로 변환할 수 있는 함수 필요!!!
     */
// 클린코드 작성을 위해 용어 정리, 전위/중위/후위 표기법(prefix/infix/postfix)
// 참고 자료 https://deepdata.tistory.com/372 (https://woongsios.tistory.com/288,https://velog.io/@mendel/%EC%A4%91%EC%9C%84-%ED%91%9C%EA%B8%B0%EB%B2%95%EA%B3%BC-%ED%9B%84%EC%9C%84%ED%91%9C%EA%B8%B0%EB%B2%95-%EC%A0%95%EB%A6%AC-%EB%B0%B1%EC%A4%80-1918-java)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char element; // 스택 원소(element)의 자료형을 char로 정의

typedef struct stackNode { // 스택의 노드를 구조체로 정의
    element data;
    struct stackNode* link;
} stackNode;

stackNode* top; // 스택의 top 노드를 지정하기 위한 포인터 top 선언

int isStackEmpty() {
    if (top == NULL) return 1;
    else return 0;
}

void push(element item) {
    stackNode* temp = (stackNode*)malloc(sizeof(stackNode));
    temp->data = item;
    temp->link = top;
    top = temp;
}

element pop() {
    element item;
    stackNode* temp = top;
    if (isStackEmpty()) {
        printf("\n\n 스텍이 비어있습니다.\n");
        return 0;
    } 
    else {
        item = temp->data;
        top = temp->link;
        free(temp);
        return item;
    }
}

int precedence(char operator) {
    switch (operator) {
        case '(': return 0;
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        default: return -1;
    }
}
//중위 -> 후위 변환 함수 
void infixChangePostfix(char* infix, char* postfix) {
    int i = 0, j = 0;
    char item, x;
    push('(');               // '('를 스택에 푸시하고 식의 끝에 ')'를 추가
    strcat(infix, ")");
    
    item = infix[i];
    
    while (!isStackEmpty()) {
        if (item == '(') {
            push(item);
        } 
        else if (item >= '0' && item <= '9') { // 숫자인 경우
            postfix[j++] = item;
        } 
        else if (item == '+' || item == '-' || item == '*' || item == '/') { // 연산자인 경우
            x = pop();
            while (precedence(x) >= precedence(item)) {
                postfix[j++] = x;
                x = pop();
            }
            push(x);
            push(item);
        } 
        else if (item == ')') { // ')'를 만났을 때
            x = pop();
            while (x != '(') {
                postfix[j++] = x;
                x = pop();
            }
        } 
        else { // 잘못된 문자를 만났을 때
            printf("식에 잘못된 문자가 있습니다.\n");
            exit(1);
        }
        item = infix[++i];
    }
    postfix[j] = '\0'; // 문자열 끝을 나타내는 '\0' 추가
}

element evalPostfix(char* exp) { 
    int opr1, opr2, value, i = 0;
    int length = strlen(exp);
    char symbol;
    top = NULL;

    for (i = 0; i < length; i++) {
        symbol = exp[i];
        if (symbol != '+' && symbol != '-' && symbol != '*' && symbol != '/') {
            value = symbol - '0';
            push(value);
        } 
        else {
            opr2 = pop();
            opr1 = pop();
            switch (symbol) {
                case '+': push(opr1 + opr2); break;
                case '-': push(opr1 - opr2); break;
                case '*': push(opr1 * opr2); break;
                case '/': push(opr1 / opr2); break;
            }
        }
    }
    return pop();
}

int main(void) {
    char infix[100] = "3+5*6-2/1"; // 입력 중위 표기식 (test중위 식들 : 7+3*(5-2)+4, 3+2+4+*5+3/1, ((3+5)*(7-3)) )
    char postfix[100] = "";        // 후위 표기식으로 변환될 배열

    printf("중위 표기식: %s\n", infix);
    infixChangePostfix(infix, postfix);
    printf("후위 표기식: %s\n", postfix);

    int result = evalPostfix(postfix);
    printf("연산 결과 => %d\n", result);

    getchar();
    return 0;
}
