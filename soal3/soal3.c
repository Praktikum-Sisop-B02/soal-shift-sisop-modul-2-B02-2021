#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char const *argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("%d%s\n", i, argv[i]);
    }
    return 0;
}
