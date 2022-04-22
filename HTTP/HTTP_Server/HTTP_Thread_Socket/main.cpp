#include "lib/http.hpp"
#include <iostream>
using namespace std;

void respond_viepro(void *, string *respond) {
    
    SetHeaderHttpRespond(Connection, "keep-Alive", respond);
    SetBodyHttpRespond("Huy depzai", respond);
    sleep(5);
}

void respond_hello(void *, string *respond) {
    SetHeaderHttpRespond(Connection, "keep-Alive", respond);
    SetHeaderHttpRespond(Connection, "keep-Alive", respond);
    SetHeaderHttpRespond(set_cookie, "gzip", respond);
    SetBodyHttpRespond("Huy qua depzai ", respond);
}

int main()
{
    HttpServer Myserver;
    Myserver.RegisterApi(GET, "/", respond_viepro, NULL);
    Myserver.RegisterApi(GET, "/data", respond_hello, NULL);
    Myserver.RegisterApi(POST, "/data", respond_hello, NULL);
    Myserver.InitHttp(7008);
}