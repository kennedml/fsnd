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
  fp = fopen(file, "wb");

  printf("file: %s\n", file);

  while(1)
  {
    addr_size = sizeof(addr);
    conn_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_size);

    int received = 0;
    while((received = recv(conn_fd, buffer, BUFSIZ, 0)) > 0)
    {
      printf("Inside received while\n");
      do
      {
        /* int read = fwrite(&buffer[offset], 256, read - offset, fp); */

        printf("BUFFER: %s\n", buffer);
        /* if (read < 1){ */
        /*   printf("Failure to write to file: %s\n", strerror(errno)); */
        /*   fclose(fp); */
        /*   return 1; */
        /* } */

        offset += received;
      } while (offset < received);
      received = read(conn_fd, buffer, BUFSIZ);
    }

    close(conn_fd);
  }

  close(listen_fd);
  fclose(fp);

  free(buffer);
  return 0;
}
