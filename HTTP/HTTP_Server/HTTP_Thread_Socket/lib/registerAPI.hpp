#ifndef __REGISTERAPI__
#define __REGISTERAPI__

#include <iostream>
#include <vector>
using namespace std;

typedef void (*MyFunc )(void *);
typedef struct {
    string method;
    string url;
    MyFunc func;
    void *parameter;
}s_api;

class MyApi {
    public:
        static void  RegisterApi(string method, string url, void (*callback)(void *), void *para, int *number_api, vector<s_api *> &MyApi);
        static void  checkApi(string method, string url, vector<s_api *> MyApi);
    
    private:
};

#endif