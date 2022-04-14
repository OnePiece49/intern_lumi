#include "lib/http.hpp"
#include "lib/registerAPI.hpp"
#include <iostream>
using namespace std;

s_fdserver Myserver;
vector<s_api *> MyApi;

void respond(void *) {
    //cout << "respond func" << endl;
    Http:: SendHttpRespond(&Myserver);
}

void respond_hello(void *) {
    cout << "respond func" << endl;
    Http:: SendHttpRespond(&Myserver);
}

int main()
{
    int a = 0;
    MyApi:: RegisterApi("GET", "/", respond, NULL, &a, MyApi);
    MyApi:: RegisterApi("GET", "/", respond_hello, NULL, &a, MyApi);
    //MyApi:: checkApi("GET", "/", MyApi);
    //MyApi:: checkApi("GET", "/ ", MyApi);
    Http:: InitHttp(&Myserver, 7000);
    cout << a << endl;
    while (1)
    {   
        Myserver.fd_data = Http:: AcceptConnect(&Myserver);
    }
}