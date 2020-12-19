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
#define PORT 8080
#define SA struct sockaddr


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"


char *clientIP;
int err;
int mySocket;




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


struct sockaddr_in server;
struct sockaddr *serverPtr = (struct sockaddr *) &server;
struct hostent *hostPtr;
struct in_addr addr;



int main(int argc, char **argv){

    if (argc != 4){
        printf("Wrong arguments!");
        return EXIT_FAILURE;
    }
    int portNum = 1000;
    char *dirName;
    int workerThreads;
    int bufferSize;
    int serverPort;
    char *serverIP = NULL;
    int portN;

    char writeBuffer[1000];

    char *pathname = NULL;
    char *message = malloc(100 * sizeof(char));
    char *token = NULL;
    char *rest;
    char *rMessage;
    char *userOnIP;

    int nClientPort = -1;
    int bytes;
    int numOfClients;
    int userOnPort;

    struct in_addr clientAddr;

    serverPort = atoi(argv[1]);
    portN = (int) atoi(argv[2]);
    serverIP = strdup(argv[3]);

    printf("Client is listening in %d\n", portN);

// Create socket
    mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if (mySocket == -1) {
        perror("<Creating socket failed!>");
        exit(-1);
    }

    inet_aton(serverIP, &addr);

    if ((hostPtr = gethostbyaddr(&addr, sizeof(addr), AF_INET)) == NULL) {
        herror("<GetHostByAddr failed!>\n");
        exit(-1);
    }

    server.sin_family = AF_INET;

    memcpy(&server.sin_addr, hostPtr->h_addr, hostPtr->h_length);

    server.sin_port = htons(serverPort);
    if (connect(mySocket, serverPtr, sizeof(server)) < 0) {
        perror("<Connect failed!>");
        exit(-1);
    }

    clientIP = getMyIP();

    nClientPort = htons(portNum);

    inet_aton(clientIP, &clientAddr);

    int client_net_address = clientAddr.s_addr;

    while (1) {


        memset(writeBuffer, '\0', 1000);

        while (fgets(writeBuffer, 1000, stdin) != NULL){

            writeBuffer[strlen(writeBuffer) -1] = '\0';

            printf(GRN "SENT FROM CLIENT: %s\n" RESET, writeBuffer);

            if (strcmp(writeBuffer, "exit") == 0) {
                printf("Client left!\n");
                break;
            }

            //stelnw thn IP kai to portNumber tou client
            strcpy(writeBuffer, clientIP);

            write(mySocket, writeBuffer, strlen(writeBuffer));

            memset(writeBuffer, '\0', 1000);

            //portnum
            strcpy(writeBuffer,argv[2]);

            write(mySocket, writeBuffer, 1000);

            memset(writeBuffer, '\0', 1000);
            printf("FD IS %d\n", mySocket);



            //perimenw na diabasw minima apodoxis
            bytes = read(mySocket, writeBuffer, 1000);

            printf(RED "MESSAGE RECEIVED: %s\n" RESET, writeBuffer);

            memset(writeBuffer, '\0', 1000);

        }
        break;

        //read(mySocket, writeBuffer, 1000);

       // printf("MESSAGE RECEIVED: %s\n", writeBuffer);
    }

    memset(writeBuffer, '\0', 1000);

    printf("%d\n", argc);

}