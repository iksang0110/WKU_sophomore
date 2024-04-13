#define _CRT_SECURE_NO_WARNING
#include<stdio.h>
int main(void) {
   int KILL;
   printf("몇킬을 했습니까");
   scanf("%d", &KILL);

   if (KILL == 2){
      printf("더블킬");
   }
   else if (KILL == 3) {
      printf("트리플킬");
   }
   else if (KILL == 4) {
      printf("쿼드라킬");
   }
   else if (KILL == 5) {
      printf("펜타킬");
   }
   else
    printf("마무리");

   return 0;
}