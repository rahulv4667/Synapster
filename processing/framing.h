#include <stdint.h>
#include <stdio.h>

int getNextMessage(FILE *in, uint8_t *buf, size_t bufSize, uint8_t *type);
int putMessage(uint8_t buf[], size_t msgSize, uint8_t type, FILE *out);