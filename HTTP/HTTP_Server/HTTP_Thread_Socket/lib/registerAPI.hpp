#ifndef __REGISTERAPI__
#define __REGISTERAPI__

#include <iostream>
#include <vector>
#include <http.hpp>
using namespace std;

typedef void (*MyFunc )(void *);

typedef struct {
    int method;
    string url;
    MyFunc func;
    void *parameter;
}s_api;

typedef enum  {
    GET = 0,
    POST = 1,
    PUSH = 2,
    PUT = 3,
    PATCH = 4,
}methodHttp;

class MyApi {
    public:
        static void  RegisterApi(methodHttp method, string url, void (*callback)(void *), void *para, vector<s_api> &MyApi);
        static void  checkApi(methodHttp method, string url, vector<s_api> MyApi);
    
    private:
        int number_api;
};

#endif