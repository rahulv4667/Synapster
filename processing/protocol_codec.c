#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocol.h"
#include "encoding.h"
#include "utilities.h"



/**
  * 
  * Coding JoinRequest
  * 
  * Wire Format:
  * 
  *   "MAGIC recordsCount keyword1 filename1 host1 service1 keyword2 filename2 host2 service2 .......keywordN hostN serviceN"
  * WARNING: Message will be silently truncated if buffer is too small
  * 
**/
size_t encodeJoinRequest(const JoinRequest *req, uint8_t *outBuf) {

    uint8_t *bufPtr = outBuf;
    unsigned long size = (size_t) MAX_MESSAGE_SIZE;

    unsigned long writtenBytes = snprintf((char*)bufPtr, size, "%s %d ", MAGIC, req->recordsCount);
    bufPtr += writtenBytes;
    size -= writtenBytes;

    for(unsigned int i = 0; i < req->recordsCount; i++) {
        
        KeywordMatch keyMatch = req->matchingList[i];
        writtenBytes = snprintf((char*)bufPtr, size, "%s %s %s %s ", 
                keyMatch.keyword,
                keyMatch.filename,
                keyMatch.host,
                keyMatch.service);

        bufPtr += writtenBytes; //to remove null character at the end
        size -= writtenBytes;

    }

    return (size_t)(bufPtr - outBuf);

}


bool decodeJoinRequest(uint8_t *inBuf, const size_t bufSize, JoinRequest *req) {

    char *token;
    token = strtok((char*) inBuf, DELIMSTR);

    if(token == NULL || strcmp(token, MAGIC) != 0) return false;

    token = strtok(NULL, DELIMSTR);
    if(token == NULL) return false;
    
    req->recordsCount = strtoul(token, NULL, 10);

    if(req->recordsCount == 0) return false;

    req->matchingList = (KeywordMatch*)malloc((req->recordsCount) * sizeof(KeywordMatch));

    for(unsigned long i = 0; i < req->recordsCount; i++) {

        token = strtok(NULL, DELIMSTR);
        if(token == NULL) return false;
        req->matchingList[i].keyword = token;

        token = strtok(NULL, DELIMSTR);
        if(token == NULL) return false;
        req->matchingList[i].filename = token;

        token = strtok(NULL, DELIMSTR);
        if(token == NULL) return false;
        req->matchingList[i].host = token;

        token = strtok(NULL, DELIMSTR);
        if(token == NULL) return false;
        req->matchingList[i].service = token;

    }

    return true;    

}


/**
 * 
 * Coding SearchRequest
 * 
 * Wire Format:
 * 
 *  "MAGIC keyword"
 * 
 * WARNING: Message will be silently truncated if buffer size is small
 * 
 * */

size_t encodeSearchRequest(const SearchRequest *req, uint8_t *outBuf) {

    uint8_t *bufPtr = outBuf;
    unsigned long size = (size_t) MAX_MESSAGE_SIZE;

    unsigned long writtenBytes = snprintf((char*) outBuf, size, "%s %s", MAGIC, req->keyword);

    bufPtr += writtenBytes;
    size -= writtenBytes;

    return (size_t)(bufPtr - outBuf);

}


bool decodeSearchRequest(uint8_t *inBuf, const size_t bufSize, SearchRequest *req) {

    char *token;
    token = strtok((char*) inBuf, DELIMSTR);

    if(token == NULL || stcmp(token, MAGIC) != 0) return false;

    token = strtok(NULL, DELIMSTR);

    if(token == NULL) return false;

    req->keyword = token;

    return true;

}


/**
 * 
 * Coding GetRequest
 * 
 * Wire Format:
 * 
 *  "MAGIC filename"
 * 
 * */

