#include "fsnd_client.h"

int fsnd_client(char* file, bool is_verbose)
{
  int server_sockfd = 0;
  int kdc_sockfd;
  char *source;

  // Change these to not be hard-coded
  //const char *kdc_host = "45.55.58.196";
  const char *kdc_host = "localhost";
  const char *kdc_port = "9286";

  printf("Dialing kdc host\n");

  kdc_sockfd = socket_dial(kdc_host, kdc_port, is_verbose);

  printf("kdc_sockfd: %d\n", kdc_sockfd);

  // Create nonce to send to KDC
  char nonce_str[1024] = "";
  long seed = 5647892341;
  unsigned long int nonce = generate_nonce(seed);
  sprintf(nonce_str, "%lu", nonce);
  // send nonce to KDC
  printf("Sending nonce %s to KDC\n", nonce_str);
  write(kdc_sockfd, nonce_str, strlen(nonce_str));

  printf("Dialing server host\n");
  server_sockfd = socket_dial(fsnd_host, fsnd_port, is_verbose);
  
  printf("server_sockfd: %d\n", server_sockfd);

  if (server_sockfd < 0)
  {
    printf("\n Error : Could not create socket connection with server \n");
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
  int file_size = 0;

  int sum = 0;
  if (fp != NULL) {
    /* Go to the end of the file. */
    if (fseek(fp, offset, SEEK_END) == 0) {
      /* Get the size of the file. */
      // Get and Send File Size
      fstat(fileno(fp), &sb);

      /* if(verbose){ printf("File size: %ld\n", bufsize);} */

      if (bytes > 0){
        file_size = bytes;
        printf("CLIENT BYTES: %d FILESIZE: %d\n", bytes, file_size);
      }
      else{
        /* sprintf(file_size, "%ju", (intmax_t)sb.st_size); */
        file_size = (int)sb.st_size;
        /* printf("filesize: %s, size: %ju, intmax_t: %ld\n", file_size, (intmax_t)sb.st_size, strlen(file_size)); */
        /* printf("sizeof: %ld\n", sizeof(file_size)); */
        /* printf("int: %d, intmax_t: %ju, long: %lu\n", (int)sb.st_size,(intmax_t)sb.st_size,(long)sb.st_size); */
      }
      printf("CLIENT BYTES: %d\n", bytes);
      printf("CLIENT OFFSET: %d\n", offset);

      // send file size to server
      /* write(server_sockfd, file_size, strlen(file_size)); */

      /* Allocate our buffer to that size. */
      source = (char*)calloc(file_size, sizeof(char));

      /* Go back to the start of the file. */
      if (fseek(fp, offset, SEEK_SET) != 0) { /* Error */ }

      /* Read the entire file into memory. */
      int newLen;
      if((newLen = fread(source, sizeof(char), file_size,fp)) > 0)
      {
        printf("newLen: %d\n", newLen);
        int total = 0;
        do{
          int written = write(server_sockfd, source, newLen);
          if (written == -1){
            printf("Failed to write: %s\n", strerror(errno));
            return 1;
          }
          total += written;
          sum += total;
        }while(total < newLen);
      }

    }
  }
  free(source);
  fclose(fp);
  close(server_sockfd);
  close(kdc_sockfd);
  if(verbose){printf("Sent %d Bytes Successfully!\n", sum);} 
  return 0;
}
