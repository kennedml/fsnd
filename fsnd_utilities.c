#include "fsnd_utilities.h"
#include <sys/stat.h>

bool verbose = false;
bool listen_flag = false;
char* default_port = "9285";
char* default_host = "localhost";
char *fsnd_port;
char *fsnd_host;
char *file_name;
long bytes = 0L;
long offset = 0L;

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


  while((opt = getopt(argc, argv, "hvp:n:o:l")) != -1)
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
        verbose = true;
        break;
      case PORT:
        fsnd_port = optarg;
        has_port_flag = true;
        break;
      case BYTES:
        bytes = atoi(optarg);
        break;
      case OFFSET:
        offset = atoi(optarg);
        break;
      case LISTEN:
        listen_flag = true;
        break;
    }
  }
  
  int n_non_flagged_opts = argc - optind;

  if(listen_flag)
  {
    if (n_non_flagged_opts == 1){
      file_name = argv[optind];
      fsnd_port = default_port;
      fsnd_host = default_host;
      rc = fsnd_listen(file_name, verbose);
    }
  }
  else
  {  
    if(n_non_flagged_opts == 0)
    {
        printf("Error: Include file name\n");
        return(EXIT_FAILURE);
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
    rc = fsnd_client(file_name, verbose);
  }

  //printf("host: %s\n", fsnd_host);
  //printf("port: %s\n", fsnd_port);
  //printf("file name: %s\n", file_name);
  //printf("offset: %d\n", offset);
  //printf("bytes: %d\n", bytes);

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
    else
    {
        return false;
    }
}

