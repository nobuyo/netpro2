#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>

extern int sessionman_init(in_port_t port);
extern void sessionman_loop();