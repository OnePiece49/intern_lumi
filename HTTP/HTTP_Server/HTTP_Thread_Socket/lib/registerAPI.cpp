#include "registerAPI.hpp"
#include <iostream>
using namespace std;

void MyApi:: RegisterApi(string method, string url, void (*callback)(void *), void *para, int *number_api, vector<s_api *> &MyApi) {
    int check = 5;
    string method_http[5]{"GET", "POST", "PUT", "PATCH", "DELETE"};
    for (int i = 0; i < 5; i++) {
        if (method.compare(method_http[i]) == 0) {
            if (callback == NULL) {
                cout << "Callback must be a function" << endl;
                return;
            }
            s_api *newAPI = new s_api;
            
            newAPI->method = method;
            newAPI->url = url;
            newAPI->func = callback;
            if(para != NULL) {
                newAPI->parameter = para;
            }
            MyApi.push_back(newAPI); 
            (*number_api)++;

        } else {
            check--;
            if(check == 0) {
                cout << "Not Method Invalid" << endl;
                return;
            }
        }
    }
}


void MyApi:: checkApi(string method, string url, vector<s_api *> MyApi) {
    for (int i = 0; i < MyApi.size(); i++) {
        if (MyApi[i]->method == method && MyApi[i]->url == url) {
            MyApi[i]->func(MyApi[i]->parameter);
            return;
        } 
    }
    cout << "None of API Regitered" << endl;
}