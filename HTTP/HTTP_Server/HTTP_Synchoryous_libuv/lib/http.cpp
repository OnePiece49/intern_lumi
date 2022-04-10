#include "http.hpp"   
#include <iostream>
#include <string>
using namespace std;

void http_server:: func() {
    cout << "wait" << endl;
    wait(NULL);
}
///Callback function to release resources
void http_server:: free_write_req(uv_write_t* req) {
    write_req_t* wr = (write_req_t*)req;
    free(wr->buf.base);
    free(wr);
}

///Allocate space to store accepted data
 void http_server:: alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

 void http_server:: close_cb(uv_handle_t *handle) {
  cout<< "Request handle done\n" << endl;
  client_t *client = (client_t *) handle->data;
  free(handle->data);
  //kill(getpid(), SIGQUIT);
}

 void http_server:: shutdown_cb(uv_shutdown_t *shutdown_req, int status) {
  cout << "Request complete and shutdowned" << endl;
  uv_close( (uv_handle_t *) shutdown_req->handle,  close_cb );
  free(shutdown_req);
}

 void http_server:: echo_write(uv_write_t* req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    cout<< "Data send successfully" << endl;
    free_write_req(req);
    return;
}

 void http_server:: echo_read(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
    if (nread > 0) {
		cout<< "Data receive: .... "<< "\n" << buf->base << endl;
    }
    cout << "Shut down" << endl;
	uv_shutdown_t *shutdown_req = (uv_shutdown_t *)malloc(sizeof(uv_shutdown_t));
    uv_shutdown(shutdown_req, handle, shutdown_cb);
    free(buf->base);
}

void http_server:: on_new_connection(uv_stream_t* server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        exit(1);
    }

    client_t* client = (client_t *)malloc(sizeof(client_t));
    uv_tcp_init(server->loop, &client->handle);
    client->handle.data = client;
    if (uv_accept(server, (uv_stream_t*)&client->handle) == 0) {
	    cout << "Start reading data...." << endl;
        if (uv_is_readable((uv_stream_t *) client )) {
            uv_read_start((uv_stream_t *)client, alloc_buffer, echo_read);
            write_req_t* req = (write_req_t *)malloc(sizeof(write_req_t));
            client_t *Sendclient = (client_t *) client->handle.data;
            uv_buf_t Mybuf = uv_buf_init(RESPONSE, sizeof(RESPONSE));
            uv_write((uv_write_t*)req, (uv_stream_t*)Sendclient, &Mybuf, 1, echo_write);
        }
    }
}

void http_server:: init_http() {
    uv_tcp_init(loop, server);
    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);
    uv_tcp_bind(server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)server, DEFAULT_BACKLOG, http_server:: on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return;
    }
    uv_run(loop, UV_RUN_DEFAULT);
}

http_server:: http_server(uv_loop_t *loop, struct sockaddr_in addr, uv_tcp_t *server) 
{
    this->loop = loop;
    this->addr = addr;
    this->server = server;
}