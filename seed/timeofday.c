#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

    int r = rand();
    printf("result : %d\n", r);

    return 0;
}