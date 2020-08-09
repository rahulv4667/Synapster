#define MAXMESSAGELENGTH 1024
#define BUFSIZE 1024


void exitWithUserMessage(const char *msg, const char *detailedMsg);
void exitWithSystemMessage(const char *msg);

void printSocketAddress(const struct sockaddr *address, FILE *stream);

int setupTCPClientSocket(const char *host, const char *service);
int setupTCPServerSocket(const char *service);
int acceptTCPConnection(int servSock);
void handleTCPClient(int clientSock);
