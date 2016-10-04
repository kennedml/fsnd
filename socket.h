/* socket.h: simple socket library
 * Written by Jeremy Winterberg for CS252 */

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

int socket_resolve(const char *host, const char *port, struct addrinfo **hostinfo, bool is_verbose);
int socket_dial(const char *host, const char *port, bool is_verbose);
int socket_listen(const char *port, bool is_verbose);

#endif
