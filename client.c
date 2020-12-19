#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <dirent.h>
#include <ifaddrs.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"



// IP FROM HERE
// https://stackoverflow.com/questions/212528/get-the-ip-address-of-the-machine
// Gets the IP of the machine
char *getMyIP() {

    struct ifaddrs * ifAddrStruct = NULL;
    struct ifaddrs * ifa = NULL;
    void * tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {

        // skip lo and try to find wlsp wifi

        if (!ifa->ifa_addr || !strcmp(ifa->ifa_name, "lo")) continue;

        if (ifa->ifa_addr->sa_family == AF_INET) {

            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;

            char addressBuffer[INET_ADDRSTRLEN];

            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);

            return strdup(addressBuffer);
        }
    }
    printf("IP address not found!\n");
    return NULL;
}





int main(int argc, char **argv){

    if (argc != 7){
        printf("Wrong arguments!");
        return EXIT_FAILURE;
    }

    char *serverIP = NULL;
    int serverPortNum = -1;
    int clientPortNum = -1;
    char readBuffer[1000];

    char writeBuffer[1000];

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

    int mySocket, fd;

    mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == -1){
        printf("Socket creation failed!\nPlease exit!\n");
        return EXIT_FAILURE;
    }


    int option = 1;

    // kill "Address already in use" error message || Reuse the same port
    if (setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int)) == -1) {
        perror("setsockopt");
        return -1;
    }

    struct sockaddr_in server,client;
    struct sockaddr *serverPtr = (struct sockaddr *)&server;
    struct sockaddr *clientPtr = (struct sockaddr *)&client;

    // ip  and portnumber for the server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(serverIP);
    server.sin_port = htons(serverPortNum);
    socklen_t socklen = sizeof(client);

    // connect the client socket to server socket
    if (connect(mySocket, serverPtr, sizeof(server)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }



    while(1){

        memset(writeBuffer, '\0', 1000);

        while (fgets(writeBuffer, 1000, stdin) != NULL){

            writeBuffer[strlen(writeBuffer) - 1] = '\0';

            if (strcmp(writeBuffer, "exit") == 0) {
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