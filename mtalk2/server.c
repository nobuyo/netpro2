#include <stdio.h>
#include <stdlib.h>
#include "sessionman.h"
#include "mtalk2.h"

int main(int argc, char *argv[]) {
    if (sessionman_init(PORT) == -1) {
        fprintf(stderr, "cannot setup server\n");
        exit(1);
    }

    sessionman_loop();
}