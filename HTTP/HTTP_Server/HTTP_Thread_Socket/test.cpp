#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    int a;
    string hello;
}hihi;

void hello(vector<hihi> &arr) {
    arr.push_back(hihi());
    arr[0].a = 5;
    arr[0].hello = "helo world";
    
}

int main()
{
    vector<hihi> arr;
    hello(arr);
    cout << arr[0].hello << endl;
}