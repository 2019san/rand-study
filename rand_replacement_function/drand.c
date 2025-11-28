#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand48(time(NULL));

    double x = drand48();

    

    printf("result : %lf\n", x);

    return 0;
}