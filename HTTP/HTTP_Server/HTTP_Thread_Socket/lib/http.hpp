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

using namespace std;

#define LINE_HEADER                \
  "HTTP/1.1 200 OK\r\n"            \
  "Content-Type: text/plain\r\n"   \

#define CONTENT_LENGTH             \
  "Content-Length: "               \

        
class Http {
    private: 
        static void *ThreadHandleRespond(void *fddata);

    public:
        static int AcceptConnect(int *fd_server);
        static void InitHttp(int *fd_server, int Port);
        static void GetHttpRequest(int *fd);
        static void SendHttpRespond(int *fd);   
};
#endif