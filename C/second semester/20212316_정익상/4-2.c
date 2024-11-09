#include <stdio.h>
#include <windows.h>
#include <process.h>

void print_num(int a){
    while (1){
        printf("%d\n", a);
        Sleep(1000);
    }
}

int main (void){
    _beginthreadex(NULL, 0, (_beginthreadex_proc_type)print_num, (void *)256, 0, NULL);
    _beginthreadex(NULL, 0, (_beginthreadex_proc_type)print_num, (void *)128, 0, NULL);
    Sleep(5000);   
}