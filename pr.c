#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

int main(){

    char buffer[100];
    char *token = NULL;
    memset(buffer, '\0', 100);

    int a = 0;
    int b = 0;
    printf("%d\n", a);
    while(fgets(buffer, 100, stdin) != NULL){

        buffer[strlen(buffer) - 1] = '\0';


        token = strtok(buffer,"+-*/");
        a = atoi(token);
        token = strtok(NULL, "+-*/");
        b = atoi(token);



        printf("a = %d \t b = %d\n", a, b);
        if (strchr(buffer, '*') != NULL){
            printf("%d * %d = %d\n", a, b, a * b);
        }
        if (strchr(buffer, '+') != NULL){
            printf("%d + %d = %d\n", a, b, a + b);
        }
        if (strchr(buffer, '-') != NULL){
            printf("%d - %d = %d\n", a, b, a - b);
        }
        if (strchr(buffer, '/') !=NULL){
            printf("%d / %d = %d\n", a, b, a / b);
        }

    }

//    // * = 42
//    char c = '*';
//    printf("%c = %d\n", c,c);
//
//    // - = 45
//    c = '-';
//    printf("%c = %d\n", c,c);
//
//    // + = 43
//    c = '+';
//    printf("%c = %d\n", c, c);
//
//    // / = 47
//    c = '/';
//    printf("%c = %d\n", c, c);

}