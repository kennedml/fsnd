#include "fsnd_server.h"
#include <fcntl.h>

int fsnd_listen(char* file, bool is_verbose)
{
  // File Descriptors
  int listen_fd = 0;
  int conn_fd = 0;

  struct sockaddr_storage addr;
  socklen_t addr_size;
  /* char *buffer = (char*)calloc(BUFSIZ, sizeof(char*)); */
  
  if(verbose){
    printf("Listening on port: %s \n", fsnd_port);
  }

  listen_fd = socket_listen(fsnd_port, is_verbose);

  if (listen_fd < 0){
    printf("Failed to listen to port: %s\n", strerror(errno));
    return(1);
  }

  /* FILE *fp; */
  int fd;
  if (file != NULL){
    fd = open(file, O_CREAT|O_WRONLY);
    /* fp = fopen(file, "w"); */
  } else {
    fd = 1;
  }


  char *buffer = (char*)calloc(BUFSIZ, sizeof(char*));
  addr_size = sizeof(addr);
  conn_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_size);

  int sum = 0;
  int received = 0;
  char BUF[BUFSIZ];
  read(conn_fd, BUF, sizeof(char*));
  int file_size = atoi(BUF);
  printf("File_size: %d\n", file_size);
  if(verbose){
    printf("Receiving file: %s\n", file);
  } 
  while(1){
    double progress = 0;
    while((received = read(conn_fd, buffer, 4096)) > 0)
    {
      sum += received;
      progress = (double)sum / (double)file_size;

      int written = write(fd, buffer, received);

      fprintf(stdout, "\rprogress= %f -- sum: %d -- received: %d", progress, sum, received);
      fflush(stdout);
    }

    if (received == 0){
      break;
    }
    if (received < 0){
      printf("Error Receiving Data: %s\n", strerror(errno));
      return 1;
    }


    if (verbose){
      printf("Receive: success\n");
      printf("File size received: %d\n", sum);
    } 
  }
  free(buffer);
  close(fd);
  close(conn_fd);
  close(listen_fd);


  return 0;
}
