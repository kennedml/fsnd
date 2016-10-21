#include "fsnd_client.h"

int fsnd_client(char* file, bool is_verbose)
{
  int sockfd = 0;
  char *source;

  sockfd = socket_dial(fsnd_host, fsnd_port, is_verbose);
  if (sockfd < 0)
  {
    printf("\n Error : Could not create socket \n");
    return 1;
  }

  FILE *fp;
  fp = fopen(file, "rb");
  if(!fp)
  {
    printf("Error opening file");
    return 1;
  }

  struct stat sb;
  char file_size[1024] = "";

  if (fp != NULL) {
    /* Go to the end of the file. */
    if (fseek(fp, offset, SEEK_END) == 0) {
      /* Get the size of the file. */
      long bufsize = ftell(fp);
      if (bufsize == -1) { /* Error */ }

      // Get and Send File Size
      fstat(fileno(fp), &sb);

      sprintf(file_size, "%d", (int)sb.st_size);

      if(verbose){ printf("File size: %ld\n", bufsize);}

      if (bytes > 0L){
        sprintf(file_size, "%ld", bytes);
      }
      else{
        sprintf(file_size, "%d", (int)sb.st_size);
      }

      // send file size to server
      write(sockfd, file_size, strlen(file_size));

      /* Allocate our buffer to that size. */
      source = malloc(sizeof(char) * (bufsize));

      /* Go back to the start of the file. */
      if (fseek(fp, offset, SEEK_SET) != 0) { /* Error */ }

      /* Read the entire file into memory. */
      int newLen;
      //size_t newLen = fread(source, sizeof(char), bufsize, fp);
      while((newLen = fread(source, sizeof(char), strlen(file_size),fp)) > 0)
      {
        int total = 0;
        do{
          int written = write(sockfd, source, newLen);
          if (written == -1){
            printf("Failed to write: %s\n", strerror(errno));
            return 1;
          }
          total += written;
        }while(total < newLen);
      }

    }
  }
  free(source);
  fclose(fp);
  if(verbose){printf("Send Success!\n");} 
  close(sockfd);
  return 0;
}
