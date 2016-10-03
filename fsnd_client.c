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
    int n = 1024;
    char *buffer = (char*)calloc(n, sizeof(char*));

    sockfd = socket_dial(fsnd_host, fsnd_port);
    /* Create a socket first */
    printf("Socket File Descriptor: %d\n", sockfd);
    if (sockfd < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    /* if (stat(file, &sb) < 0){ */
    /*   printf("file provided is invalid.\n"); */
    /*   return 1; */
    /* } */

    FILE *fp;
    printf("FILE: %s\n", file);
    fp = fopen(file, "rb");
    if(!fp)
    {
        printf("Error opening file");
        return 1;
    }
    while(!feof(fp))
    {
      int read = fread(buffer, 1, sizeof(buffer), fp);
      /* if (read < 1){ */
      /*   printf("Failed to read from file\n"); */
      /*   fclose(fp); */
      /*   return 1; */
      /* } */

      do {
        bytes_sent = send(sockfd, &buffer[offset], read - offset, 0);
        if (bytes_sent < 1){
          printf("Failed to write to socket: %s\n", strerror(errno));
          fclose(fp);
          return 1;
        }
        offset += bytes_sent;
      } while(offset < read);
    }
    fclose(fp);

    free(buffer);
    return 0;
}
