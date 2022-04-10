#include "./lib/http.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  if( argc != 2 ) {
    printf("usage: try './curl [url]' to make a get request.\n");
    return 1;
  }

  Http_Client Myclient;
  Myclient.init_http_client(argv[1]);
}