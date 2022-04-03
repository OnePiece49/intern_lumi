#include <stdlib.h>
#include <string.h>
#include <task.h>
#include <uv.h>
#include "http_parser.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
uv_thread_t hare_id;
uv_tcp_t server;

#define RESPONSE                  \
  "HTTP/1.1 200 OK\r\n"           \
  "Content-Type: text/plain\r\n"  \
  "Content-Length: 12\r\n"        \
  "\r\n"                          \
  "vietpro vip\n"
void echo_write(uv_write_t* req, int status);
uv_loop_t* loop;
struct sockaddr_in addr;

typedef struct {
  uv_tcp_t handle;
  http_parser parser;
} client_t;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

static http_parser_settings parser_settings;
void *thread_handle_http_client_connect(uv_stream_t* handle);
int headers_complete_cb(http_parser* parser) {
  printf("Server header_complete_cb\n");
  client_t *client = (client_t *) parser->data;
  uv_write_t *write_req = (uv_write_t *)malloc(sizeof(uv_write_t));
  uv_buf_t buf = uv_buf_init(RESPONSE, sizeof(RESPONSE));
  //write_req->buf = uv_buf_init(RESPONSE, sizeof(RESPONSE));
  int r = uv_write(write_req, (uv_stream_t *) &client->handle, &buf, 1, echo_write);
  ASSERT(r == 0);
  return 1;
}

///Callback function to release resources
void free_write_req(uv_write_t* req) {
    write_req_t* wr = (write_req_t*)req;
    free(wr->buf.base);
    free(wr);
}

///Allocate space to store accepted data
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void close_cb(uv_handle_t *handle) {
  printf("Request handle done\n\n");
  client_t *client = (client_t *) handle->data;
  free(handle->data);
  return;
}

void shutdown_cb(uv_shutdown_t *shutdown_req, int status) {
  printf("Request complete and shutdowned\n");
  uv_close((uv_handle_t *) shutdown_req->handle, close_cb);
  free(shutdown_req);
}

void echo_write(uv_write_t* req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    printf("Data send successfully\n");
    free_write_req(req);
	return;
}

char data_receive[200]  = {0}; 
///Put the data read from socket into request (req) and call callback function to check the status after writing (buf - > base nread bytes) to release the memory occupied by req
///Note here that req is handled by the callback function when it is read correctly
///When EOF / other errors occur, you need to close the socket and release the memory occupied by buf - > base
///EOF means the socket has been closed
///Because there is no callback function at this time
///Asynchronous callbacks are error prone
void echo_read(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
    if (nread > 0) {
        memset(data_receive, 0, 200);
		printf("Data receive: ....\n%s\n",buf->base);
        write_req_t* req = (write_req_t *)malloc(sizeof(write_req_t));
        client_t *client = (client_t *) handle->data;
        //strcpy(data_receive, buf->base);
        //uv_thread_create(&hare_id, (void *) thread_handle_http_client_connect,(uv_stream_t*) handle);
 
        size_t parsed = http_parser_execute(&client->parser, &parser_settings, buf->base, nread);
		 uv_buf_t Mybuf = uv_buf_init(RESPONSE, sizeof(RESPONSE));
		 //req->buf = uv_buf_init(RESPONSE, sizeof(RESPONSE));
         uv_write((uv_write_t*)req, (uv_stream_t*)client, &Mybuf, 1, echo_write);
        
        //while(1);
        //return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        printf("Errir bread < 0\n");
        //pthread_mutex_lock(&lock);
        uv_close((uv_handle_t*)handle, NULL);
    }
    printf("Begin shutdown client\n");
	uv_shutdown_t *shutdown_req = (uv_shutdown_t *)malloc(sizeof(uv_shutdown_t));
    uv_shutdown(shutdown_req, handle, shutdown_cb);
    free(buf->base);
}

///Establishment of a new connection
void *thread_handle_http_client_connect(uv_stream_t* server) {
    printf("\nThere are a new connection\n");
        
    client_t* client = (client_t*)malloc(sizeof(client_t));
    uv_tcp_init(server->loop, &client->handle);
    client->handle.data = client;

    if (uv_accept((uv_stream_t *)server, (uv_stream_t*)&client->handle) == 0) {
		printf("Start reading data....\n");
        
        http_parser_init(&client->parser, HTTP_REQUEST);
        client->parser.data = client;
        printf("parrser done\n");
        uv_read_start((uv_stream_t *)client, alloc_buffer, echo_read);
    } else {
        uv_close((uv_handle_t*)client, NULL);
    }
    printf("done on_new_connection\n");
    
}

// void *thread_handle_http_client_connect(uv_stream_t* handle)
// {
//    //uv_tcp_open(&client->sock, client_fd_dup);
//     printf("Starting newthread \n");
//     // uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
//      client_t *client = (client_t *) handle->data;
//     size_t parsed = http_parser_execute(&client->parser, &parser_settings, data_receive, strlen(data_receive));
// }


void Myclose_cb(uv_handle_t *handle) {
  uv_tcp_t *client_sock=(uv_tcp_t *)handle;
  free (client_sock);
}

void on_new_connection(uv_stream_t* server, int status) {

    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        exit(1);
    }


    //uv_thread_create(&hare_id, (void *) thread_handle_http_client_connect,(uv_stream_t*) server);
    //pthread_t *thread_ID2 = (pthread_t *)malloc(sizeof(pthread_t));
    //pthread_create(&thread_ID1, NULL, (void *)thread_handle_http_client_connect, server);
    //pthread_create(&thread_ID1, NULL, (void *)thread_handle_http_client_connect, server);
    printf("There are a new connection on_new_connection\n");
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        exit(1);
    }
    //client_t* client = (client_t *)malloc(sizeof(client_t));
    //uv_tcp_init(server->loop, &client->handle);
    //printf("Dia chi client %d, va client->handle %d va dia chi client->handle.data la first %d\n",client , client->handle ,client->handle.data);
    //client->handle.data = client;
    //printf("Dia chi client %d, va dia chi client->handle.data la second %d va client->handle = %d\n",client ,client->handle, &client->handle.data);
    uv_thread_create(&hare_id, (void *) thread_handle_http_client_connect,(uv_stream_t*) server);
    printf("done on_new_connection\n");
}

int main() {


    loop = uv_default_loop();

    parser_settings.on_headers_complete = headers_complete_cb;

    uv_tcp_init(loop, &server);

    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }
    printf("hello before\n");
    uv_run(loop, UV_RUN_DEFAULT);
    printf("hello after\n");

}