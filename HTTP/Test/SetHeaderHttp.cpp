#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
using namespace std;

// vector<string>httpHeader = {"Content-Type", "Content-Length", "Accept-Encoding", "Accept-Language", "Connection"};

// string HttpRespond =    "HTTP/1.1 200 OK\r\n"           \
//                         "Content-Type: text/plain\r\n"  \
//                         "Content-Length: 11\r\n"        \
//                         "\r\n"                          \
//                         "vietpro vip";

typedef enum  {
    Content_Type = 0,
    Content_Length = 1,
    Accept_Encoding = 2,
    Accept_Language = 3,
    Connection = 4,
}e_header;


void SetHeader(e_header key, string value) {
    if  (HttpRespond.find(httpHeader.at(key)) != -1) {
        int position_header = HttpRespond.find(httpHeader.at(key));
        int postion_content_header = HttpRespond.find(" ", position_header);
        int lengthContent = HttpRespond.find("\r\n", position_header) - postion_content_header;
        
        HttpRespond.erase(position_header + httpHeader.at(key).size() + 2, lengthContent - 1); // thừa 2 chỗ cho ": ",ko xóa kí tự /r
        HttpRespond.insert(position_header + httpHeader.at(key).size() + 2, value);
        cout << HttpRespond << endl;

    } else {
        string enter = "\r\n";
        int first_position_enter = 0;
        int second_position_enter = 0; 
        int check = 0;

        while (check != 2 && check != -2) {
            first_position_enter = HttpRespond.find("\r\n", second_position_enter + 1);
            check = first_position_enter - second_position_enter;
            if(check == 2 || check == 2) {
                break;
            }
            second_position_enter = HttpRespond.find("\r\n", first_position_enter + 1);
            check = first_position_enter - second_position_enter;
            cout << first_position_enter << " " << second_position_enter << endl;
            sleep(1);
        }
        string header = httpHeader[key] + ": " + value + "\r\n";

        if(check == 2) {
            HttpRespond.insert(first_position_enter , header);
        } else {
            HttpRespond.insert(second_position_enter , header);
        }
        cout << HttpRespond << endl;
    }

}

void SetBodyHttpRespond(string body) {
    string enter = "\r\n";
    int first_position_enter = 0;
    int second_position_enter = 0; 
    int check = 0;

        while (check != 2 && check != -2) {
            first_position_enter = HttpRespond.find("\r\n", second_position_enter + 1);
            check = first_position_enter - second_position_enter;
             cout << first_position_enter << " " << second_position_enter << endl;
            if(check == 2 || check == 2) {
                break;
            }
            second_position_enter = HttpRespond.find("\r\n", first_position_enter + 1);
            check = first_position_enter - second_position_enter;
            cout << first_position_enter << " " << second_position_enter << endl;
            sleep(1);
        }

        if(check == 2) {
            HttpRespond.erase(first_position_enter + 2, HttpRespond.size() - first_position_enter);
            //cout << HttpRespond << endl;
            HttpRespond.insert(first_position_enter +2, body);
        } else {
            HttpRespond.erase(second_position_enter + 2, HttpRespond.size() - second_position_enter);
            //cout << HttpRespond << endl;
            HttpRespond.insert(second_position_enter + 2, body);
        }
    cout << HttpRespond << endl;
}

int main()
{
//cout << HttpRespond << endl;
   SetHeader(Content_Type, "hello luong");
   SetHeader(Connection, "hello world");
   SetBodyHttpRespond("Huy ngu vkl");
   SetHeader(Content_Type, "hello long ngu");
   SetHeader(Accept_Language, "html/json");
}   