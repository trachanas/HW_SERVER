#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <signal.h>
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"


char *mes = NULL;
char *resultString = NULL;

void signal_handler(){

    printf("\nServer is down!\nGoodbye!\n");

    free(resultString);

    free(mes);

    exit(1);

}


int main(int argc, char **argv) {

    int bytes = -1;
    char readBuffer[1000];
    char *token = NULL;
    int a , b, result;
    int serverPortNum = -1;
    int mySocket, fd;
    int option = 1;

    struct sockaddr_in server,client;
    struct sockaddr *serverPtr = (struct sockaddr *)&server;
    struct sockaddr *clientPtr = (struct sockaddr *)&client;


    if (argc != 3){
        printf("Wrong number of arguments!\n");
        return EXIT_FAILURE;
    }


    for (int i = 1; i < argc; i += 2){
        if (strcmp(argv[i], "-pn") == 0)   serverPortNum = atoi(argv[i+1]);

        else{
            printf("Wrong arguments!\n");
            return EXIT_FAILURE;
        }
    }

    printf("Server is listening to port: %d\n", serverPortNum);

    //socket creation
    mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == -1) {
        printf("Socket creation failed!\nPlease exit!\n");
        return EXIT_FAILURE;
    }


    // kill "Address already in use" error message || Reuse the same port
    if (setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int)) == -1) {
        printf("SetSockOpt failed!\nPlease exit!\n");
        return EXIT_FAILURE;
    }

    // ip  and portnumber for the server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(serverPortNum);
    socklen_t socklen = sizeof(client);


    //bind the socket
    if (bind(mySocket, serverPtr, sizeof (server)) != 0){
        printf("Socket binding failed!\nPlease exit!\n");
        return EXIT_FAILURE;
    }

    //listen
    if ((listen(mySocket, 5)) != 0) {
        printf("Socket listening failed!\nPlease exit!\n");
        return EXIT_FAILURE;
    }

    signal(SIGINT, signal_handler);

    while (1){

        fd = accept(mySocket, clientPtr, &socklen);
        if (fd < 0){
            printf("Socket acceptance failed!\nPlease exit!\n");
            return EXIT_FAILURE;
        }

        while ((bytes = read(fd, readBuffer, 1000)) > 0){

            printf(RED "MESSAGE RECEIVED FROM CLIENT: %s\n" RESET, readBuffer);

            mes = strdup(readBuffer);

            token = strtok(readBuffer,"+-*/");
            a = atoi(token);
            token = strtok(NULL, "+-*/");
            b = atoi(token);

            printf("a = %d \t b = %d\n", a, b);
            if (strchr(mes, '*') != NULL){
                result = a * b;
                //printf("%d * %d = %d\n", a, b, a * b);
            }
            if (strchr(mes, '+') != NULL){
                result = a + b;
                //printf("%d + %d = %d\n", a, b, a + b);
            }
            if (strchr(mes, '-') != NULL){
                result = a - b;

                //printf("%d - %d = %d\n", a, b, a - b);
            }
            if (strchr(mes, '/') !=NULL){
                result = a / b;
                //printf("%d / %d = %d\n", a, b, a / b);
            }

            resultString = malloc(100 * sizeof(char));

            sprintf(resultString, "%d", result);

            write(fd, resultString, 1000);

            memset(readBuffer, '\0', 1000);
        }

    }

    return 0;
}
