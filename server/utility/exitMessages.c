#include <stdio.h>
#include <string.h>


void exitWithUserMessage(const char *msg, const char *detailedMsg) {

    fputs(msg, stderr);
    fputs(" : ", stderr);
    fputs(detailedMsg, stderr);
    fputc('\n', stderr);
    exit(1);

}


void exitWithSystemMessage(const char *msg) {

    perror(msg);
    exit(1);

}