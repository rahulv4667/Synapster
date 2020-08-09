#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <set>
#include "utilities.h"


void handleSelectActivity(int master_socket, std::set<int>& clientSock, fd_set& readSet, fd_set& exceptSet) {

    if(FD_ISSET(master_socket, &exceptSet)) 
        exitWithSystemMessage("Error occured in the master socket of server. Shutting the server down");
    
    if(FD_ISSET(master_socket, &readSet)) {

        int clientSocket = acceptTCPConnection(master_socket);
        if(clientSocket < 0)
            fputs("Error occured while accepting a client socket", stderr);
        else clientSock.insert(clientSocket);

    }
        
    for(auto socket : clientSock) {

        if(FD_ISSET(socket, &readSet))
            handleTCPClient(socket);

        if(FD_ISSET(socket, &exceptSet)) {

            fprintf(stderr, "Error occured on a client socket: %d. Closing the socket", socket);
            close(socket);
            clientSock.erase(clientSock.find(socket));

        }

    }


}