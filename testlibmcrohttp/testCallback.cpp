#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <microhttpd.h>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

class http_request {
 public:
     //static const char EMPTY[];

     const std::string get_user() const;

     const std::string get_digested_user() const;

     const std::string get_pass() const;

     const std::string& get_path() const {
         return path;
     }

     const std::string& get_method() const {
         return method;
     }
     

    // const std::map<std::string, std::string, http::header_comparator> get_cookies() const;

    // const std::map<std::string, std::string, http::arg_comparator> get_args() const;

     const std::string get_header(const std::string& key) const;

     const std::string get_cookie(const std::string& key) const;

     const std::string get_footer(const std::string& key) const;

     const std::string get_arg(const std::string& key) const;

     const std::string& get_content() const {
         return content;
     }

     bool content_too_large() const {
         return content.size() >= content_size_limit;
     }

     const std::string get_querystring() const;

     const std::string& get_version() const {
         return version;
     }

     uint16_t get_requestor_port() const;

 private:
     /**
      * Default constructor of the class. It is a specific responsibility of apis to initialize this type of objects.
     **/
     http_request() = default;

      http_request(MHD_Connection* underlying_connection):
         underlying_connection(underlying_connection)
         { }


     ~http_request();

     std::string path;
     std::string method;
     std::string content = "";
     size_t content_size_limit = static_cast<size_t>(-1);
     std::string version;

     struct MHD_Connection* underlying_connection = nullptr;
     const std::string get_connection_value(const std::string& key, enum MHD_ValueKind kind) const;


     friend class webserver;
};

const std::string http_request::get_connection_value(const std::string& key, enum MHD_ValueKind kind) const {
    printf("kaka\n");

    const char* header_c = MHD_lookup_connection_value(underlying_connection, kind, key.c_str());
    
    //printf("%s\n", header_c);
    if (header_c == nullptr) cout << "Not header" << endl;
    else {
        printf("%s\n", header_c);
    }

    //return header_c;
}


const std::string http_request::get_header(const std::string& key) const {
    return get_connection_value(key, MHD_HEADER_KIND);
}


const std::string http_request::get_cookie(const std::string& key) const {
    return get_connection_value(key, MHD_COOKIE_KIND);
}



class http_response {
 public:
     explicit http_response( std::string content,
                              int response_code = 200,
                              const std::string& content_type = "text/plain")
         : content(std::move(content))
           {respond(response_code, content_type);}

     void respond(int response_code, const std::string& content_type)
     {
        std::map<std::string, std::string>:: iterator var = headers.find(content_type);
         if (var != headers.end()) {
            headers.erase(var);
            headers.insert(pair<string, string>("Content-Type", content_type));
        } else {
            headers.insert(pair<string, string>("Content-Type", content_type));
        }
         this->response_code = response_code;
    }

     const std::string& get_header(const std::string& key) {
        //const char* header_c = MHD_lookup_connection_value(underlying_connection, kind, key.c_str());
        //return 
     }

     const std::map<std::string, std::string>& get_headers() const {
         return headers;
     }

     int get_response_code() const {
         return response_code;
     }

     void with_header(const std::string& key, const std::string& value) {
         headers[key] = value;
     }

    struct MHD_Response* get_raw_response() {
        struct MHD_Response *response;
        response = MHD_create_response_from_buffer (strlen(content.c_str()),
                                       (void *) content.c_str(),
                                       MHD_RESPMEM_PERSISTENT);
        std::map<std::string, std::string> :: iterator it = headers.begin();
        for(it; it != headers.end(); it++) {
            MHD_add_response_header(response, it->first.c_str(), it->second.c_str());
        }
        return response;
     }
    //  virtual void decorate_response(MHD_Response* response);
    //  virtual int enqueue_response(MHD_Connection* connection, MHD_Response* response);

 private:
     int response_code = -1;
     std::map<std::string, std::string> headers;
     std::string content = "";

