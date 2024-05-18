#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    char buf[BUFSIZE];
    int bytes_copied = garfield(buf, BUFSIZE);
    if (bytes_copied < 0) {
        printf(1, "Error: Failed to retrieve Garfield ASCII art\n");
    } else {
        printf(1, "%s\n", buf);
    }
    exit();
}
