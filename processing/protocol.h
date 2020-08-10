#include <stdio.h>
#include <stdint.h>

static const char *MAGIC = "Sharing";
static const uint8_t MAGIC_BIN = 42;
static const uint8_t SEARCH_REQUEST = 1;
static const uint8_t SEARCH_RESPONSE = 2;
static const uint8_t GET_REQUEST = 3;
static const uint8_t GET_RESPONSE = 4;
static const uint8_t JOIN_REQUEST = 5;
static const uint8_t JOIN_RESPONSE = 6;
static const uint8_t QUIT_REQUEST = 7;
static const uint8_t QUIT_RESPONSE = 8;
static const size_t MAX_MESSAGE_SIZE = (1<<25) - 1;     //2^54 - 1
static const uint32_t MAX_WIRE_SIZE = (1<<25) - 1;
static const uint32_t MAX_FRAME_SIZE = (1<<31) - 1;
static const uint32_t MAX_LINE = 4096;

struct JoinRequest {
    //encoded as string
    unsigned long recordsCount; 
    KeywordMatch *matchingList; 
};

struct JoinResponse {
    //encoded as binary
    unsigned long status;
    unsigned long keywordsAddedCount;
};

struct SearchRequest {
    char *keyword;
};

struct SearchResponse {
    unsigned long responseCount; 
    KeywordMatch *matchedList;  
};

struct KeywordMatch {
    char *keyword;
    char *filename;
    char *host;
    char *service;
};

struct GetRequest {
    char *filename;
};

struct GetResponse {
    //encoded as binary
    unsigned long fileSize;
    char *filename;
    FILE *fileStream;
};

struct QuitRequest {
    //encoded as text
    char *host;
    char *service;
};

struct QuitResponse {
    //encoded as binary
    unsigned long status;
    unsigned long recordsDeletedCount;
};


// struct RequestHandle {
//     size_t (*requestCoder)(const void* reqStruct, uint8_t* buf);
//     const void* requestStructure;
// };

// struct ResponseHandle {
//     bool (*responseCoder)(uint8_t* buf, const size_t msgSize, void* resStruct);
//     const void* responseStructure;
// };

typedef struct JoinRequest JoinRequest;
typedef struct JoinResponse JoinResponse;

typedef struct SearchRequest SearchRequest;
typedef struct SearchResponse SearchResponse;

typedef struct GetRequest GetRequest;
typedef struct GetResponse GetResponse;

typedef struct QuitRequest QuitRequest;
typedef struct QuitResponse QuitResponse;

typedef struct KeywordMatch KeywordMatch;




