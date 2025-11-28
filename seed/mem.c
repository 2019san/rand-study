#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    int x;
    srand((unsigned long)&x);

    int r = rand();

    printf("result : %d\n", r);

    return 0;
}