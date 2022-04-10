#ifndef __HTTP_
#define __HTTP_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

class Http_Client {
    private:
        struct Mystring {
            char *ptr;
            size_t len;
        };
        CURL *curl_handle;
        CURLcode res;

        void init_string(struct Mystring *s);
        static size_t writefunc(void *ptr, size_t size, size_t nmemb, struct Mystring *s);
    
    public:
        void init_http_client(char *url);

};

#endif