#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

extern int setup_server(in_port_t port);
extern int setup_client(char *hostname, in_port_t port);
extern int mserver_socket(in_port_t port, int num);
extern int mserver_client(int soc, int limit, void (* func)());
extern char *chop_newline(char* buf, int len);
