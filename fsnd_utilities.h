#ifndef FSND_UTILITIES_H
#define FSND_UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "fsnd_client.h"
#include "fsnd_server.h"
#include "socket.h"
#include <ctype.h>

extern bool verbose;
extern bool listen_flag;
extern char* fsnd_port;
extern char* fsnd_host;
extern int bytes;
extern int offset;

typedef enum
{
    HELP='h',
    VERBOSE='v',
    PORT='p',
    BYTES='n',
    OFFSET='o',
    LISTEN='l'
} flags;

int parse_args(int argc, char **argv);
int print_help();
bool is_valid_file(char *path);
#endif
