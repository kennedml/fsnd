#include "fsnd_kdc.h"

int run_kdc()
{
    int rc = 0;
    // File Descriptors
    int listen_fd = 0;
    int conn_fd = 0;

    struct sockaddr_storage addr;
    socklen_t addr_size;
    char ks_response[64] = "";
    char ka_response[64] = "";
    char kb_response[64] = "";

    printf("Please enter Ks: ");
    scanf("%s", ks_response);
    printf("Please enter Ka: ");
    scanf("%s", ka_response);
    printf("Please enter Kb: ");
    scanf("%s", kb_response);
    
/* TODO: dynamic port */
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
    printf("nonce received: %lu\n", nonce_int); 
    

    /* char buffer[128] = ""; */
    char *buffer = (char*)malloc(256);
    printf("ks: %s\n", ks_response);
    char ida[64] = "thing1.cs.uwec.edu";

    sprintf(buffer, "%-64s%-64s", ks_response, ida);
    printf("Buffer: %s\n", buffer);

    Blowfish ks_ctx;
    // Encrypts with KB
    ks_ctx.Set_Passwd(kb_response);
    ks_ctx.Encrypt(buffer, 128);
    printf("encrypted buffer: %s\n", buffer);
    ks_ctx.Decrypt(buffer, 128); 
    printf("decrypted buffer: %s\n", buffer);

    free(buffer);


    return rc;
}

