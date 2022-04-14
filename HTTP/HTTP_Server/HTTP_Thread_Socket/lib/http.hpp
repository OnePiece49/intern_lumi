#ifndef __HTTP_
#define __HTTP_

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "registerAPI.hpp"
using namespace std;

#define LINE_HEADER                \
  "HTTP/1.1 200 OK\r\n"            \
  "Content-Type: text/plain\r\n"   \

#define CONTENT_LENGTH             \
  "Content-Length: "               \


typedef struct {
  int fd_server;
  int fd_data;
  vector<s_api *> MyApi;
}s_fdserver;
        
class Http {
    private: 
        static void *ThreadHandleRespond(void *fddata);

    public:
        static int AcceptConnect(s_fdserver *fd);
        static void InitHttp(s_fdserver *fd, int port);
        static void GetHttpRequest(s_fdserver *fd);
        static void SendHttpRespond(s_fdserver *fd);   
};
#endif