#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "utilities.h"

static const int MAXPENDING = 5;

int setupTCPServerSocket(const char* service) {

    struct addrinfo addrCriteria;
    memset(&addrCriteria, 0, sizeof(addrCriteria));
    addrCriteria.ai_family = AF_UNSPEC;
    addrCriteria.ai_socktype = SOCK_STREAM;
    addrCriteria.ai_protocol = IPPROTO_TCP;
    addrCriteria.ai_flags = AI_PASSIVE;

    struct addrinfo *servAddr;
    int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);

    int servSock = -1;

    for(struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {

        servSock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

        if(servSock < 0) continue;

        if((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) && (listen(servSock, MAXPENDING) == 0)) {

            struct sockaddr_storage localAddr;
            socklen_t addrSize = sizeof(localAddr);
            if(getsockname(servSock, (struct sockaddr*)&localAddr, &addrSize), 0)
                exitWithSystemMessage("getsockname() failed");
            
            fputs("Binding to ", stdout);
            printSocketAddress((struct sockaddr*)&localAddr, stdout);
            fputc('\n', stdout);
            break;

        }

        close(servSock);
        servSock = -1;

    }

    freeaddrinfo(servAddr);
    return servSock;

}




int acceptTCPConnection(int servSock) {

    struct sockaddr_storage clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientSock = accept(servSock, (struct sockaddr*) &clientAddr, &clientAddrLen);

    if(clientSock < 0)
        return -1;
    
    fputs("Handling client: ", stdout);
    printSocketAddress((struct sockaddr*)&clientAddr, stdout);
    fputc('\n', stdout);
    fprintf(stdout, "Client Socket ID: %d", clientSock);    

    return clientSock;

}



