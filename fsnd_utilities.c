#include "fsnd_utilities.h"

int print_help()
{
    printf("fsend [OPTIONS] dest_ip file\n");
    printf("\t-h\t\tPrint this help screen\n");
    printf("\t-v\t\tVerbose output\n");
    printf("\t-p port\t\tSet the port to connect on (e.g., 9285\n");
    printf("\t-n bytes\tNumber of bytes to send, defaults whole file\n");
    printf("\t-o offset\tOffset into file to start sending\n");
    printf("\t-l\t\tList (on server side) on port instead of connecting and\n");
    printf("\t\t\twrite output to file and dest_ip refers to which ip to bind to.\n");
    printf("\t\t\t(default: localhost)\n");
    return(0);
}

int parse_args(int argc, char **argv)
{
    bool verbose;
    verbose = false;
    int opt;
    int rc = 0;

    while((opt = getopt(argc, argv, "hvpnol")) != -1)
    {
        switch(opt)
        {
            case 'h':
                if((rc = print_help()))
                {
                    printf("%s %d %s print_help() failed. Errno: %d. \n",
                        __FILE__,__LINE__,__func__, rc);
                }
                break;
            case 'v':
                verbose = true;
		break;
            case 'p':
                printf("p\n");
                break;
            case 'n':
                printf("n\n");
                break;
            case 'o':
                printf("o\n");
                break;
            case 'l':
                printf("l\n");
                break;
        }
    }
    return(0);
}
