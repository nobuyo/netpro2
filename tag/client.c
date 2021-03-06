#include "session.h"
#include "tag.h"

main() {
    int soc;
    char hostname[HOSTNAME_LENGTH];

    printf("Input server's hostname: ");
    fgets(hostname, HOSTNAME_LENGTH, stdin);
    chop_newline(hostname, HOSTNAME_LENGTH);

    if ((soc=setup_client(hostname, PORT)) == -1) {
        exit(1);
    }

    session_init(soc, 'o', 1, 1, 'x', 10, 10);

    session_loop();
}