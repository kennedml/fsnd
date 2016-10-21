#ifndef FSND_CLIENT_H
#define FSND_CLIENT_H
#include "fsnd_utilities.h"
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int fsnd_client(char* file, bool is_verbose);

#endif
