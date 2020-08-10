#include "protocol.h"

#define DELIMSTR " "

size_t encodeJoinRequest(const JoinRequest *req, uint8_t *outBuf);
size_t encodeJoinResponse(const JoinResponse *res, uint8_t *outBuf);

size_t encodeSearchRequest(const SearchRequest *req, uint8_t *outBuf);
size_t encodeSearchResponse(const SearchResponse *res, uint8_t *outBuf);

size_t encodeGetRequest(const GetRequest *req, uint8_t *outBuf);
size_t encodeGetResponse(const GetResponse *res, uint8_t *outBuf);

size_t encodeQuitRequest(const QuitRequest *req, uint8_t *outBuf);
size_t encodeQuitResponse(const QuitResponse *res, uint8_t *outBuf);



bool decodeJoinRequest(uint8_t *inBuf, const size_t bufSize, JoinRequest *req);
bool decodeJoinResponse(uint8_t *inBuf, const size_t bufSize, JoinResponse *res);

bool decodeSearchRequest(uint8_t *inBuf, const size_t bufSize, SearchRequest *req);
bool decodeSearchResponse(uint8_t *inBuf, const size_t bufSize, SearchResponse *res);

bool decodeGetRequest(uint8_t *inBuf, const size_t bufSize, GetRequest *req);
bool decodeGetResponse(uint8_t *inBuf, const size_t bufSize, GetResponse *res);

bool decodeQuitRequest(uint8_t *inBuf, const size_t bufSize, QuitRequest *req);
bool decodeQuitResponse(uint8_t *inBuf, const size_t bufSize, QuitResponse *res);


//function pointers
// typedef size_t (*requestDecoder)(const void*, uint8_t*);
// typedef bool (*responseDecoder)(uint8_t*, const size_t, void*);

// typedef size_t (*requestEncoder)(const void*, uint8_t*);
// typedef bool (*responseEncdoer)(uint8_t*, const size_t, void*);
