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
#include "dynamic_db.h"

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


std::unordered_map<std::string, 
        std::vector<
            std::pair<std::string, 
                    std::pair<std::string, std::string>>
        >> dynamicDB;

int decodeAndHandle(uint8_t* buf, uint8_t type, FILE *sockStream, int clientSock);
int handleSearchRequest(SearchRequest *req, FILE *sockStream);
int handleSearchResponse(SearchResponse *res, FILE *sockStream);
int handleGetRequest(GetRequest *req, FILE *sockStream);
int handleGetResponse(GetResponse *res, FILE *sockStream);
int handleJoinRequest(JoinRequest *req, FILE *sockStream);
int handleJoinResponse(JoinResponse *res, FILE *sockStream);
int handleQuitRequest(QuitRequest *req, FILE *sockStream, int clientSock);
int handleQuitResponse(QuitResponse *res, FILE *sockStream, int clientSock);





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

    decodeAndHandle(buffer, msgType, sockStream, clientSock);

    //close(clientSock);

}



int decodeAndHandle(uint8_t* buf, uint8_t type, FILE *sockStream, int clientSock) {

    switch(type){

        case SEARCH_REQUEST: {
            SearchRequest *req = (SearchRequest*)malloc(sizeof(SearchRequest));
            if(!decodeSearchRequest(buf, MAX_WIRE_SIZE, req))  {
                fprintf(stderr, "Error while trying to decode search request\n");
                return -1;
            }
            return handleSearchRequest(req, sockStream);
        }
        
        case SEARCH_RESPONSE: {
            SearchResponse *res = (SearchResponse*)malloc(sizeof(SearchResponse));
            if(!decodeSearchResponse(buf, MAX_WIRE_SIZE, res)) {
                fprintf(stderr, "Error while trying to decode search response\n");
                return -1;
            }
            return handleSearchResponse(res, sockStream);
        }

        case GET_REQUEST: {
            GetRequest *req = (GetRequest*)malloc(sizeof(GetRequest));
            if(!decodeGetRequest(buf, MAX_WIRE_SIZE, req))  {
                fprintf(stderr, "Error while trying to decode get request\n");
                return -1;
            }
            return handleGetRequest(req, sockStream);
        }
        
        case GET_RESPONSE: {
            GetResponse *res = (GetResponse*)malloc(sizeof(GetResponse));
            if(!decodeGetResponse(buf, MAX_WIRE_SIZE, res)) {
                fprintf(stderr, "Error while trying to decode get response\n");
                return -1;
            }
            return handleGetResponse(res, sockStream);
        }

        case JOIN_REQUEST: {
            JoinRequest *req = (JoinRequest*)malloc(sizeof(JoinRequest));
            if(!decodeJoinRequest(buf, MAX_WIRE_SIZE, req))  {
                fprintf(stderr, "Error while trying to decode join request\n");
                return -1;
            }
            return handleJoinRequest(req, sockStream);
        }
        
        case JOIN_RESPONSE: {
            JoinResponse *res = (JoinResponse*)malloc(sizeof(JoinResponse));
            if(!decodeJoinResponse(buf, MAX_WIRE_SIZE, res)) {
                fprintf(stderr, "Error while trying to decode join response\n");
                return -1;
            }
            return handleJoinResponse(res, sockStream);
        }

        case QUIT_REQUEST: {
            QuitRequest *req = (QuitRequest*)malloc(sizeof(QuitRequest));
            if(!decodeQuitRequest(buf, MAX_WIRE_SIZE, req))  {
                fprintf(stderr, "Error while trying to decode quit request\n");
                return -1;
            }
            return handleQuitRequest(req, sockStream, clientSock);
        }
        
        case QUIT_RESPONSE: {
            QuitResponse *res = (QuitResponse*)malloc(sizeof(QuitResponse));
            if(!decodeQuitResponse(buf, MAX_WIRE_SIZE, res)) {
                fprintf(stderr, "Error while trying to decode quit response\n");
                return -1;
            }
            return handleQuitResponse(res, sockStream, clientSock);
        }

        default: return -1;

    }

}





