#include "socket.h"

int socket_resolve(const char *host, const char *port, struct addrinfo **hostinfo, bool is_verbose) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags    = AI_PASSIVE;

  return getaddrinfo(host, port, &hints, hostinfo);
}

int socket_dial(const char *host, const char *port, bool is_verbose) {
  struct addrinfo *hostinfo;
  int s; 

  if(socket_resolve(host, port, &hostinfo, is_verbose) < 0){
    printf("breaking here\n");
    return -1;
  }
  if((s = socket(hostinfo->ai_family, hostinfo->ai_socktype, hostinfo->ai_protocol)) < 0){
    printf("no breaking here\n");
    return -1;
  }
  if(connect(s, hostinfo->ai_addr, hostinfo->ai_addrlen) < 0){
    printf("nope its breaking here, %s\n", strerror(errno));
    return -1;
  }

  return s;
}

int socket_listen(const char *port, bool is_verbose) {
  struct addrinfo *hostinfo;
  int s;

  if(socket_resolve(NULL, port, &hostinfo,is_verbose) < 0)
    return -1;
  if((s = socket(hostinfo->ai_family, hostinfo->ai_socktype, hostinfo->ai_protocol)) < 0)
    return -1;
  if(bind(s, hostinfo->ai_addr, hostinfo->ai_addrlen) < 0 )
    return -1;
  if(listen(s, 10) < 0)
    return -1;
  return s;
}
