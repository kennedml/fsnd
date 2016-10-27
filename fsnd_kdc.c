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
    
    conn_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_size);
    char nonce[64] = "";
    //unsigned long int nonce;
    read(conn_fd, nonce, sizeof(long unsigned int)); 
    unsigned long int nonce_int = atol(nonce);
    printf("Request for session key sent with nonce: %lu\n", nonce_int); 
    

    /* char buffer[128] = ""; */
    char *kb_buffer = (char*)malloc(128);
    printf("Session Key Recv'd: %s\n", ks_response);
    char ida[64] = "thing1.cs.uwec.edu";

    sprintf(kb_buffer, "%-64s%-64s", ks_response, ida);
    //printf("Buffer: %s\n", kb_buffer);

    Blowfish kb_ctx;
    // Encrypts with KB
    kb_ctx.Set_Passwd(kb_response);
    kb_ctx.Encrypt(kb_buffer, 128);
    /* printf("encrypted buffer: %s\n", kb_buffer); */
    /* kb_ctx.Decrypt(buffer, 128); */ 
    /* printf("decrypted buffer: %s\n", kb_buffer); */

   char *ka_buffer = (char*)malloc(320);

    char request[64] = "thing2.cs.uwec.edu";
    sprintf(ka_buffer, "%-64s%-64s%-64lu%128s", ks_response, request, nonce_int, kb_buffer);

    printf("Buffer encrypted with Kb: %s\n", ka_buffer);

    Blowfish ka_ctx;
    ka_ctx.Set_Passwd(ka_response);
    ka_ctx.Encrypt(ka_buffer, 320);
    //printf("encrypted buffer: %s\n", ka_buffer);
    //printf("\n\n");
    //ka_ctx.Decrypt(ka_buffer, 320); 
    //printf("decrypted buffer: %s\n", ka_buffer);
    write(conn_fd, ka_buffer, 320);

    printf("Key distribution complete. Closing KDC.\n");

    free(kb_buffer);
    free(ka_buffer);


    return rc;
}