     struct MHD_Connection* underlying_connection = nullptr;

};


enum start_method_T {
     INTERNAL_SELECT = MHD_USE_SELECT_INTERNALLY | MHD_USE_AUTO,
     THREAD_PER_CONNECTION = MHD_USE_THREAD_PER_CONNECTION | MHD_USE_AUTO
};

class http_resource {
    public:
        http_resource() {
            (method_state)[MHD_HTTP_METHOD_GET] = true;
            (method_state)[MHD_HTTP_METHOD_POST] = true;
            (method_state)[MHD_HTTP_METHOD_PUT] = true;
            (method_state)[MHD_HTTP_METHOD_HEAD] = true;
            (method_state)[MHD_HTTP_METHOD_DELETE] = true;
            (method_state)[MHD_HTTP_METHOD_TRACE] = true;
            (method_state)[MHD_HTTP_METHOD_CONNECT] = true;
            (method_state)[MHD_HTTP_METHOD_OPTIONS] = true;
            (method_state)[MHD_HTTP_METHOD_PATCH] = true;
        }

        void allow_method(string method) {
            if (method_state.count(method)) {
                method_state[method] = true;
            }
        }

        void disallow_method(string method) {
            if (method_state.count(method)) {
                method_state[method] = false;
            }
        }

        void allow_all() {
            std::map<std::string, bool>::iterator it;
            for (it=method_state.begin(); it != method_state.end(); ++it) {
                method_state[(*it).first] = true;
            }
        }   

        void disallow_all() {
            std::map<std::string, bool>::iterator it;
            for (it=method_state.begin(); it != method_state.end(); ++it) {
                method_state[(*it).first] = false;
            }
        } 

    virtual const std::shared_ptr<http_response> render(const http_request& req) {
         return NULL;
     }

     virtual const std::shared_ptr<http_response> render_POST(const http_request& req) {
         return render(req);
     }

     virtual const std::shared_ptr<http_response> render_PUT(const http_request& req) {
         return render(req);
     }

     virtual const std::shared_ptr<http_response> render_HEAD(const http_request& req) {
         return render(req);
     }

     virtual const std::shared_ptr<http_response> render_DELETE(const http_request& req) {
         return render(req);
     }

     virtual const std::shared_ptr<http_response> render_TRACE(const http_request& req) {
         return render(req);
     }

     virtual const std::shared_ptr<http_response> render_OPTIONS(const http_request& req) {
         return render(req);
     }

     virtual const std::shared_ptr<http_response> render_PATCH(const http_request& req) {
         return render(req);
     }


     virtual const std::shared_ptr<http_response> render_CONNECT(const http_request& req) {
         return render(req);
     }

    virtual const std::shared_ptr<http_response> render_GET(const http_request& req) {
        std::shared_ptr<http_response> respond(new http_response ("hello ae wibu"));
        req.get_header("User-Agent");
        return respond;
    }
    std::map<std::string, bool> method_state;
    private:
        
        friend class webserver;
        
};

class create_webserver {
 public:

     explicit create_webserver(uint16_t port):
         _port(port) { }

     create_webserver& port(uint16_t port) {
         _port = port;
         return *this;
     }

     create_webserver& start_method(const start_method_T& start_method) {
         _start_method = start_method;
         return *this;
     }

     create_webserver& max_threads(int max_threads) {
         _max_threads = max_threads;
         return *this;
     }

     create_webserver& max_connections(int max_connections) {
         _max_connections = max_connections;
         return *this;
     }

     create_webserver& memory_limit(int memory_limit) {
         _memory_limit = memory_limit;
         return *this;
     }

     create_webserver& content_size_limit(size_t content_size_limit) {
         _content_size_limit = content_size_limit;
         return *this;
     }

     create_webserver& connection_timeout(int connection_timeout) {
         _connection_timeout = connection_timeout;
         return *this;
     }

     create_webserver& per_IP_connection_limit(int per_IP_connection_limit) {
         _per_IP_connection_limit = per_IP_connection_limit;
         return *this;
     }

