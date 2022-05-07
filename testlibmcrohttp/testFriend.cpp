#include <iostream>
#include <string>
#include <map>
using namespace std;

class wibu{
    public:
        wibu() {};
        void say_hihi() {
            cout << "hihih ae wibu" << endl;
        }

    private:
        friend class wibu_viet;
        string wibu_hihi = "hello I'm wibu god";

};

class wibu_viet{
    private:
        
    
    public:
        void say_hello_viet(wibu *wb) {
            cout << wb->wibu_hihi << endl;
            //wb->say_hihi();
        }
};

int main()
{
    int a = 12, b = 13;
    //int *a = new(int);
    // wibu *wb_Viet = new wibu();
    // wibu_viet Viet;
    // Viet.say_hello_viet(wb_Viet);
    const char *abc = "long ngu";
    printf("%s", abc);
    map<string, int*> *my_map = new (map<string, int*>);
    my_map->insert(pair<string, int*>("huyngu", &a));
    my_map->insert(pair<string, int*>("viet", &b));
    
    map<string, int*>::iterator it = my_map->find("viet");
    cout << it->first << endl;
    return 0;
}