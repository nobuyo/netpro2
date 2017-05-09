#include <stdio.h>
#include "sessionman.h"
#include "mylib.h"

main(int argc, char *argv[]) {
    int num;
    int soc;
    int maxfd;

    if (argc != 2) {
        fprintf(stderr, "Usage: s attendants\n");
        exit(1);
    }

    if ((num = atoi(argv[1]))<=0 || num > MAX_ATTENDANTS) {
        fprintf(stderr, "attendants limit= %d\n", MAX_ATTENDANTS);
    }

    if ((soc = mserver_maccept(PORT, num)) == -1) {
        fprintf(stderr, "cannot setup server.\n");
    }

    maxfd = mserver_maccept(soc, num, enter);

    sessionman_init(num, maxfd);

    sessionman_loop();
}