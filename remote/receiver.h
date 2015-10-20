#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NO 1234

void error(const char*);
int pollMsg(int, char*);
void connectHost(int*, int*);
void closeConnection(int, int);
