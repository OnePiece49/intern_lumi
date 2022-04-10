#include "lib/http.hpp"

int main()
{
    int fd_server;
    int fddata;
    Http Myserver;

    Myserver.InitHttp(&fd_server, 7000);
    while (1)
    {   
        fddata = Myserver.AcceptConnect(&fd_server);
    }
}