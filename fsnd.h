#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#ifndef FSND_H
#define FSND_H

int parse_args(int argc,char **argv);
int print_help();
int fsnd_listen();
int print_args();
int fsnd_client();

typedef enum
{
    HELP='h',
    VERBOSE='v',
    PORT='p',
    BYTES='n',
    OFFSET='o',
    LISTEN='l'
} flags;

#endif // FSND_H
