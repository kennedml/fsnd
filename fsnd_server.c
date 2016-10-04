#include "fsnd_server.h"

int fsnd_listen(char* file)
{

  // File Descriptors
  int listen_fd = 0;
  int conn_fd = 0;
  struct sockaddr_storage addr;
  socklen_t addr_size;
  char *buffer = (char*)calloc(BUFSIZ, sizeof(char*));

  listen_fd = socket_listen(fsnd_port);
  if (listen_fd < 0)
    printf("Failed to listen to port: %s\n", strerror(errno));


  FILE *fp;
  fp = fopen(file, "w");

  printf("file: %s\n", file);

  addr_size = sizeof(addr);
  conn_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_size);

  int remaining = 0;
  int received = 0;
  recv(conn_fd, buffer, BUFSIZ, 0);
  int file_size = atoi(buffer);
  printf("FILE SIZE: %d\n", file_size);

  remaining = file_size;

  while(((received = read(conn_fd, buffer, file_size)) > 0) && (remaining > 0))
  {
    printf("REC: %d\tREM: %d\t BUF: %zu\n", received, remaining, sizeof(buffer));
    fwrite(buffer, 1, received, fp);
    remaining -= received;
  }

  fclose(fp);
  close(conn_fd);

  close(listen_fd);

  free(buffer);
  return 0;
}
