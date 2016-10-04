#include "fsnd_server.h"

int fsnd_listen(char* file, bool is_verbose)
{


  // File Descriptors
  int listen_fd = 0;
  int conn_fd = 0;
  struct sockaddr_storage addr;
  socklen_t addr_size;
  char *buffer = (char*)calloc(BUFSIZ, sizeof(char*));
  
  if(verbose)
  {
      printf("Listening on port: %s \n", fsnd_port);
  }

  listen_fd = socket_listen(fsnd_port, is_verbose);
  if (listen_fd < 0)
    printf("Failed to listen to port: %s\n", strerror(errno));


  FILE *fp;
  fp = fopen(file, "w");
 
  if(verbose)
  {
      printf("Receiving file: %s\n", file);
  } 
  addr_size = sizeof(addr);
  conn_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_size);

  int remaining = 0;
  int received = 0;
  recv(conn_fd, buffer, BUFSIZ, 0);
  int file_size = atoi(buffer);

  remaining = file_size;
  int sum = 0;
  double progress = 0;
  while(((received = read(conn_fd, buffer, file_size)) > 0) && (remaining > 0))
  {
    sum += received;
    progress = (double)sum / (double)file_size;

    printf("\rprogress= %f", progress);
    fflush(stdout);

    fwrite(buffer, 1, received, fp);
    remaining -= received;
  }
  if (verbose)
  {
    printf("Receive: success\n");
    printf("File size received: %d\n", sum);
  } 
  fclose(fp);
  free(buffer);
  close(conn_fd);
  close(listen_fd);

  return 0;
}
