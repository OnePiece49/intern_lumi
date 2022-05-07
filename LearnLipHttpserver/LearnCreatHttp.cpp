#include <iostream>
#include <string>
using namespace std;

class Http {
    private:
        int _port = 0;
        string _huy_ngu = "huy ngu vl";
        friend class webserver;

    public:
        // Http() = default;
        // Http(const Http& b) = default;
        // Http(Http&& b) noexcept = default;
        // Http& operator=(const Http& b) = default;
        // Http& operator=(Http&& b) = default;

        explicit Http(int port) 
            : _port(port)
        {}

        Http& ChangePort(int port) {
            _port = port;
            return *this;
        }

        Http& ChangeString(string huyNgu) {
            _huy_ngu = huyNgu;
            return *this;
        }



};


class webserver {
 public:
     //Keeping this non explicit on purpose to easy construction through builder class.
    webserver(const Http& params):
        port(params._port), huyngu(params._huy_ngu)
    {}
    
    webserver(int thread, int port) : port(port) {
        this->thread = thread;
    }

    void GetPort() {
            cout << port << endl;
    }

        void GetString() {
            cout << huyngu << endl;
    }

    private:
        const int port;
        const string huyngu;
        int thread = 0;
    
    protected:
        webserver& operator=(const webserver& other);
};

int main()
{
    webserver Huyngu(300, 400);
    Huyngu.GetPort();
    Huyngu.GetString();
}