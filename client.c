#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT (in_port_t)50000
#define BUF_LEN 512

int main(void) {
    struct hostent *server_ent;
    struct sockaddr_in server;
    int soc_waiting;
    int soc;
    char hostname[] = "ayu020";
    char buf[BUF_LEN];

    if ((server_ent = gethostbyname(hostname)) == NULL ) {
        perror("gethostbyname");
        exit(1);
    }

    printf("%s\n", server_ent->h_addr);

    memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    memcpy((char *)&server.sin_addr, server_ent->h_addr, server_ent->h_length);


    if ((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("socket");
        exit(1);
    }

    if (connect(soc, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("connect");
        exit(1);
    }

    write(1,"Wait\n", 5);

    do {
        int n;
        n = read(soc, buf, BUF_LEN);
        write(1, buf, n);
        write(1, "> ", 2);
        n = read(0, buf, BUF_LEN);
        write(soc, buf, n);
    } while ( strncmp(buf, "quit", 4) != 0 );

    close(soc);

}
