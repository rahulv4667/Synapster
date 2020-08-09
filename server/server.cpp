/**
 * Partly inspired from :
 * https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
 * 
 * */


#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <string.h>
// #include <stdint.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>
#include <set>
#include "utilities.h"

// int main(int argc, char *argv[]) {

//     if(argc < 3)
//         exitWithUserMessage("Parameter(s)", "<Timeout (sec.s)> <Port/Service 1> <Port/Service 2> ....");

//     long timeout = atol(argv[1]);
//     int noOfPorts = argc - 2;

//     int servSock[noOfPorts];

//     int maxDescriptor = -1;

//     for(int port = 0; port < noOfPorts; port++) {

//         servSock[port] = setupTCPServerSocket(argv[port + 2]);

//         if(servSock[port] > maxDescriptor)  maxDescriptor = servSock[port];

//     }

//     fprintf(stdout, "Starting server with %d ports: Hit return to shutdown...", noOfPorts);

//     bool running = true;
//     fd_set sockSet;

//     while(running) {

//         FD_ZERO(&sockSet);

//         //for keyboard input
//         FD_SET(STDIN_FILENO, &sockSet);

//         for(int port = 0; port < noOfPorts; port++) {
//             FD_SET(servSock[port], &sockSet);
//         }

//         struct timeval selTimeout;
//         selTimeout.tv_sec = timeout;
//         selTimeout.tv_usec = 0;

//         if(select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout) == 0)
//             printf("No requests for %ld secs.... Server still alive \n", timeout);
//         else {

//             if(FD_ISSET(0, &sockSet)) {

//                 puts("Shutting down server ..");
//                 getchar();
//                 running = false;

//             }

//             for(int port = 0; port < noOfPorts; port++) {

//                 if(FD_ISSET(servSock[port], &sockSet)) {

//                     printf("Request on port %d: ", port);
                    
//                     //change this for each kind of operation--like call a diff function for read, write etc.
//                     handleTCPClient(acceptTCPConnection(servSock[port]));


//                 }

//             }

//         }

//     }

// }



int main(int argc, char *argv[]) {

    if(argc < 2)
        exitWithUserMessage("Parameter(s)", "<Port/Service>");

    int master_socket = setupTCPServerSocket(argv[1]);
    fd_set sockSet;
    fd_set exceptionSet;
    std::set<int> clientSockSet;

    int maxDescriptor = master_socket;    

    fprintf(stdout, "Starting server at port %s ....", argv[1]);

    bool running = true;
    int activity;

    while(running) {

        FD_ZERO(&sockSet);
        FD_ZERO(&exceptionSet);

        FD_SET(master_socket, &sockSet);
        FD_SET(master_socket, &exceptionSet);

        for(auto socket : clientSockSet) {

            FD_SET(socket, &sockSet);
            FD_SET(socket, &exceptionSet);
            if(maxDescriptor < socket) maxDescriptor = socket;
        } 

        // struct timeval timeout;
        // timeout.tv_sec = 5;
        // timeout.tv_usec = 0;

        activity = select(maxDescriptor + 1, &sockSet, NULL, &exceptionSet, NULL/*&timeout*/);

        switch(activity) {

            case -1:
                exitWithSystemMessage("select() failed..shutting down process");
            
            case 0:
                //printf("No requests for %ld secs...Server still alive and listening...", timeout.tv_sec);
                printf("Should never reach here ...");
                break;
            
            default:
                handleSelectActivity(master_socket, clientSockSet, sockSet, exceptionSet);

        }



    }

}