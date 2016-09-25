#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int fsnd_listen()
{
    // File Descriptors
    int listen_fd = 0;
    int conn_fd = 0;
    int n = 1024;
    struct sockaddr_in serv_addr;
    char *buffer = (char*)calloc(n, sizeof(char*));

    // TODO - add verbosity
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
    {
	perror("socket failure");
	exit(EXIT_FAILURE);
    }
   

    // TODO - add verbosity
    printf("Socket retrieve success\n");
    
    printf("fd: %d\n", listen_fd);
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&buffer, 0, sizeof(buffer));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    // TODO - add verbosity
    if(bind(listen_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
    {
	perror("bind error");
	exit(EXIT_FAILURE);
    }
    if(listen(listen_fd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }

    while(1)
    {
        conn_fd = accept(listen_fd, (struct sockaddr*)NULL ,NULL);

        /* Open the file that we wish to transfer */
        FILE *fp = fopen("sample_file.txt","rb");
        if(fp==NULL)
        {
            printf("File opern error");
            return 1;   
        }   

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            unsigned char buff[256]={0};
            int nread = fread(buff,1,256,fp);
            printf("Bytes read %d \n", nread);        

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
                write(conn_fd, buff, nread);
            }

            /*
             * There is something tricky going on with read .. 
             * Either there was error, or we reached end of file.
             */
            if (nread < 256)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }


        }

        close(conn_fd);
        sleep(1);
    }

    free(buffer);
    return 0;
}