     create_webserver& bind_address(const struct sockaddr* bind_address) {
         _bind_address = bind_address;
         return *this;
     }

     create_webserver& bind_socket(int bind_socket) {
         _bind_socket = bind_socket;
         return *this;
     }

     create_webserver& max_thread_stack_size(int max_thread_stack_size) {
         _max_thread_stack_size = max_thread_stack_size;
         return *this;
     }

     create_webserver& use_ssl() {
         _use_ssl = true;
         return *this;
     }

     create_webserver& no_ssl() {
         _use_ssl = false;
         return *this;
     }

     create_webserver& use_ipv6() {
         _use_ipv6 = true;
         return *this;
     }

     create_webserver& no_ipv6() {
         _use_ipv6 = false;
         return *this;
     }

     create_webserver& use_dual_stack() {
         _use_dual_stack = true;
         return *this;
     }

     create_webserver& no_dual_stack() {
         _use_dual_stack = false;
         return *this;
     }

     create_webserver& debug() {
         _debug = true;
         return *this;
     }

     create_webserver& no_debug() {
         _debug = false;
         return *this;
     }

     create_webserver& pedantic() {
         _pedantic = true;
         return *this;
     }

     create_webserver& no_pedantic() {
         _pedantic = false;
         return *this;
     }


     create_webserver& basic_auth() {
         _basic_auth_enabled = true;
         return *this;
     }

     create_webserver& no_basic_auth() {
         _basic_auth_enabled = false;
         return *this;
     }

     create_webserver& digest_auth() {
         _digest_auth_enabled = true;
         return *this;
     }

     create_webserver& no_digest_auth() {
         _digest_auth_enabled = false;
         return *this;
     }

     create_webserver& deferred() {
         _deferred_enabled = true;
         return *this;
     }

     create_webserver& no_deferred() {
         _deferred_enabled = false;
         return *this;
     }

     create_webserver& regex_checking() {
         _regex_checking = true;
         return *this;
     }

     create_webserver& no_regex_checking() {
         _regex_checking = false;
         return *this;
     }

     create_webserver& ban_system() {
         _ban_system_enabled = true;
         return *this;
     }

     create_webserver& no_ban_system() {
         _ban_system_enabled = false;
         return *this;
     }

     create_webserver& post_process() {
         _post_process_enabled = true;
         return *this;
     }

     create_webserver& no_post_process() {
         _post_process_enabled = false;
         return *this;
     }

     create_webserver& no_put_processed_data_to_content() {
         _put_processed_data_to_content = false;
         return *this;
     }

     create_webserver& put_processed_data_to_content() {
         _put_processed_data_to_content = true;
         return *this;
     }


     create_webserver& single_resource() {
         _single_resource = true;
         return *this;
     }

     create_webserver& no_single_resource() {
         _single_resource = false;
         return *this;
     }

    //  create_webserver& not_found_resource(render_ptr not_found_resource) {
    //      _not_found_resource = not_found_resource;
    //      return *this;
    //  }

    //  create_webserver& method_not_allowed_resource(render_ptr method_not_allowed_resource) {
    //      _method_not_allowed_resource = method_not_allowed_resource;
    //      return *this;
    //  }

    //  create_webserver& internal_error_resource(render_ptr internal_error_resource) {
    //      _internal_error_resource = internal_error_resource;
    //      return *this;
    //  }

 private:

