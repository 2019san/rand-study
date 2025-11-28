#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    srand(getpid());

    int r = rand();

    printf("result : %d\n", r);

    return 0;
}