#include "lib/http.hpp"
#include <iostream>
using namespace std;

void respond_viepro(void *, string *respond) {
    SetHeaderHttpRespond(Connection, "keep-Alive", respond);
    SetBodyHttpRespond("Huy ngu", respond);
}

void respond_hello(void *, string *respond) {
    SetHeaderHttpRespond(Content_Type, "text/html", respond);
    SetHeaderHttpRespond(Connection, "keep-Alive", respond);
    SetHeaderHttpRespond(Connection, "keep-Alive", respond);
    SetHeaderHttpRespond(set_cookie, "gzip", respond);
    SetHeaderHttpRespond(Accept_Encoding, "jqkA", respond);
    SetHeaderHttpRespond(Huy_ngu, "Huy ngu vcl AAA", respond);
    SetHeaderHttpRespond(Accept_langue, "Adu jqk1234", respond);
    SetBodyHttpRespond("<!DOCTYPE html><html><head><title>Example</title></head><body><p>This is an example of a simple HTML page with one paragraph.</p></body></html> ", respond);
}

int main()
{
    HttpServer Myserver;
    Myserver.RegisterApi(GET, "/", respond_viepro, NULL);
    Myserver.RegisterApi(GET, "/data", respond_hello, NULL);
    Myserver.RegisterApi(POST, "/data", respond_hello, NULL);
    Myserver.InitHttp(8080);
}