size_t encodeGetRequest(const GetRequest *req, uint8_t *outBuf) {

    uint8_t *bufPtr = outBuf;
    unsigned long size = (size_t) MAX_MESSAGE_SIZE;

    unsigned long writtenBytes = snprintf((char*) outBuf, size, "%s %s", MAGIC, req->filename);

    bufPtr += writtenBytes;
    size -= writtenBytes;

    return (size_t)(bufPtr - outBuf);

}

bool decodeGetRequest(uint8_t *inBuf, const size_t bufSize, GetRequest *req) {

    char *token;
    token = strtok((char*) inBuf, DELIMSTR);

    if(token == NULL || stcmp(token, MAGIC) != 0) return false;

    token = strtok(NULL, DELIMSTR);

    if(token == NULL) return false;

    req->filename = token;

    return true;

}



/**
 * 
 * Coding QuitRequest
 * 
 * Wire Format: 
 *      "MAGIC host service"
 * 
 * */

size_t encodeQuitRequest(const QuitRequest *req, uint8_t *outBuf) {

    uint8_t *bufPtr = outBuf;
    unsigned long size = (size_t) MAX_MESSAGE_SIZE;

    unsigned long writtenBytes = snprintf((char*) outBuf, size, "%s %s %s", MAGIC, req->host, req->service);

    bufPtr += writtenBytes;
    size -= writtenBytes;

    return (size_t)(bufPtr - outBuf);

}

bool decodeQuitRequest(uint8_t *inBuf, const size_t bufSize, QuitRequest *req) {

    char *token;
    token = strtok((char*) inBuf, DELIMSTR);

    if(token == NULL || stcmp(token, MAGIC) != 0) return false;

    token = strtok(NULL, DELIMSTR);
    if(token == NULL) return false;
    req->host = token;

    token = strtok(NULL, DELIMSTR);
    if(token == NULL) return false;
    req->service = token;

    return true;

}



/**
 * 
 * Coding JoinResponse
 * 
 * Wire Format: 
 * 
 *  1byte - MAGIC_BIN == 42
 *  4byte - status
 *  4byte - keywordsAddedCount
 * 
 * Referred this SO answer: https://stackoverflow.com/questions/20695586/casting-uint8-to-uint32
 * Implementation suggestion: use memcpy to convert uint8_t bytes to uint32_t type
 * */

size_t encodeJoinResponse(const JoinResponse *res, uint8_t *outBuf) {

    uint8_t *bufPtr = outBuf;
    unsigned long size = (size_t) MAX_MESSAGE_SIZE;

    uint32_t status = (uint32_t) res->status;
    uint32_t keywordsAddedCount = (uint32_t) res->keywordsAddedCount;

    uint8_t bytes[8];
    uint32_t initialMask = (1<<8) - 1; //2^8 - 1

    bytes[0] = status & (initialMask<<(3*8));
    bytes[1] = status & (initialMask<<(2*8));
    bytes[2] = status & (initialMask<<(1*8));
    bytes[3] = status & (initialMask); 

    bytes[4] = keywordsAddedCount & (initialMask<<(3*8));
    bytes[5] = keywordsAddedCount & (initialMask<<(2*8));
    bytes[6] = keywordsAddedCount & (initialMask<<(1*8));
    bytes[7] = keywordsAddedCount & (initialMask); 

    unsigned long writtenBytes = snprintf((char*) bufPtr, size, "%c", (char)MAGIC_BIN);

    bufPtr += writtenBytes;
    size -= writtenBytes;

    for(int i=0; i<8; i++) {
        snprintf((char*) bufPtr, size, "%c", (char)bytes[i]);
        bufPtr += writtenBytes;
        size -= writtenBytes;
    }
    
    return (size_t)(bufPtr - outBuf);

}

