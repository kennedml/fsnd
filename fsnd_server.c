#include "fsnd_server.h"
#include <fcntl.h>
#include <inttypes.h>

int fsnd_listen(char* file, bool is_verbose)
{
  // File Descriptors
  int listen_fd = 0;
  int conn_fd = 0;

  struct sockaddr_storage addr;
  socklen_t addr_size;
  /* char *buffer = (char*)calloc(BUFSIZ, sizeof(char*)); */

  char nonce[64] = "";
  char kb[64] = "";
  printf("Please enter nonce for b: ");
  scanf("%s", nonce);
  printf("Please enter private key for b: ");
  scanf("%s", kb);

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
    fd = open(file, O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
  } else {
    fd = 1;
  }

  char *buffer = (char*)calloc(BUFSIZ, sizeof(char*));
  addr_size = sizeof(addr);
  conn_fd = accept(listen_fd, (struct sockaddr*)&addr, &addr_size);

  int sum = 0;
  int received = 0;

  // Filesize for progress bar
  /* char BUF[BUFSIZ] = ""; */
  /* char *endptr; */
  /* read(conn_fd, BUF, sizeof(char*)); */
  /* printf("BUF %s\n", BUF); */
  /* intmax_t file_size = strtoimax(BUF, &endptr, 10); */

  if(verbose){printf("Receiving file: %s\n", file);} 

  while(1){
    /* double progress = 0; */
    while((received = read(conn_fd, buffer, 4096)) > 0)
    {
      sum += received;
      /* progress = (double)sum / (double)file_size; */

      int total = 0;
      while(total < received){
        int written = write(fd, buffer, received);
        if (written == -1){
          printf("Failed to write: %s\n", strerror(errno));
          return 1;
        }
        total += written;
      }

      if (file != NULL){
        /* fprintf(stdout, "\rprogress= %f | bytes received: %d", progress, sum); */
        fprintf(stdout, "\rBytes Received: %d", sum);
        fflush(stdout);
      }
    }

    if (received == 0){
      printf("\n");
      break;
    }
    if (received < 0){
      printf("Error Receiving Data: %s\n", strerror(errno));
      return 1;
    }
  }
  if (verbose){
    printf("Receive: success\n");
    printf("File size received: %d\n", sum);
  } 
  free(buffer);
  close(fd);
  close(conn_fd);
  close(listen_fd);

  return 0;
}