/**
 * 
 * 
 * All handlers dont free the memory that they take as parameters. Find a way to clean up this memory
 * 
 * */

//used by server
int handleSearchRequest(SearchRequest *req, FILE *sockStream) {

    std::string keyword = std::string(req->keyword);
    SearchResponse *res = (SearchResponse*)malloc(sizeof(SearchResponse));

    if(dynamicDB.count(keyword) == 0) {

        res->responseCount = 0;
        res->matchedList = NULL;

    } else {

        auto mapResult = dynamicDB[keyword];
        res->responseCount = mapResult.size();
        res->matchedList = (KeywordMatch*)malloc((res->responseCount) * sizeof(KeywordMatch));
        
        for(auto i = 0; i < mapResult.size(); i++) {

            auto filename = mapResult[i].first;
            auto address = mapResult[i].second;
            auto host = address.first;
            auto service = address.second;

            res->matchedList[i].keyword = keyword.c_str();
            res->matchedList[i].filename = filename.c_str();
            res->matchedList[i].host = host.c_str();
            res->matchedList[i].service = service.c_str();

        }

    }

    uint8_t buffer[MAX_MESSAGE_SIZE];
    size_t msgSize = encodeSearchResponse(res, buffer);
    size_t totalBytesSent = putMessage(buffer, msgSize, SEARCH_RESPONSE, sockStream);

    if(res->responseCount != 0) {

        for(int i = 0; i < res->responseCount; i++) {
            res->matchedList[i].keyword = NULL;
            res->matchedList[i].filename = NULL;
            res->matchedList[i].host = NULL;
            res->matchedList[i].service = NULL;
        }

    }

    free(res->matchedList);
    free(res);
    
    // free(req->keyword);
    // free(req);

    return totalBytesSent;

}


//used by peers
int handleSearchResponse(SearchResponse *res, FILE* sockStream){



}


//used by peers
int handleGetRequest(GetRequest *req, FILE* sockStream){



}


//used by peers
int handleGetResponse(GetResponse *res, FILE *sockStream) {



}


//used by server
int handleJoinRequest(JoinRequest *req, FILE *sockStream) {

    JoinResponse *res = (JoinResponse*)malloc(sizeof(JoinResponse));
    unsigned long status = STATUS_UNKNOWN;
    unsigned long keywordsAddedCount = 0;

    for(int i = 0; i < req->recordsCount; i++) {

        std::string keyword = req->matchingList[i].keyword;
        std::string filename = req->matchingList[i].filename;
        std::string host = req->matchingList[i].host;
        std::string service = req->matchingList[i].service;

        std::pair<std::string, std::string> address = std::make_pair(host, service);
        std::pair<std::string, std::pair<std::string, std::string>> dbEntry = std::make_pair(filename, address);

        dynamicDB[keyword].push_back(dbEntry);
        if(dynamicDB.count(keyword) > 0)
            keywordsAddedCount++;
        else {
            status = STATUS_FAILURE;
            break;
        }

    }

    res->status = (status==STATUS_UNKNOWN)?STATUS_SUCCESS:status;
    res->keywordsAddedCount = keywordsAddedCount;

    uint8_t buffer[MAX_MESSAGE_SIZE];
    size_t msgSize = encodeJoinResponse(res, buffer);
    size_t totalBytesSent = putMessage(buffer, msgSize, JOIN_RESPONSE, sockStream);

    free(res);

    return totalBytesSent;

}


//used by peers
int handleJoinResponse(JoinResponse *res, FILE *sockStream) {



}


//used by server
int handleQuitRequest(QuitRequest *req, FILE *sockStream, int clientSock) {

    QuitResponse *res = (QuitResponse*)malloc(sizeof(QuitResponse));
    unsigned long status = STATUS_UNKNOWN;

    for(auto entry = dynamicDB.begin(); entry != dynamicDB.end();) {

        auto keyword = (*entry).first;
        auto fileVec = (*entry).second;

        for(int i = 0; i < fileVec.size(); i++) {

            

        }
        

    }

}


//used by peers
int handleQuitResponse(QuitResponse *res, int clientSock){



}


