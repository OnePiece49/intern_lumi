#include "http.hpp"
#include "registerAPI.hpp"
#include <string.h>
#include <string>

using namespace std;

void Http:: SendHttpRespond(s_fdserver *fd) {
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
    write(fd->fd_data, respond , strlen(respond));
}

void Http:: GetHttpRequest(s_fdserver *fd) {
    int byte_read = 0;
    char buf[500] = {0};
    char respond[300];

    byte_read =  read(fd->fd_data, buf, 500);
    cout << "Data receive : " << buf << endl;

    char *token_method = strtok(buf, " ");
    char *token_url = strtok(NULL, " ");
    string method(token_method);
    string url(token_url);
    cout << "Method : " <<  method  << "path : " << url << "." << endl;
    MyApi:: checkApi(method, url, fd->MyApi); 
}

void* Http:: ThreadHandleRespond(void *fd) {
    s_fdserver *fddata = (s_fdserver *)fd;
    GetHttpRequest(fddata);
    //SendHttpRespond(fddata);
    return NULL;
}

int Http:: AcceptConnect(s_fdserver *fd) {
    pthread_t threadID;
    struct sockaddr_in claddr = {0};
    socklen_t len = sizeof(claddr);

    if ((fd->fd_data = accept(fd->fd_server , (struct sockaddr *)&claddr, &len))  == -1) {
        cout << "server not accept";
        exit;
    } else {
       pthread_create(&threadID, NULL, ThreadHandleRespond, (void *) fd);
       cout << "accept connect" << endl;
    }
    return fd->fd_data;
}

void Http:: InitHttp(s_fdserver *fd, int port) {
    int opt;
    //B1 : khoi tao socket
    fd->fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (fd->fd_server == -1) {
        cout << "cannot intiatize socket " << endl;
        return;
    }
    
    if (setsockopt(fd->fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        perror("setsockopt()"); 

    //B2 : Need to gán các thông số cho file socket "fd" đã tạo trên qua biến trung gian struct
    struct sockaddr_in svaddr = {0};   //Biến trung gian là svaddr
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = INADDR_ANY; 
    svaddr.sin_port = htons(port);
    
    //B3 : Gán thông số cho file socket "fd" qua biến trung gian svaddr nhờ hàm
    if (bind(fd->fd_server, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_in)) == -1) {
        cout << "cannot bind socket " << endl;
        exit;
    }

    //B4: Khởi tạo điều kiện cho phép tối đa bao nhiêu client kết nối tới ở hàng đợi
    if( listen(fd->fd_server, 128) == -1) {
        cout << "lisen failed " << endl;
        exit;
    } 
    cout << "lisen OK" << endl;    
}