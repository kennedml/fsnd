#include "fsnd_kdc.h"

int run_kdc()
{
    int rc = 0;
    // File Descriptors
    int listen_fd = 0;
    int conn_fd = 0;

    struct sockaddr_storage addr;
    socklen_t addr_size;
    
    listen_fd = socket_listen("9286", true);

    if(listen_fd < 0)
    {
        printf("Failed to listen to port: %s\n", fsnd_port);
        return errno;
    }
    printf("listen_fd: %d\n", listen_fd);
    
    conn_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_size);
    char nonce[1024] = "";
    //unsigned long int nonce;
    read(conn_fd, nonce, sizeof(long unsigned int)); 
    unsigned long int nonce_int = atol(nonce);
    printf("nonce: %lu\n", nonce_int);

    while(1)
    {
    }


    return rc;
}

