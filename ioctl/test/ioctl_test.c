#include "../ioctl.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

int main(void) {
    unsigned long ioctl_arg = 0;
    ssize_t bytes_read = 0;

    char trash[BUFLEN];
    char message[BUFLEN];
    memset(message, 0, BUFLEN);

    int file_desc = open(DEVICE_PATH, O_RDWR);

    ioctl(file_desc, IOCTL_VALGET, &ioctl_arg);
    printf("Initial IOCTL_VALGET value: %lu\n", ioctl_arg);

    bytes_read = read(file_desc, message, BUFLEN);
    read(file_desc, trash, BUFLEN);
    message[bytes_read] = '\0';

    printf("Message before IOCTL_VALSET:\n\t%s", message);

    ioctl(file_desc, IOCTL_VALSET, ioctl_arg + 1);

    ioctl(file_desc, IOCTL_VALGET, &ioctl_arg);
    printf("Current IOCTL_VALGET value: %lu\n", ioctl_arg);

    bytes_read = read(file_desc, message, BUFLEN);
    read(file_desc, trash, BUFLEN);
    message[bytes_read] = '\0';

    printf("Message after IOCTL_VALSET:\n\t%s", message);

    close(file_desc);

    return EXIT_SUCCESS;
}