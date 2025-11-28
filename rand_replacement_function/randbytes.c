#include <stdio.h>
#include <openssl/rand.h>

int main(void){
    unsigned char buf[32];

    if(RAND_bytes(buf, sizeof(buf))!=1){
        fprintf(stderr, "RAND_bytes failed\n");
        return 1;
    }

    for(int i = 0; i<32; i++)
        printf("%02x", buf[i]);
    printf("\n");
    
    return 0;
}