     uint16_t _port = 8888;
     start_method_T _start_method = INTERNAL_SELECT;
     int _max_threads = 0;
     int _max_connections = 0;
     int _memory_limit = 0;
     size_t _content_size_limit = static_cast<size_t>(-1);
     int _connection_timeout = 180;
     int _per_IP_connection_limit = 0;
     const struct sockaddr* _bind_address = nullptr;
     int _bind_socket = 0;
     int _max_thread_stack_size = 0;
     bool _use_ssl = false;
     bool _use_ipv6 = false;
     bool _use_dual_stack = false;
     bool _debug = false;
     bool _pedantic = false;
     int _nonce_nc_size = 0;
     bool _basic_auth_enabled = true;
     bool _digest_auth_enabled = true;
     bool _regex_checking = true;
     bool _ban_system_enabled = true;
     bool _post_process_enabled = true;
     bool _put_processed_data_to_content = true;
     bool _generate_random_filename_on_upload = false;
     bool _deferred_enabled = false;
     bool _single_resource = false;
     bool _tcp_nodelay = false;

     friend class webserver;
};

class webserver {
 public:
     
     webserver(const create_webserver& params);  // NOLINT(runtime/explicit)

     ~webserver();

     bool start(bool blocking = false);
     //bool stop();
     bool is_running();
     bool register_resource(const std::string& resource, http_resource* res) {
         registered_resources_str.insert(pair<string, http_resource *>(resource, res));
     }

     void unregister_resource(const std::string& resource);
     void ban_ip(const std::string& ip);
     void allow_ip(const std::string& ip);
     void unban_ip(const std::string& ip);
     void disallow_ip(const std::string& ip);

     void sweet_kill();

 protected:
     webserver& operator=(const webserver& other);

 private:
     const uint16_t port;
     start_method_T start_method;
     const int max_threads;
     const int max_connections;
     const int memory_limit;
     const size_t content_size_limit;
     const int connection_timeout;
     const int per_IP_connection_limit;
     const struct sockaddr* bind_address;

     MHD_socket bind_socket;
     const int max_thread_stack_size;
     const bool use_ssl;
     const bool use_ipv6;
     const bool use_dual_stack;
     const bool debug;
     const bool pedantic;
     const std::string https_mem_key;
     const std::string https_mem_cert;
     const std::string https_mem_trust;
     const std::string https_priorities;
     const std::string digest_auth_random;
     const int nonce_nc_size;
     bool running;
     const bool basic_auth_enabled;
     const bool digest_auth_enabled;
     const bool regex_checking;
     const bool ban_system_enabled;
     const bool post_process_enabled;
     const bool put_processed_data_to_content;
     const std::string file_upload_dir;
     const bool generate_random_filename_on_upload;
     const bool deferred_enabled;
     bool single_resource;
     bool tcp_nodelay;
     pthread_mutex_t mutexwait;
     pthread_cond_t mutexcond;
     std::map<std::string, http_resource*> registered_resources_str;
     
     friend class http_resource;
     struct MHD_Daemon* daemon;

    //  static void request_completed(void *cls,
    //          struct MHD_Connection *connection, void **con_cls,
    //          enum MHD_RequestTerminationCode toe);

     static MHD_Result answer_to_connection(void* cls, MHD_Connection* connection, const char* url,
             const char* method, const char* version, const char* upload_data,
             size_t* upload_data_size, void** con_cls);

     static MHD_Result post_iterator(void *cls, enum MHD_ValueKind kind, const char *key,
             const char *filename, const char *content_type, const char *transfer_encoding,
             const char *data, uint64_t off, size_t size);

     static void upgrade_handler(void *cls, struct MHD_Connection* connection, void **con_cls, int upgrade_socket);


     friend MHD_Result policy_callback(void *cls, const struct sockaddr* addr, socklen_t addrlen);
     friend void error_log(void* cls, const char* fmt, va_list ap);
     friend void access_log(webserver* cls, std::string uri);
     friend void* uri_log(void* cls, const char* uri);
     friend size_t unescaper_func(void * cls, struct MHD_Connection *c, char *s);
     friend class http_response;
};


