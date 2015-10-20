#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT_NO 1234

int sendMessage(int, char*);
int connectRemote(char*);
void closeConnection(int);
void error(const char*);
