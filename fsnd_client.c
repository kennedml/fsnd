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
    char *buffer = (char*)calloc(BUFSIZ, sizeof(char*));

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

    while(!feof(fp))
    {
      int read = fread(buffer, 256, sizeof(buffer), fp);
      printf("%s\n", buffer);
      do {
        bytes_sent = send(sockfd, &buffer, strlen(buffer), 0);
        if (bytes_sent < 1){
          printf("Didn't write to socket: %s\n", strerror(errno));
          fclose(fp);
          return 1;
        }
        offset += bytes_sent;
        printf("Wrote to socket: %d\n", bytes_sent);
      } while(offset < read);
    }
    fclose(fp);

    free(buffer);
    return 0;
}
