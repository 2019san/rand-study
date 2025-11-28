#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{

    srand(clock());

    int r = rand();

    printf("result : %d\n", r);

    return 0;
}