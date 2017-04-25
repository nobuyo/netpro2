#include "session.h"
#include "tag.h"

main() {
    int soc;

    session_init(soc, 'o', 1, 1, 'x', 10, 10);

    session_loop();
}