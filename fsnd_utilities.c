#include "fsnd_utilities.h"
#include "fsnd_server.h"
#include "fsnd_client.h"

bool verbose = false;
bool listen_flag = false;
int port = -1;
int bytes = -1;
int offset = -1;


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

    while((opt = getopt(argc, argv, "hvpnol")) != -1)
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
                port = atoi(optarg);
		break;
            case BYTES:
                bytes = atoi(optarg);
		break;
            case OFFSET:
                offset = atoi(optarg);
		break;
            case LISTEN:
		rc = fsnd_listen();
		break;
        }
    }
    return(0);
}
