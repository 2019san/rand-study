#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    unsigned int seed;
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, &seed, sizeof(seed));
    close(fd);

    srand(seed);

    int r = rand();
    printf("result : %d\n", r);

    return 0;
}