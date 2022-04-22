#include "http.hpp"
#include <string.h>
#include <string>

using namespace std;

static string  RESPONSE =                 
  "HTTP/1.1 200 OK\r\n"           \
  "Content-Type: text/plain\r\n"  \
  "Content-Length: 11\r\n"        \
  "\r\n"                          \
  "vietpro vip\n";

static string  ERRORRESPOND =                 
  "HTTP/1.1 400 Bad Request\r\n"            \
  "Content-Type: text/plain\r\n"            \
  "Content-Length: 8\r\n"                   \
  "\r\n"                                    \
  "Bad API\n";

static vector<string>httpHeader = {"Content-Type", "Content-Length", "content-encoding", "set-cookie", "Connection"};

void SetHeaderHttpRespond(e_header key, string value, string *Respond) {
    if (Respond->find(httpHeader.at(key)) != -1) {
        int position_header = Respond->find(httpHeader.at(key));
        int postion_content_header = Respond->find(" ", position_header);
        int lengthContent = Respond->find("\r\n", position_header) - postion_content_header;
        
        Respond->erase(position_header + httpHeader.at(key).size() + 2, lengthContent - 1); // thừa 2 chỗ cho ": ",ko xóa kí tự /r
        Respond->insert(position_header + httpHeader.at(key).size() + 2, value);
        cout << Respond << endl;

    } else {
        string enter = "\r\n";
        int first_position_enter = 0;
        int second_position_enter = 0; 
        int check = 0;

        while (check != 2 && check != -2) {
            first_position_enter = Respond->find("\r\n", second_position_enter + 1);
            check = first_position_enter - second_position_enter;
            if(check == 2 || check == 2) {
                break;
            }
            second_position_enter = Respond->find("\r\n", first_position_enter + 1);
            check = first_position_enter - second_position_enter;
            //cout << first_position_enter << " " << second_position_enter << endl;
            //sleep(1);
        }
        string header = httpHeader[key] + ": " + value + "\r\n";

        if(check == 2) {
            Respond->insert(first_position_enter, header);
        } else {
            Respond->insert(second_position_enter, header);
        }
        //cout << *Respond << endl;
    }

}

void SetBodyHttpRespond(string body, string *HttpRespond) {
    string enter = "\r\n";
    int first_position_enter = 0;
    int second_position_enter = 0; 
    int check = 0;

        while (check != 2 && check != -2) {
            first_position_enter = HttpRespond->find("\r\n", second_position_enter + 1);
            check = first_position_enter - second_position_enter;
            //cout << first_position_enter << " " << second_position_enter << endl;
            if(check == 2 || check == 2) {
                break;
            }
            second_position_enter = HttpRespond->find("\r\n", first_position_enter + 1);
            check = first_position_enter - second_position_enter;
            //cout << first_position_enter << " " << second_position_enter << endl;
            //sleep(1);
        }

        if(check == 2) {
            HttpRespond->erase(first_position_enter + 2, HttpRespond->size() - first_position_enter);
            //cout << HttpRespond << endl;
            HttpRespond->insert(first_position_enter +2, body);
            string lengthbody = to_string(body.size());
            SetHeaderHttpRespond(Content_Length, lengthbody, HttpRespond);
        } else {
            HttpRespond->erase(second_position_enter + 2, HttpRespond->size() - second_position_enter);
            //cout << HttpRespond << endl;
            HttpRespond->insert(second_position_enter + 2, body);
        }
    cout << *HttpRespond << endl;
}

void HttpServer:: RegisterApi(methodHttp method, string url, void (*callback)(void *, string *), void *para) {
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
    for (int i = 0; i < fddata->MyApi.size(); i++) {
        if (fddata->MyApi[i].method == method && fddata->MyApi[i].url == url) {
            fddata->MyApi[i].func(fddata->MyApi[i].parameter, &fddata->httprespond);
            write(fddata->fd_data, fddata->httprespond.c_str() , strlen(fddata->httprespond.c_str()));
            delete fddata;
            fddata = nullptr;
            return;
        } 
    }
    cout << "None of API Regitered" << endl;
    write(fddata->fd_data, ERRORRESPOND.c_str() , strlen(ERRORRESPOND.c_str()));
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
        coppyHttpServer->httprespond = RESPONSE;
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