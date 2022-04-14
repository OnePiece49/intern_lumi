#include <iostream>
using namespace std;

class hello{
    public:
        static void hihi() {
            cout << "Hello wibu" << endl;
        }
};

int main()
{
    hello:: hihi();
}