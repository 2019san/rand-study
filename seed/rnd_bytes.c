#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>

int main()
{

    unsigned char buf[32]; // 256 bit(32byte)

    if(RAND_bytes(buf, sizeof(buf)) != 1){
        fprintf(stderr, "RAND_bytes() error\n");
        return 1;
    }
    printf("randnm(HEX) : ");
    for(int i = 0; i<sizeof(buf); i++)
    {
        printf("%02X", buf[i]);
    }
    
    printf("\n");

    return 0;
}