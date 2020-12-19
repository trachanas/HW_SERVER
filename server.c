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


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"
#define SA struct sockaddr

int main(int argc, char **argv) {

    if (argc != 2){
        printf("Wrong number of arguments!\n");
        return EXIT_FAILURE;
    }


    return 0;
}
