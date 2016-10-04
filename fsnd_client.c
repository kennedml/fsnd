#include "fsnd_client.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int fsnd_client(char* file)
{
    int sockfd = 0;
    int bytes_sent = 0;
    char *source;

    sockfd = socket_dial(fsnd_host, fsnd_port);
    if (sockfd < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    FILE *fp;
    printf("FILE: %s\n", file);
    fp = fopen(file, "rb");
    if(!fp)
    {
        printf("Error opening file");
        return 1;
    }

    struct stat sb;
    char file_size[512];
    
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }

            // Get and Send File Size
            fstat(fileno(fp), &sb);

            sprintf(file_size, "%d", (int)sb.st_size);
            printf("BUFSIZ: %ld\n", bufsize); 
            send(sockfd, file_size, sizeof(file_size), 0);

            /* Allocate our buffer to that size. */
            source = malloc(sizeof(char) * (bufsize + 1));

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            /* Read the entire file into memory. */
            size_t newLen;
            int sum = 0;
            //size_t newLen = fread(source, sizeof(char), bufsize, fp);
            while((newLen = fread(source, sizeof(char), sizeof(bufsize),fp)) > 0)
            {
                sum += newLen;
                printf("newLen: %zu\ttotal len: %d\n", newLen, sum);
                /* if ( ferror( fp ) != 0 ) { */
                /* fputs("Error reading file", stderr); */
                /* } else { */
                /*     source[newLen++] = '\0'; /1* Just to be safe. *1/ */
                /* } */
                write(sockfd, source, newLen);
            }
            printf("BUFSIZ: %ld\n", bufsize); 
            printf("sizeof(bufsiz) %zu\n", sizeof(bufsize));
            /* write(sockfd, source, bufsize-sum); */ 
        }


    fclose(fp);
    }
    
    //printf("%s\n", source);
    //write(sockfd, source, atoi(file_size)+1);
    return 0;
}
