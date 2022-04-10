#ifndef _HTTP_H
#define _HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <task.h>
#include <uv.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

#define RESPONSE                  \
  "HTTP/1.1 200 OK\r\n"           \
  "Content-Type: text/plain\r\n"  \
  "Content-Length: 11\r\n"        \
  "\r\n"                          \
  "vietpro vip\n"

class http_server {
      typedef struct {
        uv_tcp_t handle;
      } client_t;

      typedef struct {
        uv_write_t req;
        uv_buf_t buf;
      } write_req_t; 

      static void shutdown_cb(uv_shutdown_t *shutdown_req, int status);
      static void echo_write(uv_write_t* req, int status);
      static void echo_read(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
      static void on_new_connection(uv_stream_t* server, int status);
      static void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
      static void free_write_req(uv_write_t* req);
      static void close_cb(uv_handle_t *handle);
      static void func();
    
    private:
        uv_loop_t* loop;
        struct sockaddr_in addr;
        uv_tcp_t *server;
    
    public:
        http_server(uv_loop_t *loop, struct sockaddr_in addr, uv_tcp_t *server);
        void init_http();
};

#endif