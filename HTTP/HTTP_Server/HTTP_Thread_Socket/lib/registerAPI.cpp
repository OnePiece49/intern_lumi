#include "registerAPI.hpp"
#include <iostream>
using namespace std;

void MyApi:: RegisterApi(methodHttp method, string url, void (*callback)(void *), void *para, vector<s_api> &MyApi) {
    int check = 5;
    for (int i = 0; i < MyApi.size(); i++) {
        if (ApiServer[i].method == method && ApiServer[i].url == url) {
            cout << "Api is exist" << endl;
            return;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (method == i) {
            if (callback == NULL) {
                cout << "Callback must be a function" << endl;
                return;
            }
            MyApi.push_back(s_api());
            MyApi[number_api].method = method; 
            MyApi[number_api].url = url; 
            MyApi[number_api].parameter = para; 
            if(callback != NULL) {
                MyApi[*number_api].func = callback; 
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


void MyApi:: checkApi(methodHttp method, string url, vector<s_api> ApiServer) {
    for (int i = 0; i < ApiServer.size(); i++) {
        if (ApiServer[i].method == method && ApiServer[i].url == url) {
            ApiServer[i].func(MyApi[i].parameter);
            return;
        } 
    }
    cout << "None of API Regitered" << endl;
}