#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{

    srand(time(NULL) ^ clock());

    int r = rand();

    printf("result : %d\n", r);

    return 0;
}