#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/mmap_device"
#define MAP_SIZE 4096

int main(void)
{
    int fd;
    char *mapped;
    const char *msg = "Hello from user space via mmap!";

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("open");
        fprintf(stderr, "Could not open %s. Load the driver first.\n", DEVICE_PATH);
        return EXIT_FAILURE;
    }

    mapped = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Mapping created at address: %p\n", (void *)mapped);
    memset(mapped, 0, MAP_SIZE);
    memcpy(mapped, msg, strlen(msg) + 1);

    printf("Kernel buffer contents: %s\n", mapped);

    if (munmap(mapped, MAP_SIZE) != 0) {
        perror("munmap");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return EXIT_SUCCESS;
}