webserver::webserver(const create_webserver& params):
    port(params._port),
    start_method(params._start_method),
    max_threads(params._max_threads),
    max_connections(params._max_connections),
    memory_limit(params._memory_limit),
    content_size_limit(params._content_size_limit),
    connection_timeout(params._connection_timeout),
    per_IP_connection_limit(params._per_IP_connection_limit),
    bind_address(params._bind_address),
    bind_socket(params._bind_socket),
    max_thread_stack_size(params._max_thread_stack_size),
    use_ssl(params._use_ssl),
    use_ipv6(params._use_ipv6),
    use_dual_stack(params._use_dual_stack),
    debug(params._debug),
    pedantic(params._pedantic),
    nonce_nc_size(params._nonce_nc_size),
    running(false),
    basic_auth_enabled(params._basic_auth_enabled),
    digest_auth_enabled(params._digest_auth_enabled),
    regex_checking(params._regex_checking),
    ban_system_enabled(params._ban_system_enabled),
    post_process_enabled(params._post_process_enabled),
    put_processed_data_to_content(params._put_processed_data_to_content),
    generate_random_filename_on_upload(params._generate_random_filename_on_upload),
    deferred_enabled(params._deferred_enabled),
    single_resource(params._single_resource),
    tcp_nodelay(params._tcp_nodelay){}

webserver::~webserver() {
    //stop();
    pthread_mutex_destroy(&mutexwait);
    pthread_cond_destroy(&mutexcond);
}

