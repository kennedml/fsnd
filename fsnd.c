#include "fsnd.h"

#define DBG

int rc = 0;
bool is_verbose = false;
bool listen_flag = false;
int fsnd_port = -1;
int fsnd_bytes = -1;
int fsnd_offset = -1;
char * fsnd_url = "localhost";

int main(int argc, char **argv)
{
  int rc = 0;

  if((rc = parse_args(argc, argv)))
  {
    printf("%s %d %s print_help() failed. Errno: %d. \n",
        __FILE__,__LINE__,__func__, rc);
  }
  return(0);
}

int print_help()
{
  printf("fsend [OPTIONS] dest_ip file\n");
  printf("\t-h\t\tPrint this help screen\n");
  printf("\t-v\t\tVerbose output\n");
  printf("\t-p port\t\tSet the port to connect on (e.g., 9285)\n");
  printf("\t-n bytes\tNumber of bytes to send, defaults whole file\n");
  printf("\t-o offset\tOffset into file to start sending\n");
  printf("\t-l\t\tList (on server side) on port instead of connecting and\n");
  printf("\t\t\twrite output to file and dest_ip refers to which ip to bind to.\n");
  printf("\t\t\t(default: localhost)\n");
  return(0);
}

int parse_args(int argc, char **argv)
{
  int opt;

  while(optind < argc)
  {
    if ((opt = getopt(argc, argv, "hvp:n:o:l")) != -1)
    {
      switch(opt)
      {
        case HELP:
          if((rc = print_help()))
          {
            printf("%s %d %s print_help() failed. Errno: %d. \n",
                __FILE__,__LINE__,__func__, rc);
          }
          exit(0);
        case VERBOSE:
          is_verbose = true;
          break;
        case PORT:
          if(fsnd_port != '\0')
          {    
            fsnd_port = atoi(optarg);
          }
          break;
        case BYTES:
          fsnd_bytes = atoi(optarg);
          break;
        case OFFSET:
          fsnd_offset = atoi(optarg);
          break;
        case LISTEN:
          rc = fsnd_listen();
          exit(0);
      }
    }
    else
    {
      printf("should be server: %s", argv[optind]);
      fsnd_url = argv[optind];
      optind++;
    }
    fsnd_client();
  }
#ifdef DBG
  printf("verbose_flag: %s\n", is_verbose ? "true" : "false");
  printf("port: %d\n", fsnd_port);
  printf("bytes: %d\n", fsnd_bytes);
  printf("offset: %d\n", fsnd_offset);

#endif

  return(0);
}


int fsnd_listen()
{
  // File Descriptors
  int listen_fd = 0;
  int conn_fd = 0;
  int n = 1024;
  struct sockaddr_in serv_addr;
  char *buffer = (char*)calloc(n, sizeof(char*));

  char *p = (char*)calloc(100, sizeof(char*));
  snprintf(p, sizeof(&p), "%d", fsnd_port);
  if ((listen_fd = socket_listen(p)) != -1){
  }
  printf("Listening to port: %s, listen_fd: %d", p, listen_fd);
  free(p);

  while(1)
  {
    conn_fd = accept(listen_fd, (struct sockaddr*)'\0' ,'\0');

    /* Open the file that we wish to transfer */
    FILE *fp = fopen("sample_file.txt","rb");
    if(fp=='\0')
    {
      printf("File opern error");
      return 1;   
    }   

    /* Read data from file and send it */
    while(1)
    {
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

int fsnd_client()
{
  int sockfd = 0;
  int bytesReceived = 0;
  int n = 1024;
  char *recvBuff = (char*)calloc(n, sizeof(char*));
  memset(&recvBuff, 0, sizeof(recvBuff));
  struct sockaddr_in serv_addr;

  /* Create a socket first */
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
  {
    printf("\n Error : Could not create socket \n");
    return 1;
  }

  /* Initialize sockaddr_in data structure */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(fsnd_port); // port
  serv_addr.sin_addr.s_addr = inet_addr(fsnd_url);

  /* Attempt a connection */
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
  {
    printf("\n Error : Connect Failed \n");
    return 1;
  }

  /* Create file where data will be stored */
  FILE *fp;
  fp = fopen("sample_file.txt", "ab"); 
  if('\0' == fp)
  {
    printf("Error opening file");
    return 1;
  }

  /* Receive data in chunks of 256 bytes */
  while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
  {
    printf("Bytes received %d\n",bytesReceived);    
    // recvBuff[n] = 0;
    fwrite(recvBuff, 1,bytesReceived,fp);
    // printf("%s \n", recvBuff);
  }

  if(bytesReceived < 0)
  {
    printf("\n Read Error \n");
  }

  free(recvBuff);
  return 0;
}

