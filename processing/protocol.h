#include <stdio.h>
#include <stdint.h>

static const char *MAGIC = "Sharing";
static uint8_t MAGIC_BIN = 42;
static uint8_t SEARCH_REQUEST = 1;
static uint8_t SEARCH_RESPONSE = 11;
static uint8_t GET_REQUEST = 2;
static uint8_t GET_RESPONSE = 22;
static uint8_t JOIN_REQUEST = 3;
static uint8_t JOIN_RESPONSE = 33;
static uint8_t QUIT_REQUEST = 4;
static uint8_t QUIT_RESPONSE = 44;
static uint64_t MAX_FILE_SIZE = (1<<50) - 1;        //2^50 - 1
static uint64_t MAX_MESSAGE_SIZE = (1<<63) - 1;     //2^63 - 1
static uint32_t MAX_LINE = 4096;

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


typedef struct JoinRequest JoinRequest;
typedef struct JoinResponse JoinResponse;

typedef struct SearchRequest SearchRequest;
typedef struct SearchResponse SearchResponse;

typedef struct GetRequest GetRequest;
typedef struct GetResponse GetResponse;

typedef struct QuitRequest QuitRequest;
typedef struct QuitResponse QuitResponse;

typedef struct KeywordMatch KeywordMatch;




