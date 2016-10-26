#include "fsnd_kdc.h"

int run_kdc()
{
    int rc = 0;
    long seed = 432455647892341;
    unsigned long int nonce = generate_nonce(seed);
    printf("nonce: %ld\n", nonce);
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
    while(1)
    {
    }


    return rc;
}

unsigned long int generate_nonce(long nonce) {
    const long A = 48271;
    const long M = 2147483647;
    const long Q = M/A;
    const long R = M%A;

	static long state = 1;
	long t = A * (state % Q) - R * (state / Q);
	
	if (t > 0)
		state = t;
	else
		state = t + M;
	return (long)(((double) state/M)* nonce);
}

