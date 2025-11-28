#include <sodium.h>
#include <stdio.h>

int main(void){
    if(sodium_init() < 0){
        printf("sodium_init failed\n");
        return 1;
    }

    unsigned char buf[16];
    randombytes_buf(buf, sizeof(buf));

    printf("result(HEX) : ");
    for(int i=0; i<16; i++){
        printf("%02X", buf[i]);
    }
    printf("\n");

    return 0;
}