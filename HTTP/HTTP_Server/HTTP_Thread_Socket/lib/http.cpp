#include "http.hpp"

using namespace std;

void Http:: SendHttpRespond(int *fddata) {
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
    write(*fddata, respond , strlen(respond));
}

void Http:: GetHttpRequest(int *fd) {
    int *fddata = (int *)fd;
    int byte_read = 0;
    char buf[300] = {0};
    char respond[300];

    byte_read =  read(*fddata, buf, 300);
    cout << "Data receive : " << buf << endl;
}

void* Http:: ThreadHandleRespond(void *fd) {
    int *fddata = (int *)fd;
    GetHttpRequest(fddata);
    SendHttpRespond(fddata);
    return NULL;
}

int Http:: AcceptConnect(int *fd_server) {
    int *fddata = new int; 
    pthread_t threadID;
    struct sockaddr_in claddr = {0};
    socklen_t len = sizeof(claddr);

    if ((*fddata = accept(*fd_server , (struct sockaddr *)&claddr, &len))  == -1) {
        cout << "server not accept";
        exit;
    } else {
       pthread_create(&threadID, NULL, ThreadHandleRespond, (void *) fddata);
       cout << "accept connect" << endl;
    }
    return *fddata;
}

void Http:: InitHttp(int *fd_server, int Port) {
    int opt;
    //B1 : khoi tao socket
    *fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (*fd_server == -1) {
        cout << "cannot intiatize socket " << endl;
        return;
    }
    
    if (setsockopt(*fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        perror("setsockopt()"); 

    //B2 : Need to gán các thông số cho file socket "fd" đã tạo trên qua biến trung gian struct
    struct sockaddr_in svaddr = {0};   //Biến trung gian là svaddr
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = INADDR_ANY; 
    svaddr.sin_port = htons(Port);
    
    //B3 : Gán thông số cho file socket "fd" qua biến trung gian svaddr nhờ hàm
    if (bind(*fd_server, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_in)) == -1) {
        cout << "cannot bind socket " << endl;
        return;
    }

    //B4: Khởi tạo điều kiện cho phép tối đa bao nhiêu client kết nối tới ở hàng đợi
    if( listen(*fd_server, 128) == -1) {
        cout << "lisen failed " << endl;
        return;
    } 
    cout << "lisen OK" << endl;    
}