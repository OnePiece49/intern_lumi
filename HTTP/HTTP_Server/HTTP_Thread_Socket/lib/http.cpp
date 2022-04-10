#include "http.hpp"

using namespace std;

Http:: Http(int fd_server) {
    this->fd_server = fd_server;
}

void Http:: dong_goi_ban_tin(char *respond, char *body, char *content_length ) {
    char new_line[10] = "\r\n";
    int number_byteRespond = 0;
    memset(respond, 0, sizeof(respond));

    strcat(respond, LINE_HEADER);           //LINE_HEADER
    strcat(respond, CONTENT_LENGTH);        //content-length
    strcat(respond, content_length);         //content
    strcat(respond, new_line);              // /r/n
    strcat(respond, new_line);              // /r/n
    strcat(respond, body);                  // /r/n
    strcat(respond, new_line);              // body
}

void Http:: ban_tin(char *respond) {
    char data_respond[500] = {0};
    char data_body[200] = {0};
    char content_length[20] = {0};
    int number_write = 0;
    
    cout << "Enter data respond: ";
    fgets(data_body, 200, stdin);
    for(int i = 0; i < 200; i++) {
        if(data_body[i] == '\n') {
            data_body[i] = '\0';
        }
    }
    number_write = strlen(data_body);
    sprintf(content_length, "%d", number_write);

    dong_goi_ban_tin(data_respond, data_body, content_length);
    strcpy(respond, data_respond);
}

void* Http:: thread_handle_respond(void *fd) {
    int *fddata = (int *)fd;
    int byte_read = 0;
    char buf[300] = {0};
    char respond[300];

    byte_read =  read(*fddata, buf, BUF_SIZE);
    cout << "Data receive : " << buf << endl;
    ban_tin(respond);
    write(*fddata, respond , strlen(respond));
    return NULL;
}

void Http:: accept_connect() {
    int *fddata = new int; 
    pthread_t threadID;
    struct sockaddr_in claddr = {0};
    socklen_t len = sizeof(claddr);

    if ((*fddata = accept(fd_server , (struct sockaddr *)&claddr, &len))  == -1) {
        cout << "server not accept";
        exit;
    } else {
       pthread_create(&threadID, NULL, thread_handle_respond, (void *) fddata);
       cout << "accept connect" << endl;
    }
}

void Http:: init_http() {
    int opt;
    //B1 : khoi tao socket
    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_server == -1) {
        cout << "cannot intiatize socket " << endl;
        return;
    }
    
    if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        perror("setsockopt()"); 

    //B2 : Need to gán các thông số cho file socket "fd" đã tạo trên qua biến trung gian struct
    struct sockaddr_in svaddr = {0};   //Biến trung gian là svaddr
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = INADDR_ANY; 
    svaddr.sin_port = htons(PORT_NUMBER);
    
    //B3 : Gán thông số cho file socket "fd" qua biến trung gian svaddr nhờ hàm
    if (bind(fd_server, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_in)) == -1) {
        cout << "cannot bind socket " << endl;
        return;
    }

    //B4: Khởi tạo điều kiện cho phép tối đa bao nhiêu client kết nối tới
    if( listen(fd_server, 128) == -1) {
        cout << "lisen failed " << endl;
        return;
    } 
    cout << "lisen OK" << endl;
    while (1) {
        accept_connect();
    }
    
}