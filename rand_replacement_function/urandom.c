#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{

    unsigned char buf[32];
    int fd = open("/dev/urandom", O_RDONLY);
    if(fd < 0){
        perror("open");
        return 1;
    }

    if(read(fd, buf, sizeof(buf))!= sizeof(buf)){
        perror("read");
        close(fd);
        return 1;
    }
    close(fd);
    
    printf("result(HEX) : ");
    for(int i = 0; i< 32; i++){
        printf("%02X", buf[i]);
    }
    printf("\n");
    return 0;
}