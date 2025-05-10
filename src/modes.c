#include "modes.h"

void raw_mode() {
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    cfmakeraw(&newt);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}