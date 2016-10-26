#include "fsnd_kdc.h"

int run_kdc()
{
    int rc = 0;
    // File Descriptors
    int listen_fd = 0;
    int conn_fd = 0;

    struct sockaddr_storage addr;
    socklen_t addr_size;
    
    listen_fd = socket_listen("9285", true);

    if(listen_fd < 0)
    {
        printf("Failed to listen to port: %d\n", fsnd_port);
        return errno;
    }
    printf("listen_fd: %d\n", listen_fd);
    
    conn_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_size);
    char nonce[sizeof(unsigned long int)];
    read(conn_fd, nonce, sizeof(nonce)); 
    printf("nonce: %s\n", nonce);
    while(1)
    {
    }


    return rc;
}

