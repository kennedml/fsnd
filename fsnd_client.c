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

  char prompt1_buffer[1024] = "";
  char prompt2_buffer[1024] = "";
  char prompt3_buffer[1024] = "";
  char prompt4_buffer[1024] = "";
  char ks_response[BUFSIZ] = "";
  char ka_response[BUFSIZ] = "";
  char kb_response[BUFSIZ] = "";
  char host_response[BUFSIZ] = "";
  
  // Send first parameter
  read(kdc_sockfd, prompt1_buffer, sizeof(prompt1_buffer));
  printf("%s", prompt1_buffer);
  scanf("%s", ks_response);
  write(kdc_sockfd, ks_response, strlen(ks_response));
 
  // Send second parameter
  read(kdc_sockfd, prompt2_buffer, sizeof(prompt2_buffer));
  printf("%s", prompt2_buffer);
  scanf("%s", ka_response);
  write(kdc_sockfd, ka_response, strlen(ka_response));
  

  // Send third parameter
  read(kdc_sockfd, prompt3_buffer, sizeof(prompt3_buffer));
  printf("%s", prompt3_buffer);
  scanf("%s", kb_response);
  write(kdc_sockfd, kb_response, strlen(kb_response));

  // Send fourth parameter
  read(kdc_sockfd, prompt4_buffer, sizeof(prompt4_buffer));
  printf("%s", prompt4_buffer);
  scanf("%s", host_response);
  write(kdc_sockfd, host_response, strlen(host_response));

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
  char file_size[1024] = "";

  int sum = 0;
  if (fp != NULL) {
    /* Go to the end of the file. */
    if (fseek(fp, offset, SEEK_END) == 0) {
      /* Get the size of the file. */
      long bufsize = ftell(fp);
      if (bufsize == -1) { /* Error */ }

      // Get and Send File Size
      fstat(fileno(fp), &sb);

      if(verbose){ printf("File size: %ld\n", bufsize);}

      if (bytes > 0){
        sprintf(file_size, "%ld", bytes);
      }
      else{
        sprintf(file_size, "%d", (int)sb.st_size);
      }

      // send file size to server
      write(server_sockfd, file_size, strlen(file_size));

      /* Allocate our buffer to that size. */
      source = malloc(sizeof(char) * (bufsize));

      /* Go back to the start of the file. */
      if (fseek(fp, offset, SEEK_SET) != 0) { /* Error */ }

      /* Read the entire file into memory. */
      int newLen;
      while((newLen = fread(source, sizeof(char), strlen(file_size),fp)) > 0)
      {
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
