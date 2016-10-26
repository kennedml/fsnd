#include "fsnd_utilities.h"
#include <sys/stat.h>

bool verbose = false;
bool listen_flag = false;
bool kdc_flag = false;
char* default_port = "9285";
char* default_host = "localhost";
char *fsnd_port;
char *fsnd_host;
char *file_name;
int bytes = 0;
int offset = 0;

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
  int rc = 0;
  bool has_port_flag = false;


  while((opt = getopt(argc, argv, "hkvp:n:o:l")) != -1)
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
      case KDC:
        kdc_flag = true;
        break;
      case VERBOSE:
        verbose = true;
        break;
      case PORT:
        fsnd_port = optarg;
        has_port_flag = true;
        break;
      case BYTES:
        bytes = atoi(optarg);
        printf("BYTES: %d\n", bytes);
        break;
      case OFFSET:
        offset = atoi(optarg);
        printf("OFFSET: %d\n", offset);
        break;
      case LISTEN:
        listen_flag = true;
        break;
    }
  }

  int n_non_flagged_opts = argc - optind;

  if(kdc_flag)
  {
        rc = run_kdc();
        return(0);
  }

  if(listen_flag)
  {
    if (n_non_flagged_opts == 1){
      file_name = argv[optind];
    } else {
      file_name = NULL;
    }
    if (fsnd_port == NULL)
      fsnd_port = default_port;
    if (fsnd_host == NULL)
      fsnd_host = default_host;
  }
  /* else */
  /* { */  
    if(n_non_flagged_opts == 0)
    {
      /* This is for STDIN piping, didn't quite finish */
      /* Will finish later this week */

      /* char stdin_buffer[BUFSIZ]; */
      /* int n; */
      /* scanf("%d", n); */
      /* printf("n: %d\n", n); */

      /* int count = fread(stdin_buffer, BUFSIZ, 1, stdin); */
      /* printf("str: %s\n", stdin_buffer); */
      /* printf("read %d bytes\n", count); */
      if (listen_flag == false){
        printf("Error: Include file name\n");
        return(EXIT_FAILURE);
      }
    }
    else if(n_non_flagged_opts == 1)
    {      
      if(is_valid_file(argv[optind]))
      {
        fsnd_host = default_host;
        fsnd_port = default_port;
        file_name = argv[optind];
      }
      else
      {
        printf("invalid file\n");
        return(EXIT_FAILURE);
      }
    }
    else if(n_non_flagged_opts == 2)
    {
      if(has_port_flag)
      {
        if(is_valid_file(argv[optind+1]))
        {
          fsnd_host = argv[optind];
          //fsnd_port = default_port;
          file_name = argv[optind+1];
        }
        else
        {
          printf("invalid file\n");
          return(EXIT_FAILURE);
        }
      }
      else
      {
        printf("Not enough arguments. Check to be sure you pass port with -p flag\n");
        return(EXIT_FAILURE);
      }
    }
    else if(n_non_flagged_opts == 3)
    {
      if(has_port_flag)
      {
        printf("Too many arguments! Try removing the -p flag!\n");
        return(EXIT_FAILURE);
      }
      else
      {
        printf("port: %s\n", fsnd_port);
        if(is_valid_file(argv[optind+2]))
        {
          fsnd_host = argv[optind];
          fsnd_port = argv[optind+1];
          file_name = argv[optind+2];
        }
        else
        {
          printf("invalid file\n");
          return(EXIT_FAILURE);
        }
      }
    }
    else
    {
      printf("Too many arguments entered\n");
      return(EXIT_FAILURE);
    }
    if(verbose)
    {
      printf("Connecting to IP: %s\n", fsnd_host);
      printf("On port: %s\n", fsnd_port);
      printf("File name: %s\n", file_name);
    }
    if (listen_flag)
      rc = fsnd_listen(file_name, verbose);
    else
      rc = fsnd_client(file_name, verbose);
  /*elseclosing } */

  return(0);
}

bool is_valid_file(char *path)
{
  struct stat sb;
  stat(path, &sb);
  if(S_ISREG(sb.st_mode))
  {
    return true;
  }
  else if (listen_flag)
  {
    return true;
  }
  else
  {
    return false;
  }
}

unsigned long int generate_nonce(long nonce) {
    const long A = 48271;
    const long M = 2147483647;
    const long Q = M/A;
    const long R = M%A;

	static long state = 1;
	long t = A * (state % Q) - R * (state / Q);
	
	if (t > 0)
		state = t;
	else
		state = t + M;
	return (long)(((double) state/M)* nonce);
}


