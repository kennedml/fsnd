#include "fsnd_server.h"

int fsnd_listen(char* file)
{
  // File Descriptors
  int listen_fd = 0;
  int conn_fd = 0;
  int n = 1024;
  struct sockaddr_storage addr;
  socklen_t addr_size;
  char *buffer = (char*)calloc(n, sizeof(char*));

  listen_fd = socket_listen(fsnd_port);
  if (listen_fd < 0)
    printf("Failed to listen to port: %s\n", strerror(errno));

  printf("Listening to port: %s, listen_fd: %d\n", fsnd_port, listen_fd);

  FILE *fp;
  if (file != NULL){
    fp = fopen(file, "wb");
  }

  while(1)
  {
    addr_size = sizeof(addr);
    conn_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_size);
    printf("connection established\n");

    int received = recv(conn_fd, buffer, sizeof(buffer), 0);

    do
    {
      int read = fwrite(&buffer[offset], 1, read - offset, fp);
      if (read < 1){
        printf("Failure to write to file: %s\n", strerror(errno));
        fclose(fp);
        return 1;
      }

      offset += received;
    } while (offset < received);

    close(conn_fd);
    printf("connection closed\n");
  }

  fclose(fp);

  free(buffer);
  return 0;
}
