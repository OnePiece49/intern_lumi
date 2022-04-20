#include "http.hpp"
#include <string.h>
#include <string>

using namespace std;

void HttpServer:: RegisterApi(methodHttp method, string url, void (*callback)(void *, int *), void *para) {
    int check = 5;
    if (Server.MyApi.size() != 0) {
        for (int i = 0; i < Server.MyApi.size() + 1; i++) {
            if (Server.MyApi[i].method == method && Server.MyApi[i].url == url) {
                cout << "Api is exist" << endl;
                return;
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        if (method == i) {
            if (callback == NULL) {
                cout << "Callback must be a function" << endl;
                return;
            }
            Server.MyApi.push_back(s_api());
            Server.MyApi[number_api].method = method; 
            Server.MyApi[number_api].url = url; 
            Server.MyApi[number_api].parameter = para; 
            if(callback != NULL) {
                Server.MyApi[number_api].func = callback; 
            }
            number_api++;
            
        } else {
            check--;
            if(check == 0) {
                cout << "Not Method valid" << endl;
                return;
            }
        }
    }
}

void HttpServer:: CheckApi(methodHttp method, string url, s_fdclient *fddata) {
    cout << "Entered checkAPi" << endl;
    for (int i = 0; i < fddata->MyApi.size(); i++) {
        if (fddata->MyApi[i].method == method && fddata->MyApi[i].url == url) {
            fddata->MyApi[i].func(fddata->MyApi[i].parameter, &fddata->fd_data);
            delete fddata;
            fddata = nullptr;
            return;
        } 
    }
    cout << "None of API Regitered" << endl;
}

void HttpServer:: SendHttpRespond(int *fd) {
    char respond[400] = {0};
    char data_body[200] = {0};
    char content_length[20] = {0};
    int number_write = 0;
    char new_line[10] = "\r\n";
    int number_byteRespond = 0;
    
    memset(respond, 0, sizeof(respond));
    cout << "Enter data respond: ";
    fgets(data_body, 200, stdin);
    for (int i = 0; i < 200; i++) {
        if (data_body[i] == '\n') {
            data_body[i] = '\0';
        }
    }
    number_write = strlen(data_body);
    sprintf(content_length, "%d", number_write);

    strcat(respond, LINE_HEADER);           //LINE_HEADER
    strcat(respond, CONTENT_LENGTH);        //content-length
    strcat(respond, content_length);        //content
    strcat(respond, new_line);              // /r/n
    strcat(respond, new_line);              // /r/n
    strcat(respond, data_body);             // /r/n
    strcat(respond, new_line);              // body
    write(*fd, respond , strlen(respond));
}

void HttpServer:: GetHttpRequest(s_fdclient *fd) {
    int byte_read = 0;
    char buf[500] = {0};
    char respond[300];

    byte_read =  read(fd->fd_data, buf, MAXSIZE);
    cout << "Data receive : " << buf << endl;

    char *token_method = strtok(buf, " ");
    char *token_url = strtok(NULL, " ");
    int methodHtpp;
    string method(token_method);
    string url(token_url);

    if (method ==  "GET") {
        CheckApi(GET, url, fd); 
    } else if (method ==  "POST") {
        CheckApi(POST, url, fd); 
    } else if (method ==  "PUT") {
        CheckApi(PUT, url, fd); 
    } else if (method ==  "PATCH") {
        CheckApi(PATCH, url, fd); 
    } else if (method ==  "DELETE") {
        CheckApi(DELETE, url, fd); 
    } else {
        cout << "Not method valid!" << endl;
        return;
    }
}

void* HttpServer:: ThreadHandleRespond(void *fd) {
    pthread_detach(pthread_self());
    s_fdclient *fddata = (s_fdclient *)fd;
    int *numThread = fddata->number_thread;

    GetHttpRequest(fddata);
    (*numThread)++;
    return NULL;
}

void HttpServer::  AcceptConnect() {
    static int number_thread = 2;
    pthread_t threadID;
    struct sockaddr_in claddr = {0};
    socklen_t len = sizeof(claddr);
    s_fdclient *coppyHttpServer = new s_fdclient;

    coppyHttpServer->number_thread = &number_thread;
    while(number_thread == 0);
    number_thread--;
    coppyHttpServer->fd_data = accept(Server.fd_server , (struct sockaddr *)&claddr, &len);
    if (coppyHttpServer->fd_data  == -1) {
        cout << "server not accept" << endl;
        exit;
    } else {
        coppyHttpServer->MyApi = Server.MyApi;
        pthread_create(&threadID, NULL,  ThreadHandleRespond, (void *)coppyHttpServer);
        cout << "accept connect" << endl;
    }
}


void HttpServer:: InitHttp(int port) {
    int opt;
    //B1 : khoi tao socket
    Server.fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (Server.fd_server == -1) {
        cout << "cannot intiatize socket " << endl;
        return;
    }

    if (setsockopt(Server.fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        perror("setsockopt()"); 

    //B2 : Need to gán các thông số cho file socket "fd" đã tạo trên qua biến trung gian struct
    struct sockaddr_in svaddr = {0};   //Biến trung gian là svaddr
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = INADDR_ANY; 
    svaddr.sin_port = htons(port);

    //B3 : Gán thông số cho file socket "fd" qua biến trung gian svaddr nhờ hàm
    int bindState = bind(Server.fd_server, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_in));
    if (bindState == -1) {
        cout << "cannot bind socket " << endl;
        return;
    }

    //B4: Khởi tạo điều kiện cho phép tối đa bao nhiêu client kết nối tới ở hàng đợi
    if (listen(Server.fd_server, 128) == -1) {
        cout << "lisen failed " << endl;
        return;
    } 
    cout << "lisen OK in port " << port << endl; 
 
    while (1)
    {
        AcceptConnect();
    }
}