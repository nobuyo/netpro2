#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>

extern int session_setupclient(char *hostname, in_port_t port);
extern void session_init();
extern void session_loop();