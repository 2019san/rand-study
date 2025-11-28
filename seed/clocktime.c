#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand(ts.tv_nsec ^ ts.tv_sec);

    int r = rand();
    printf("result : %d\n", r);

    return 0;
}