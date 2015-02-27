#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

#define FILENAME "/dev/secret"

int main(int argc, char *argv[]) {
    uid_t uid;
    int res;
    int fd = open(FILENAME, O_WRONLY);
    printf("Opening ... fd=%d\n", fd);

    res = write(fd, "Super random secret\n", 21);
    printf("Writing ... res=%d\n", res);

    /* try grant */
    if (argc > 1 && 0 != (uid = atoi(argv[1]))) {
        if (res = ioctl(fd, SSGRANT, &uid)) {
            fprintf(stderr, "ioctl");
        }
        printf("Trying to change owner to %d ... res=%d\n", uid, res);
    }

    res = close(fd);
    printf("closing ... res=%d\n", res);
}
