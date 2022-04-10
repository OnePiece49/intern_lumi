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

#define PORT_NUMBER 7000
#define BUF_SIZE 500

#define RESPONSE                  \
  "HTTP/1.1 200 OK\r\n"           \
  "Content-Type: text/plain\r\n"  \
  "Content-Length: 11\r\n"        \
  "\r\n"                          \
  "vietpro vip\r\n"

#define LINE_HEADER                \
  "HTTP/1.1 200 OK\r\n"            \
  "Content-Type: text/plain\r\n"   \

#define CONTENT_LENGTH             \
  "Content-Length: "               \

        
class Http {
    private:
        int fd_server;
        void accept_connect(); 
        static void dong_goi_ban_tin(char *respond, char *body, char *content_length);         
        static void ban_tin(char *respond);
        static void *thread_handle_respond(void *fddata);
    public:
        Http(int fd_server);
        Http(){};
        void init_http();
};

#endif