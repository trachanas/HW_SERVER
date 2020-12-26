#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"


char *serverIP = NULL;

void signal_handler() {

    printf("\nClient left with SIGINT!\nGoodbye!\n");

    free(serverIP);

    exit(1);
}


int main(int argc, char **argv){

    if (argc != 7){
        printf("Wrong arguments!");
        return EXIT_FAILURE;
    }

    int serverPortNum = -1;
    int clientPortNum = -1;
    int mySocket, fd;
    int option = 1;

    char readBuffer[1000];
    char writeBuffer[1000];

    struct sockaddr_in server,client;
    struct sockaddr *serverPtr = (struct sockaddr *)&server;
    struct sockaddr *clientPtr = (struct sockaddr *)&client;


    for (int i = 1; i < argc; i += 2){

        if (strcmp(argv[i],"-sip") == 0)  serverIP = strdup(argv[i+1]);

        else if (strcmp(argv[i], "-spn") == 0)  serverPortNum = atoi(argv[i+1]);

        else if (strcmp(argv[i], "-pn") == 0)   clientPortNum = atoi(argv[i+1]);

        else {
            printf("Wrong arguments!\n");
            return EXIT_FAILURE;
        }
    }

    printf("Client is listening to port: %d\n", clientPortNum);

    mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == -1){
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
    server.sin_addr.s_addr = inet_addr(serverIP);
    server.sin_port = htons(serverPortNum);
    socklen_t socklen = sizeof(client);

    // connect the client socket to server socket
    if (connect(mySocket, serverPtr, sizeof(server)) != 0) {
        printf("Connection with the server failed...\n");
        return EXIT_FAILURE;
    }


    signal(SIGINT, signal_handler);

    while(1){

        memset(writeBuffer, '\0', 1000);

        while (fgets(writeBuffer, 1000, stdin) != NULL){

            writeBuffer[strlen(writeBuffer) - 1] = '\0';

            if (strcmp(writeBuffer, "exit") == 0) {

                free(serverIP);

                printf("Client left!\n");

                break;
            }

            write(mySocket, writeBuffer, strlen(writeBuffer));

            memset(writeBuffer, '\0', 1000);

            read(mySocket, readBuffer, 1000);

            printf("%s\n", readBuffer);

            memset(readBuffer, '\0', 1000);
        }
        break;
    }
    return 0;
}