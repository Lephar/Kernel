#ifndef IOCTL_MODULE_H
#define IOCTL_MODULE_H

#include <linux/ioctl.h>

#define SUCCESS 0
#define BUFLEN 256

#define DEVICE_NAME "ioctl_dev"
#define DEVICE_PATH "/dev/ioctl_dev"

#define MAJOR_NUM 100
#define IOCTL_VALGET _IOR(MAJOR_NUM, 0, unsigned long)
#define IOCTL_VALSET _IOW(MAJOR_NUM, 1, unsigned long)

#endif //IOCTL_MODULE_H