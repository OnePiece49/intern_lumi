#include "registerAPI.hpp"
#include <iostream>
using namespace std;

void func(void *) {
    cout << "Hello luong" << endl;
}

int main()
{
    int num_api = 0;
    vector<s_api *> MyApi;
    MyApi:: RegisterApi("GET", "/data", func, NULL ,&num_api, MyApi);
    MyApi:: RegisterApi("POST", "localhost:127.0.0.1", func, NULL ,&num_api, MyApi);
    MyApi:: RegisterApi("Get", "localhost:127.0.0.1", func, NULL ,&num_api, MyApi);
    MyApi:: checkApi("POST", "localhost:127.0.0.1", MyApi);
    MyApi:: checkApi("GET", "localhost:127.0.0.1", MyApi);
}