bool webserver::start(bool blocking) {
    struct {
        MHD_OptionItem operator ()(enum MHD_OPTION opt, intptr_t val, void *ptr = nullptr) {
            MHD_OptionItem x = {opt, val, ptr};
            return x;
        }
    } gen;
    vector<struct MHD_OptionItem> iov;

    //iov.push_back(gen(MHD_OPTION_NOTIFY_COMPLETED, (intptr_t) &request_completed, nullptr));
    //iov.push_back(gen(MHD_OPTION_URI_LOG_CALLBACK, (intptr_t) &uri_log, this));
    //iov.push_back(gen(MHD_OPTION_EXTERNAL_LOGGER, (intptr_t) &error_log, this));
    //iov.push_back(gen(MHD_OPTION_UNESCAPE_CALLBACK, (intptr_t) &unescaper_func, this));
    iov.push_back(gen(MHD_OPTION_CONNECTION_TIMEOUT, connection_timeout));
    if (bind_socket != 0) {
        iov.push_back(gen(MHD_OPTION_LISTEN_SOCKET, bind_socket));
    }

    if (start_method == THREAD_PER_CONNECTION && (max_threads != 0 || max_thread_stack_size != 0)) {
        throw std::invalid_argument("Cannot specify maximum number of threads when using a thread per connection");
    }

    if (max_threads != 0) {
        iov.push_back(gen(MHD_OPTION_THREAD_POOL_SIZE, max_threads));
    }

    if (max_connections != 0) {
        iov.push_back(gen(MHD_OPTION_CONNECTION_LIMIT, max_connections));
    }

    if (memory_limit != 0) {
        iov.push_back(gen(MHD_OPTION_CONNECTION_MEMORY_LIMIT, memory_limit));
    }

    if (per_IP_connection_limit != 0) {
        iov.push_back(gen(MHD_OPTION_PER_IP_CONNECTION_LIMIT, per_IP_connection_limit));
    }

    if (max_thread_stack_size != 0) {
        iov.push_back(gen(MHD_OPTION_THREAD_STACK_SIZE, max_thread_stack_size));
    }

    if (nonce_nc_size != 0) {
        iov.push_back(gen(MHD_OPTION_NONCE_NC_SIZE, nonce_nc_size));
    }

    if (use_ssl) {
        // Need for const_cast to respect MHD interface that needs a void*
        iov.push_back(gen(MHD_OPTION_HTTPS_MEM_KEY, 0, reinterpret_cast<void*>(const_cast<char*>(https_mem_key.c_str()))));
    }

    if (use_ssl) {
        // Need for const_cast to respect MHD interface that needs a void*
        iov.push_back(gen(MHD_OPTION_HTTPS_MEM_CERT, 0, reinterpret_cast<void*>(const_cast<char*>(https_mem_cert.c_str()))));
    }

    if (https_mem_trust != "" && use_ssl) {
        // Need for const_cast to respect MHD interface that needs a void*
        iov.push_back(gen(MHD_OPTION_HTTPS_MEM_TRUST, 0, reinterpret_cast<void*>(const_cast<char*>(https_mem_trust.c_str()))));
    }

    if (https_priorities != "" && use_ssl) {
        // Need for const_cast to respect MHD interface that needs a void*
        iov.push_back(gen(MHD_OPTION_HTTPS_PRIORITIES, 0, reinterpret_cast<void*>(const_cast<char*>(https_priorities.c_str()))));
    }

    if (digest_auth_random != "") {
        // Need for const_cast to respect MHD interface that needs a char*
        iov.push_back(gen(MHD_OPTION_DIGEST_AUTH_RANDOM, digest_auth_random.size(), const_cast<char*>(digest_auth_random.c_str())));
    }

#ifdef HAVE_GNUTLS
    if (cred_type != http_utils::NONE) {
        iov.push_back(gen(MHD_OPTION_HTTPS_CRED_TYPE, cred_type));
    }
#endif  // HAVE_GNUTLS

    iov.push_back(gen(MHD_OPTION_END, 0, nullptr));

    int start_conf = start_method;

    if (use_ssl) {
        start_conf |= MHD_USE_SSL;
    }

    if (use_ipv6) {
        start_conf |= MHD_USE_IPv6;
    }

    if (use_dual_stack) {
        start_conf |= MHD_USE_DUAL_STACK;
    }

    if (debug) {
        start_conf |= MHD_USE_DEBUG;
    }
    if (pedantic) {
        start_conf |= MHD_USE_PEDANTIC_CHECKS;
    }

    if (deferred_enabled) {
        start_conf |= MHD_USE_SUSPEND_RESUME;
    }

#ifdef USE_FASTOPEN
    start_conf |= MHD_USE_TCP_FASTOPEN;
#endif
    daemon = nullptr;
    std::map<std::string, http_resource*> http_resource_coppy = registered_resources_str;
    if (bind_address == nullptr) {
        daemon = MHD_start_daemon(start_conf, port, NULL, this,
                &answer_to_connection, &http_resource_coppy, MHD_OPTION_ARRAY,
                &iov[0], MHD_OPTION_END);
    } else {
        daemon = MHD_start_daemon(start_conf, 1, NULL, this,
                &answer_to_connection, &http_resource_coppy, MHD_OPTION_ARRAY,
                &iov[0], MHD_OPTION_SOCK_ADDR, bind_address, MHD_OPTION_END);
    }

    if (daemon == nullptr) {
        throw std::invalid_argument("Unable to connect daemon to port: " + std::to_string(port));
    }

    bool value_onclose = false;

    running = true;

    if (blocking) {
        pthread_mutex_lock(&mutexwait);
        while (blocking && running) {
            pthread_cond_wait(&mutexcond, &mutexwait);
        }
        pthread_mutex_unlock(&mutexwait);
        value_onclose = true;
    }
    return value_onclose;
}

#define PORT 8888

