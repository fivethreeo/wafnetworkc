#ifndef NETWORKC_H
#define NETWORKC_H

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
#ifdef OS_WIN
  
#include <winsock2.h>
#include <Ws2tcpip.h>
  
#else
  
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#endif

#define PORT "9034"   // port we're listening on

#endif