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
    char ks_response[BUFSIZ] = "";
    char ka_response[BUFSIZ] = "";
    char kb_response[BUFSIZ] = "";
    char request[BUFSIZ] = "";

    //unsigned long int nonce;
    read(conn_fd, nonce, sizeof(long unsigned int)); 
    unsigned long int nonce_int = atol(nonce);
    
    // Send the first prompt to the user
    char prompt1[] = "Please enter Ks: ";
    write(conn_fd, prompt1, strlen(prompt1));
    // Get a response back from the user
    read(conn_fd, ks_response, sizeof(ks_response));

    // Send the second prompt to the user
    char prompt2[] = "Please enter Ka: ";
    write(conn_fd, prompt2, strlen(prompt2));
    // Get a response back from the user
    read(conn_fd, ka_response, sizeof(ka_response));
    
    // Send the second prompt to the user
    char prompt3[] = "Please enter Kb: ";
    write(conn_fd, prompt3, strlen(prompt3));
    // Get a response back from the user
    read(conn_fd, kb_response, sizeof(kb_response));
    
    char prompt4[] = "Please enter the host address: ";
    write(conn_fd, prompt4, strlen(prompt4));
    // Get a response back from the user
    read(conn_fd, request, sizeof(request));
    Blowfish ctx;
    /*
    BLOWFISH_CTX ka, kb; 

    unsigned long L = 1, R = 2;

    // Initialize blowfish on ctx a
    printf("kb: %s\n", kb_response);
    Blowfish_Init(&kb, (unsigned char*)kb_response, strlen(kb_response));

    Blowfish_Encrypt(&kb, &L, &R);
    printf("encrypted data: ");
    printf("0x%08lx 0x%08lx\n", L, R);

    Blowfish_Decrypt(&kb, &L, &R);
    printf("decrypted data: ");
    printf("L: %lu  R: %lu\n", L,R);
    */

    return rc;
}