MHD_Result
webserver:: answer_to_connection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
  std::string request_url(url);
  struct MHD_Response *response = {0};
  enum MHD_Result ret;
  (void) cls;               /* Unused. Silent compiler warning. */
  (void) url;               /* Unused. Silent compiler warning. */
  (void) method;            /* Unused. Silent compiler warning. */
  (void) version;           /* Unused. Silent compiler warning. */
  (void) upload_data;       /* Unused. Silent compiler warning. */
  (void) upload_data_size;  /* Unused. Silent compiler warning. */
  (void) con_cls;           /* Unused. Silent compiler warning. */

    std::map<std::string, http_resource*> *http_resource_rgt = (std::map<std::string, http_resource*>*)cls;
    //printf("%d va %d\n", htttpserver, cls);
    cout << http_resource_rgt->size() << endl;
    cout << "haha" << endl;
    
    std::map<std::string, http_resource*> ::iterator var_first;
    var_first = http_resource_rgt->find(request_url);
    string abc = var_first->first;
    //cout << abc << endl;
    cout << "haha" << endl;
    cout << var_first->first << endl;
    cout << "haha" << endl;
    if(var_first == http_resource_rgt->end()) {
        cout<< "vvlon" << endl;
        return MHD_NO;
    }
     cout << "hihih1" << endl;
    var_first->second->method_state[MHD_HTTP_METHOD_GET] ;
    cout << "hihih1" << endl;

    if (0 == strcmp(method, "GET")) {
        if(var_first->second->method_state[MHD_HTTP_METHOD_GET] == true) {
            cout << "hihih1.5" << endl;
            http_request *request = new http_request(connection);
            response = (var_first)->second->render_GET(*request)->get_raw_response();
            cout << "hihih2" << endl;
        }        
    } else if (0 == strcmp(method, "POST")) {
        if(var_first->second->method_state["POST"] == true) {
            http_request *request = new http_request(connection);
            response = (var_first)->second->render_GET(*request)->get_raw_response();
        }        
    } else if (0 == strcmp(method, "PUT")) {
        if(var_first->second->method_state["PUT"] == true) {
            http_request *request = new http_request(connection);
            response = (var_first)->second->render_GET(*request)->get_raw_response();
        }
    } else if (0 == strcmp(method, "DELETE")) {
        if(var_first->second->method_state["DELETE"] == true) {
            http_request *request = new http_request(connection);
            response = (var_first)->second->render_GET(*request)->get_raw_response();
        }
    } else if (0 == strcmp(method, "PATCH")) {
        if(var_first->second->method_state["PATCH"] == true) {
            http_request *request = new http_request(connection);
            response = (var_first)->second->render_GET(*request)->get_raw_response();
        }
    } else if (0 == strcmp(method, "HEAD")) {
        if(var_first->second->method_state["HEAD"] == true) {
            http_request *request = new http_request(connection);
            response = (var_first)->second->render_GET(*request)->get_raw_response();
        }
    } else if (0 ==strcmp(method, "CONNECT")) {
        if(var_first->second->method_state["CONNECT"] == true) {
            http_request *request = new http_request(connection);
            response = (var_first)->second->render_GET(*request)->get_raw_response();
        }
    } else if (0 == strcmp(method, "TRACE")) {
        if(var_first->second->method_state["TRACE"] == true) {
            http_request *request = new http_request(connection);
            response = (var_first)->second->render_GET(*request)->get_raw_response();
        }
    } else if (0 ==strcmp(method, "OPTION")) {
        if(var_first->second->method_state["OPTION"] == true) {
            http_request *request = new http_request(connection);
            response = (var_first)->second->render_GET(*request)->get_raw_response();
        }
    }

  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}

class test_http_resource : public http_resource {
     const std::shared_ptr<http_response> render_GET(const http_request& req) {
        std::shared_ptr<http_response> respond(new http_response ("hello ae GET", 300, "non vl"));
        respond->with_header("kaka", "wibu");
        respond->with_header("huy-ngu", "vietdz");
        return respond;
    }

         const std::shared_ptr<http_response> render_POST(const http_request& req) {
        std::shared_ptr<http_response> respond(new http_response ("hello ae POST"));
        return respond;
    }
};


int main()
{
    http_resource hello_ae_wibu;
    if(hello_ae_wibu.method_state[MHD_HTTP_METHOD_GET] == true)
        cout << "wibu đb" << endl;
    test_http_resource thr;
    webserver ws = create_webserver(8080).start_method(INTERNAL_SELECT)
                                        .max_connections(3)
                                        .connection_timeout(4);

    ws.register_resource("/", &hello_ae_wibu);
    ws.register_resource("/data", &thr);
    ws.start();
    

}