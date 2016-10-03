/* socket.h: simple socket library
 * Written by Jeremy Winterberg for CS252 */

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

int socket_resolve(const char *host, const char *port, struct addrinfo **hostinfo);
int socket_dial(const char *host, const char *port);
int socket_listen(const char *port);

#endif
