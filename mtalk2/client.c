#include <stdio.h>
#include "mtalk2.h"
#include "session.h"

main() {
    char hostname[HOSTNAME_LENGTH];

    printf("Input server's hostname: ");
    fgets(hostname, HOSTNAME_LENGTH, stdin);
    chop_newline(hostname, HOSTNAME_LENGTH);

    if ((soc=setup_client(hostname, PORT)) == -1) {
        exit(1);
    }

    session_init();

    session_loop();
}