bool decodeJoinResponse(uint8_t *inBuf, const size_t bufSize, JoinResponse *res) {

    if(bufSize < 9) return false;
    uint8_t *bufPtr = inBuf;

    if(*bufPtr != MAGIC_BIN) return false;

    uint32_t status = 0;
    
    uint32_t temp = 0;
    
    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    status |= temp<<(3*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    status |= temp<<(2*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    status |= temp<<(1*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    status |= temp;
    temp = 0;

    uint32_t keywordsAddedCount = 0;


    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    keywordsAddedCount |= temp<<(3*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    keywordsAddedCount |= temp<<(2*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    keywordsAddedCount |= temp<<(1*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    keywordsAddedCount |= temp;
    temp = 0;

    
    res->status = status;
    res->keywordsAddedCount = keywordsAddedCount;

    return true;

}



/**
 * 
 * Coding SearchResponse
 * 
 * Wire Format:
 * 
 *  "MAGIC responseCount keyword1 filename1 host1 service1 keyword2 filename2 host2 service2 ..... keywordN filenameN hostN serviceN"
 * 
 * */
size_t encodeSearchResponse(const SearchResponse *res, uint8_t *outBuf) {

    uint8_t *bufPtr = outBuf;
    unsigned long size = (size_t) MAX_MESSAGE_SIZE;

    unsigned long writtenBytes = snprintf((char*)bufPtr, size, "%s %d ", MAGIC, res->responseCount);
    bufPtr += writtenBytes;
    size -= writtenBytes;

    for(unsigned int i = 0; i < res->responseCount; i++) {
        
        KeywordMatch keyMatch = res->matchedList[i];
        writtenBytes = snprintf((char*)bufPtr, size, "%s %s %s %s ", 
                keyMatch.keyword,
                keyMatch.filename,
                keyMatch.host,
                keyMatch.service);

        bufPtr += writtenBytes; //to remove null character at the end
        size -= writtenBytes;

    }

    return (size_t)(bufPtr - outBuf);

}


bool decodeSearchResponse(uint8_t *inBuf, const size_t bufSize, SearchResponse *res) {

    char *token;
    token = strtok((char*) inBuf, DELIMSTR);

    if(token == NULL || strcmp(token, MAGIC) != 0) return false;

    token = strtok(NULL, DELIMSTR);
    if(token == NULL) return false;
    
    res->responseCount = strtoul(token, NULL, 10);

    if(res->responseCount == 0) return false;

    res->matchedList = (KeywordMatch*)malloc((res->responseCount) * sizeof(KeywordMatch));

    for(unsigned long i = 0; i < res->responseCount; i++) {

        token = strtok(NULL, DELIMSTR);
        if(token == NULL) return false;
        res->matchedList[i].keyword = token;

        token = strtok(NULL, DELIMSTR);
        if(token == NULL) return false;
        res->matchedList[i].filename = token;

        token = strtok(NULL, DELIMSTR);
        if(token == NULL) return false;
        res->matchedList[i].host = token;

        token = strtok(NULL, DELIMSTR);
        if(token == NULL) return false;
        res->matchedList[i].service = token;

    }

    return true;    

}


/**
 * 
 * Coding GetResponse
 * 
 * Wire Format:
 * 
 *  "MAGIC filesize filename "+<raw_bytes>
 * 
 * Referred this source: https://github.com/omair18/Socket-Programming-in-C/blob/master/send_file.c
 * 
 * */

size_t encodeGetResponse(const GetResponse *res, uint8_t *outBuf) {

    uint8_t *bufPtr = outBuf;
    unsigned long size = (size_t) MAX_MESSAGE_SIZE;

    unsigned long writtenBytes = snprintf((char*) bufPtr, size, "%s %s %lu ", MAGIC, res->filename, res->fileSize);
    bufPtr += writtenBytes;
    size -= writtenBytes;

    unsigned n;
    char sendLine[MAX_LINE];
    memset(sendLine, 0, MAX_LINE);
    
    while((n = fread(sendLine, sizeof(char), MAX_LINE, res->fileStream)) > 0) {

        if(n != MAX_LINE && ferror(res->fileStream))
            exitWithSystemMessage("file read failed");
        
        writtenBytes = snprintf((char*) bufPtr, size, "%s", sendLine);

        bufPtr += writtenBytes;
        size -= writtenBytes;

        memset(sendLine, 0, MAX_LINE);

    }

    return (size_t)(bufPtr - outBuf);

}



bool decodeGetResponse(uint8_t *inBuf, const size_t bufSize, GetResponse *res) {

    char *token;

    token = strtok_r((char*) inBuf, " ", inBuf);

    if(token == NULL || strcmp(token, MAGIC) != 0) return false;

    token = strtok_r((char*) inBuf, " ", inBuf);
    if(token == NULL) return false;
    res->fileSize = strtoul(token, NULL, 10);

    token = strtok_r((char*) inBuf, " ", inBuf);
    if(token == NULL) return false;
    res->fileStream = fopen(token, "w");

    unsigned long fileSize = res->fileSize;
    unsigned long writeBufSize = MAX_LINE;
    unsigned long writtenBytes = 0;

    while(fileSize) {
        
        writeBufSize = (MAX_LINE < fileSize)?MAX_LINE:fileSize;
        
        if((writtenBytes = fwrite(inBuf, sizeof(char), writeBufSize, res->fileStream)) != writeBufSize)
            exitWithSystemMessage("fwrite() error");
        
        fileSize -= writtenBytes;
        inBuf += writtenBytes;

    }

    fclose(res->fileStream);
    res->fileStream = fopen(token, "r");

    return true;

}




/**
 * 
 * 
 * Coding QuitResponse
 * 
 * Wire Format: 
 * 
 *      1byte - MAGIC_BIN
 *      4byte - status
 *      4byte - recordsDeletedCount
 * 
 * */

size_t encodeQuitResponse(const QuitResponse *res, uint8_t *outBuf) {

    uint8_t *bufPtr = outBuf;
    unsigned long size = (size_t) MAX_MESSAGE_SIZE;

    uint32_t status = (uint32_t) res->status;
    uint32_t recordsDeletedCount = (uint32_t) res->recordsDeletedCount;

    uint8_t bytes[8];
    uint32_t initialMask = (1<<8) - 1; //2^8 - 1

    bytes[0] = status & (initialMask<<(3*8));
    bytes[1] = status & (initialMask<<(2*8));
    bytes[2] = status & (initialMask<<(1*8));
    bytes[3] = status & (initialMask); 

    bytes[4] = recordsDeletedCount & (initialMask<<(3*8));
    bytes[5] = recordsDeletedCount & (initialMask<<(2*8));
    bytes[6] = recordsDeletedCount & (initialMask<<(1*8));
    bytes[7] = recordsDeletedCount & (initialMask); 

    unsigned long writtenBytes = snprintf((char*) bufPtr, size, "%c", (char)MAGIC_BIN);

    bufPtr += writtenBytes;
    size -= writtenBytes;

    for(int i=0; i<8; i++) {
        snprintf((char*) bufPtr, size, "%c", (char)bytes[i]);
        bufPtr += writtenBytes;
        size -= writtenBytes;
    }
    
    return (size_t)(bufPtr - outBuf);    

}


bool decodeQuitResponse(uint8_t *inBuf, const size_t bufSize, QuitResponse *res) {

    if(bufSize < 9) return false;
    uint8_t *bufPtr = inBuf;

    if(*bufPtr != MAGIC_BIN) return false;

    uint32_t status = 0;
    
    uint32_t temp = 0;
    
    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    status |= temp<<(3*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    status |= temp<<(2*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    status |= temp<<(1*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    status |= temp;
    temp = 0;

    uint32_t recordsDeletedCount = 0;


    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    recordsDeletedCount |= temp<<(3*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    recordsDeletedCount |= temp<<(2*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    recordsDeletedCount |= temp<<(1*8);
    temp = 0;

    bufPtr++;
    temp = *((uint32_t*)bufPtr);
    recordsDeletedCount |= temp;
    temp = 0;

    
    res->status = status;
    res->recordsDeletedCount = recordsDeletedCount;

    return true;

}
