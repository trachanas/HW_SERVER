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
    char *a = "aaaa";
    printf("%d\n", atoi(a));
    if(isnumber(atoi(a)) == 0){
        printf("Is number\n");
    }
}