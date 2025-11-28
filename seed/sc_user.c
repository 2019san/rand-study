#include <stdio.h>
#include <stdlib.h>

int main()
{

    unsigned int seed;

    printf("input seed : ");
    scanf("%u", &seed);
    srand(seed);
    int r = rand();

    printf("result : %d\n", r);

    return 0;
}