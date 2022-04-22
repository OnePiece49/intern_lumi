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
//#include "registerAPI.hpp"
using namespace std;

#define LINE_HEADER                \
  "HTTP/1.1 200 OK\r\n"            \
  "Content-Type: text/plain\r\n"   \

#define CONTENT_LENGTH             \
  "Content-Length: "               \



typedef void (*MyFunc )(void *, string *);
typedef enum  {
  GET = 0,
  POST = 1,
  PUT = 2,
  PATCH = 3,
  DELETE = 4,
}methodHttp;

typedef enum  {
  Content_Type = 0,
  Content_Length = 1,
  content_encoding = 2,
  set_cookie = 3,
  Connection = 4,
}e_header;


typedef struct {
  int method;
  string url;
  MyFunc func;
  void *parameter;
}s_api;

typedef struct {
  int fd_server;
  vector<s_api> MyApi;
}s_fdserver;

typedef struct {
  int fd_data;
  int *number_thread;
  string httprespond;
  vector<s_api> MyApi;
}s_fdclient;


void SetHeaderHttpRespond(e_header key, string value, string *Respond);
void SetBodyHttpRespond(string body, string *HttpRespond);

class HttpServer {
  private: 
    #define MAXSIZE 65536
    
    s_fdserver Server;
    int number_api = 0;

    static void CheckApi(methodHttp method, string url,s_fdclient *fd); 
    static void *ThreadHandleRespond(void *fddata);
    static void  GetHttpRequest(s_fdclient *fd);
    void  AcceptConnect();
    
  public:
    void InitHttp(int port);
    void RegisterApi(methodHttp method, string url, void (*callback)(void *, string *), void *para);
    static void SendHttpRespond(int *fd);
};

#endif