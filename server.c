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

    int portN = atoi(argv[1]);

    printf("%d\n", portN);

    char *token = NULL;
    char *clientIP = NULL;
    int clientPortNum = 0;
    int flag = 0;
    int bytes = -1;
    int numOfClients = 0;
    int fd ;


    int mySocket;
//strings for messages process
    char *tuple = NULL;
    char *getClientsMessage = NULL;
    char *message = NULL;
    char readBuffer[1000];
    char writeBuffer[1000];
    char *userOnMessage = malloc(50 * sizeof(char));
    char delim[4] = " ,";
    //Structs for socket communication
    struct sockaddr_in server,client;
    struct sockaddr *serverPtr = (struct sockaddr *)&server;
    struct sockaddr *clientPtr = (struct sockaddr *)&client;
    struct in_addr clientAddr;
    socklen_t socklen = sizeof(client);

    mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if (mySocket == -1){
        printf("Socket creation failed!\n");
        return EXIT_FAILURE;
    }

    // kill "Address already in use" error message || Reuse the same port
    if (setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR,&optind ,sizeof(int)) == -1) {
        perror("setsockopt");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(portN);

    //Binding
    if (bind(mySocket,serverPtr, sizeof(server)) < 0) {
        perror("<Socket binding failed!>");
        return -1;
    }

    //Listening
    if (listen(mySocket,3) < 0) {
        perror("<Socket listening failed!>");
        return -1;
    }

    printf("Server is listening in port %d!\n", portN);

    int ch = -1;

    while (1){

        fd = accept(mySocket, clientPtr, &socklen);

        if (fd < 0) {
            printf("Accept new connection failed!\n");
            return EXIT_FAILURE;
        }

        while ((bytes = read(fd, readBuffer, 1000)) > 0){
            ch = 1;

            printf(RED "MESSAGE RECEIVED: %s\n" RESET, readBuffer);

            for (int i = 0; i < strlen(readBuffer); i++){
                if (!isdigit(readBuffer[i])){
                    ch = 0;
                    break;
                }
            }

            if (ch){
                //an einai arithmos diladi port num
                printf(RED "Clients port number %d\n" RESET, atoi(readBuffer));

                clientPortNum = atoi(readBuffer);

                message = strdup("malaka eftase to minima");
            }
            else{
                //an einai minima
                printf(RED "SENT FROM CLIENT: %s\n" RESET, readBuffer);

                clientIP = strdup(readBuffer);
            }

            printf(GRN "CLIENTS IP: %s\t" RESET, clientIP);
            printf(GRN "PORT NUM: %d\n" RESET, clientPortNum);

            int sockfd;
            struct sockaddr_in servaddr, cli;

            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd == -1) {
                printf("socket creation failed...\n");
                exit(0);
            }
            else
                printf("Socket successfully created..\n");


            bzero(&servaddr, sizeof(servaddr));

            // assign IP, PORT
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
            servaddr.sin_port = htons(clientPortNum);
            printf("FD IS %d\n", mySocket);


            int  connfd, len;

            // socket create and verification
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd == -1) {
                printf("socket creation failed...\n");
                exit(0);
            }
            else
                printf("Socket successfully created..\n");
            bzero(&servaddr, sizeof(servaddr));

            // assign IP, PORT
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
            servaddr.sin_port = htons(4040);

            // Binding newly created socket to given IP and verification
            if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
                printf("socket bind failed...\n");
                exit(0);
            }
            else
                printf("Socket successfully binded..\n");

            // Now server is ready to listen and verification
            if ((listen(sockfd, 5)) != 0) {
                printf("Listen failed...\n");
                exit(0);
            }
            else
                printf("Server listening..\n");
            len = sizeof(cli);

            // Accept the data packet from client and verification
            connfd = accept(sockfd, (SA*)&cli, &len);
            if (connfd < 0) {
                printf("server acccept failed...\n");
                exit(0);
            }
            else
                printf("server acccept the client...\n");

            printf("fd is %d\n", mySocket);

            //stpcpy(writeBuffer,message);
            bytes = write(sockfd, "this", strlen("this"));

            printf("%d\n", bytes);

            memset(writeBuffer, '\0', 1000);

            memset(readBuffer, '\0', 1000);

        }

    }

    return 0;
}
