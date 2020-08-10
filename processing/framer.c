#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include "utilities.h"
#include "protocol.h"


/**
 * 
 * Wire Format for frame
 * 
 * 4 bytes - size
 * 1 byte  - msgType
 * nbytes  = message
 *
 * */

int getNextMessage(FILE *in, uint8_t *buf, size_t bufSize, uint8_t *type) {

    uint32_t mSize = 0;
    uint32_t extra = 0;

    if(fread(&mSize, sizeof(uint32_t), 1, in) != 1) return -1;
    if(fread(type, sizeof(uint8_t), 1, in) != 1) return -1;

    mSize = ntohl(mSize);

    if(mSize < bufSize) {

        extra = mSize - bufSize;
        mSize = bufSize;

    }

    if(fread(buf, sizeof(uint8_t), mSize, in) != mSize) {

        fprintf(stderr, "Framing error: expected %lu, read less\n", mSize);
        return -1;

    }

    if(extra > 0) {

        uint8_t waste[BUFSIZE];
        fread(waste, sizeof(uint8_t), extra, in);
        return -(mSize+extra);

    } else {
        return mSize;
    }

}



int putMessage(uint8_t buf[], size_t msgSize, uint8_t *type, FILE *out) {

    if(msgSize > UINT32_MAX) return -1;
    uint32_t payloadSize = htonl(msgSize);

    if((fwrite(&payloadSize, sizeof(uint32_t), 1, out)) != 1
        || (fwrite(type, sizeof(uint8_t), 1, out) != 1)
        || (fwrite(buf, sizeof(uint8_t), msgSize, out) != msgSize))
        return -1;
    
    fflush(out);
    return msgSize;

}