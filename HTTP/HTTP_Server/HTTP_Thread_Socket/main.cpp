#include "lib/http.hpp"
//#include "lib/registerAPI.hpp"
#include <iostream>
using namespace std;

void respond_viepro(void *, int *fd) {
    sleep(5);
    write(*fd, RESPONSE , strlen(RESPONSE));
}

void respond_hello(void *, int *fd) {
    HttpServer:: SendHttpRespond(fd);
}


int main()
{
    HttpServer Myserver;
    Myserver.RegisterApi(GET, "/", respond_viepro, NULL);
    Myserver.RegisterApi(GET, "/data", respond_hello, NULL);
    Myserver.RegisterApi(POST, "/data", respond_hello, NULL);
    Myserver.InitHttp(7004);
}