#include "fsnd_server.h"
#include <fcntl.h>
#include <inttypes.h>
#include <string>

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

  char receive_enc[128] = "";
  read(conn_fd, receive_enc, 128);
  printf("\nEncrypted receive buffer: %s\n", receive_enc);
  
  Blowfish ctx_b;
  ctx_b.Set_Passwd(kb);
  ctx_b.Decrypt(receive_enc, 128);
  printf("\nDecrypted receive buffer: %s\n", receive_enc);

  char *id_a = (char*)malloc(64);
  char *session_key = (char*)malloc(64);
  //sprintf(id_a, "%-64s", &receive_enc[64]);
  //sprintf(session_key, "%-64s", &receive_enc[0]);
 
  memcpy(session_key, receive_enc, 64);
  memcpy(id_a, receive_enc + 64, 64);

  //memmove(session_key, receive_enc, 64);
  //session_key[64] = '\0';
  //memcpy(id_a, receive_enc + 64, 64);
  //id_a[64] = '\0';
  printf("id_a: %s\n", id_a);
  
 
  // null terminates session_key at end of key for blowfish password
  for (int i = 0; i < 64; i++){
    if(session_key[i] == ' '){
      session_key[i] = '\0';
      break;
    }
  }
  printf("Session key: %s\n", session_key);

  // Send nonce b over socket encrypted with session key
  Blowfish ctx_session;
  ctx_session.Set_Passwd(session_key);
  ctx_session.Encrypt(nonce, 64);
  printf("Encrypted nonce: %s\n", nonce);
  //ctx_session.Decrypt(nonce, 64);
  //printf("De(nonce: %s\n", nonce);

  printf("Writing encrypted nonce to socket\n");
  write(conn_fd, nonce, 64);

  //TODO: This should decrypt original and IDA nonce for verification
  //Currently not working

  //read nonce back from client
  /* char *f_anonce = (char*)malloc(64); */
  /* read(conn_fd, f_anonce, 64); */
  /* ctx_session.Decrypt(f_anonce, 64); */


  /* ctx_session.Decrypt(nonce, 64); */

  /* // null terminates nonce at end of key for function */
  /* for (int i = 0; i < 64; i++){ */
  /*   if(nonce[i] == ' '){ */
  /*     nonce[i] = '\0'; */
  /*     break; */
  /*   } */
  /* } */
  /* for (int i = 0; i < 64; i++){ */
  /*   if(f_anonce[i] == ' '){ */
  /*     f_anonce[i] = '\0'; */
  /*     break; */
  /*   } */
  /* } */
  /* unsigned long int f_bnonce = generate_nonce(atol(f_anonce)); */
  /* unsigned long int orig_fnonce = generate_nonce(atol(nonce)); */
  /* printf("function nonce: %lu\n", f_bnonce); */
  /* printf("function nonce: %lu\n", orig_fnonce); */
  /* printf("check: %d\n", f_bnonce == orig_fnonce); */

  char *hex_str = (char*)malloc(64);
  printf("Waiting for hex representation of test string\n");
  read(conn_fd, hex_str, 64);

  ctx_session.Decrypt(hex_str, strlen(hex_str));
  printf("Decrypted hex representation of test string: %s\n", hex_str);

  char buff = 0;
  char *hex_to_str = (char*)calloc(1024, 1);
  for(int i = 0; i < (int)strlen(hex_str); i++){
    if (i % 2 != 0){
      sprintf(hex_to_str, "%s%c", hex_to_str, hex_to_ascii(buff, hex_str[i]));
    } else{
      buff = hex_str[i];
    }
  }
  hex_to_str[strlen(hex_to_str)] = '\0';
  printf("Decrypted test string: %s\n", hex_to_str);

  free(hex_to_str);


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
  free(session_key);
  free(id_a);
  close(fd);
  close(conn_fd);
  close(listen_fd);

  return 0;
}
