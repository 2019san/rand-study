#include <stdio.h>
#include <sys/random.h>

int main()
{

    unsigned char buf[32];

    ssize_t ret = getrandom(buf, sizeof(buf), 0);
    
    if(ret<0){
        perror("getrandom");
        return 1;
    }

    printf("result(HEX) : ");
    for(int i = 0; i< sizeof(buf); i++){
        printf("%02X", buf[i]);
    }

    printf("\n");

    return 0;
}