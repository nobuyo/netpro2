
#include <stdio.h>
#include <stdlib.h>
#include "mtalk2.h"
#include "session.h"

int main(int argc, char const *argv[]) {
    char hostname[HOSTNAME_LENGTH];

    printf("Input sever's hostname: ");
    fgets(hostname, HOSTNAME_LENGTH, stdin);
    chop_newline(hostname, HOSTNAME_LENGTH);

    if ((session_setupclient(hostname, PORT)) < 0) {
        exit(1);
    }

    session_init();

    session_loop();

    return 0;
}
