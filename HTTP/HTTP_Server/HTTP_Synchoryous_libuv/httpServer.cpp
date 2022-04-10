#include "http.hpp"
#include <iostream>
#include <string>

uv_loop_t* loop;
struct sockaddr_in addr;
uv_tcp_t server;

int main()
{
    loop = uv_default_loop();
    http_server Myserver(loop, addr, &server);
    Myserver.init_http();
}