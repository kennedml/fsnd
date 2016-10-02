#include "fsnd_server.h"
#include <errno.h>

int fsnd_listen()
{
  // File Descriptors
  int listen_fd = 0;
  int conn_fd = 0;
  int n = 1024;
  struct sockaddr_storage addr;
  socklen_t addr_size;
  char *buffer = (char*)calloc(n, sizeof(char*));

  if ((listen_fd = socket_listen(fsnd_port)) < 0)
    printf("Failed to listen to port\n");

  printf("Listening to port: %s, listen_fd: %d\n", fsnd_port, listen_fd);

  while(1) {
    printf("inside top while");
    addr_size = sizeof(addr);
    conn_fd = accept(listen_fd, (struct sockaddr*)'\0', '\0');
    printf("Errno %s", strerror(errno));

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
      printf("inside second while");
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
