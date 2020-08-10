#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "utilities.h"
#include "protocol.h"
#include "framing.h"
#include "encoding.h"

// void *getRequestHandler(uint8_t type) {
    
//     switch(type){
//         case 1:
//             RequestHandle *reqHandle = (RequestHandle*)malloc(sizeof(RequestHandle));
//         case 3: return encodeGetRequest;
//         case 5: return encodeJoinRequest;
//         case 7: return encodeQuitRequest;
//         default: return NULL;
//     }

// }

// void *getResponseHandler() {

// }

// int decodeHandler(uint8_t *buf, uint8_t type) {

//     void *handler;
//     if(type % 2 == 0) {

//         handler = (void*)getRequestHandler(type);
//         if(handler == NULL) return -1;

//         requestDecoder reqDec =  (requestDecoder)handler;
//         reqDec()

//     }
//     else
//         handler = requestDecoder;
    


// }

int decodeHandler(uint8_t* buf, uint8_t type, int clientSock);
int handleSearchRequest(SearchRequest *req, int clientSock);
int handleSearchResponse(SearchResponse *res, int clientSock);
int handleGetRequest(GetRequest *req, int clientSock);
int handleGetResponse(GetResponse *res, int clientSock);
int handleJoinRequest(JoinRequest *req, int clientSock);
int handleJoinResponse(JoinResponse *res, int clientSock);
int handleQuitRequest(QuitRequest *req, int clientSock);
int handleQuitResponse(QuitResponse *res, int clientSock);





//Echo handling
//TODO: change this
void handleTCPClient(int clientSock) {

    // char buffer[BUFSIZE];

    // ssize_t numBytesRecvd = recv(clientSock, buffer, BUFSIZE, 0);

    // if(numBytesRecvd < 0)
    //     exitWithSystemMessage("recv() failed");
    
    // while(numBytesRecvd > 0) {

    //     ssize_t numBytesSent = send(clientSock, buffer, numBytesRecvd, 0);

    //     if(numBytesSent < 0)
    //         exitWithSystemMessage("send() failed");
    //     else if(numBytesSent != numBytesRecvd)
    //         exitWithUserMessage("send()", "sent unexpected number of bytes");
        
    //     numBytesRecvd = recv(clientSock, buffer, BUFSIZE, 0);

    //     if(numBytesRecvd < 0)
    //         exitWithSystemMessage("recv() failed");

    // }
    uint8_t buffer[MAX_WIRE_SIZE];
    uint8_t msgType;
    
    FILE *sockStream = fdopen(clientSock, "r+");
    if(sockStream == NULL) {
        fprintf(stderr, "Error occured while trying to get a stream pointer for socket %d\n", clientSock);
        return;
    }

    size_t requestSize = getNextMessage(sockStream, buffer, MAX_WIRE_SIZE, &msgType);

    decodeHandler(buffer, msgType, clientSock);

    close(clientSock);

}



int decodeHandler(uint8_t* buf, uint8_t type, int clientSock) {

    switch(type){

        case SEARCH_REQUEST:
            SearchRequest *req = (SearchRequest*)malloc(sizeof(SearchRequest));
            if(!decodeSearchRequest(buf, MAX_WIRE_SIZE, req))  {
                fprintf(stderr, "Error while trying to decode search request\n");
                return -1;
            }
            return handleSearchRequest(req, clientSock);
        
        case SEARCH_RESPONSE:
            SearchResponse *res = (SearchResponse*)malloc(sizeof(SearchResponse));
            if(!decodeSearchResponse(buf, MAX_WIRE_SIZE, res)) {
                fprintf(stderr, "Error while trying to decode search response\n");
                return -1;
            }
            return handleSearchResponse(res, clientSock);

        case GET_REQUEST:
            GetRequest *req = (GetRequest*)malloc(sizeof(GetRequest));
            if(!decodeGetRequest(buf, MAX_WIRE_SIZE, req))  {
                fprintf(stderr, "Error while trying to decode get request\n");
                return -1;
            }
            return handleGetRequest(req, clientSock);
        
        case GET_RESPONSE:
            GetResponse *res = (GetResponse*)malloc(sizeof(GetResponse));
            if(!decodeGetResponse(buf, MAX_WIRE_SIZE, res)) {
                fprintf(stderr, "Error while trying to decode get response\n");
                return -1;
            }
            return handleGetResponse(res, clientSock);

        case JOIN_REQUEST:
            JoinRequest *req = (JoinRequest*)malloc(sizeof(JoinRequest));
            if(!decodeJoinRequest(buf, MAX_WIRE_SIZE, req))  {
                fprintf(stderr, "Error while trying to decode join request\n");
                return -1;
            }
            return handleJoinRequest(req, clientSock);
        
        case JOIN_RESPONSE:
            JoinResponse *res = (JoinResponse*)malloc(sizeof(JoinResponse));
            if(!decodeJoinResponse(buf, MAX_WIRE_SIZE, res)) {
                fprintf(stderr, "Error while trying to decode join response\n");
                return -1;
            }
            return handleJoinResponse(res, clientSock);

        case QUIT_REQUEST:
            QuitRequest *req = (QuitRequest*)malloc(sizeof(QuitRequest));
            if(!decodeQuitRequest(buf, MAX_WIRE_SIZE, req))  {
                fprintf(stderr, "Error while trying to decode quit request\n");
                return -1;
            }
            return handleQuitRequest(req, clientSock);
        
        case QUIT_RESPONSE:
            QuitResponse *res = (QuitResponse*)malloc(sizeof(QuitResponse));
            if(!decodeQuitResponse(buf, MAX_WIRE_SIZE, res)) {
                fprintf(stderr, "Error while trying to decode quit response\n");
                return -1;
            }
            return handleQuitResponse(res, clientSock);

        default: return -1;

    }

}




int handleSearchRequest(SearchRequest *req, int clientSock){

    

}



int handleSearchResponse(SearchResponse *res, int clientSock){



}



int handleGetRequest(GetRequest *req, int clientSock){



}



int handleGetResponse(GetResponse *res, int clientSock){



}



int handleJoinRequest(JoinRequest *req, int clientSock){



}



int handleJoinResponse(JoinResponse *res, int clientSock){



}



int handleQuitRequest(QuitRequest *req, int clientSock){



}



int handleQuitResponse(QuitResponse *res, int clientSock){



}


