#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <microhttpd.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

using namespace std;

#define COMPARATOR(x, y, op) { \
    size_t l1 = (x).size(); \
    size_t l2 = (y).size(); \
    if (l1 < l2) return true; \
    if (l1 > l2) return false; \
    \
    for (size_t n = 0; n < l1; n++) { \
        int xc = op((x)[n]); \
        int yc = op((y)[n]); \
        if (xc < yc) return true; \
        if (xc > yc) return false; \
    } \
    return false; \
}

class header_comparator {
 public:
     /**
      * Operator used to compare strings.
      * @param first string
      * @param second string
     **/
     bool operator()(const std::string& x, const std::string& y) const {
         COMPARATOR(x, y, std::toupper);
     }
};

class http_utils {
    public:
        static const char* http_version_1_0;
        static const char* http_version_1_1;

        static const char* http_header_content_type;

        static const char* http_method_connect;
        static const char* http_method_delete;
        static const char* http_method_head;
        static const char* http_method_get;
        static const char* http_method_options;
        static const char* http_method_post;
        static const char* http_method_put;
        static const char* http_method_trace;
        static const char* http_method_patch;

};


// namespace httpserver {

// namespace details { std::shared_ptr<http_response> empty_render(const http_request& r); }

// void resource_init(std::map<std::string, bool>* res);

// /**
//  * Class representing a callable http resource.
// **/

// class http_response {
//  public:
//      http_response() = default;

//      explicit http_response(int response_code, const std::string& content_type):
//          response_code(response_code) {
//              headers[http_utils::http_header_content_type] = content_type;
//      }

//      /**
//       * Copy constructor
//       * @param b The http_response object to copy attributes value from.
//      **/
//      http_response(const http_response& b) = default;
//      http_response(http_response&& b) noexcept = default;

//      http_response& operator=(const http_response& b) = default;
//      http_response& operator=(http_response&& b) noexcept = default;

//      virtual ~http_response() = default;

//      /**
//       * Method used to get a specified header defined for the response
//       * @param key The header identification
//       * @return a string representing the value assumed by the header
//      **/
//      const std::string& get_header(const std::string& key) {
//          return headers[key];
//      }

//      /**
//       * Method used to get a specified footer defined for the response
//       * @param key The footer identification
//       * @return a string representing the value assumed by the footer
//      **/
//      const std::string& get_footer(const std::string& key) {
//          return footers[key];
//      }

//      const std::string& get_cookie(const std::string& key) {
//          return cookies[key];
//      }

//      /**
//       * Method used to get all headers passed with the request.
//       * @return a map<string,string> containing all headers.
//      **/
//      const std::map<std::string, std::string, header_comparator>& get_headers() const {
//          return headers;
//      }

//      /**
//       * Method used to get all footers passed with the request.
//       * @return a map<string,string> containing all footers.
//      **/
//      const std::map<std::string, std::string, header_comparator>& get_footers() const {
//          return footers;
//      }

//      const std::map<std::string, std::string, header_comparator>& get_cookies() const {
//          return cookies;
//      }

//      /**
//       * Method used to get the response code from the response
//       * @return The response code
//      **/
//      int get_response_code() const {
//          return response_code;
//      }

//      void with_header(const std::string& key, const std::string& value) {
//          headers[key] = value;
//      }

//      void with_footer(const std::string& key, const std::string& value) {
//          footers[key] = value;
//      }

//      void with_cookie(const std::string& key, const std::string& value) {
//          cookies[key] = value;
//      }

//      void shoutCAST();

//      virtual MHD_Response* get_raw_response();
//      virtual void decorate_response(MHD_Response* response);
//      virtual int enqueue_response(MHD_Connection* connection, MHD_Response* response);

//  private:
//      int response_code = -1;

//      std::map<std::string, std::string, header_comparator> headers;
//      std::map<std::string, std::string, header_comparator> footers;
//      std::map<std::string, std::string, header_comparator> cookies;

//  protected:
//      friend std::ostream &operator<< (std::ostream &os, const http_response &r);
// };

// class http_resource {
//  public:
//      /**
//       * Class destructor
//      **/
//      virtual ~http_resource() = default;

//      /**
//       * Method used to answer to a generic request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render(const http_request& req) {
//          return details::empty_render(req);
//      }

//      /**
//       * Method used to answer to a GET request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render_GET(const http_request& req) {
//          return render(req);
//      }

//      /**
//       * Method used to answer to a POST request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render_POST(const http_request& req) {
//          return render(req);
//      }

//      /**
//       * Method used to answer to a PUT request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render_PUT(const http_request& req) {
//          return render(req);
//      }

//      /**
//       * Method used to answer to a HEAD request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render_HEAD(const http_request& req) {
//          return render(req);
//      }

//      /**
//       * Method used to answer to a DELETE request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render_DELETE(const http_request& req) {
//          return render(req);
//      }

//      /**
//       * Method used to answer to a TRACE request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render_TRACE(const http_request& req) {
//          return render(req);
//      }

//      /**
//       * Method used to answer to a OPTIONS request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render_OPTIONS(const http_request& req) {
//          return render(req);
//      }

//      /**
//       * Method used to answer to a PATCH request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render_PATCH(const http_request& req) {
//          return render(req);
//      }

//      /**
//       * Method used to answer to a CONNECT request
//       * @param req Request passed through http
//       * @return A http_response object
//      **/
//      virtual const std::shared_ptr<http_response> render_CONNECT(const http_request& req) {
//          return render(req);
//      }

//      /**
//       * Method used to set if a specific method is allowed or not on this request
//       * @param method method to set permission on
//       * @param allowed boolean indicating if the method is allowed or not
//      **/
//      void set_allowing(const std::string& method, bool allowed) {
//          if (method_state.count(method)) {
//              method_state[method] = allowed;
//          }
//      }

//      /**
//       * Method used to implicitly allow all methods
//      **/
//      void allow_all() {
//          std::map<std::string, bool>::iterator it;
//          for (it=method_state.begin(); it != method_state.end(); ++it) {
//              method_state[(*it).first] = true;
//          }
//      }

//      /**
//       * Method used to implicitly disallow all methods
//      **/
//      void disallow_all() {
//          std::map<std::string, bool>::iterator it;
//          for (it=method_state.begin(); it != method_state.end(); ++it) {
//              method_state[(*it).first] = false;
//          }
//      }

//      /**
//       * Method used to discover if an http method is allowed or not for this resource
//       * @param method Method to discover allowings
//       * @return true if the method is allowed
//      **/
//      bool is_allowed(const std::string& method) {
//          if (method_state.count(method)) {
//              return method_state[method];
//          }
//      }
//      /**
//       * Method used to return a list of currently allowed HTTP methods for this resource
//       * @return vector of strings
//      **/
//      std::vector<std::string> get_allowed_methods() {
//          std::vector<std::string> allowed_methods;

//          for (auto it = method_state.cbegin(); it != method_state.cend(); ++it) {
//              if ( (*it).second ) {
//                  allowed_methods.push_back((*it).first);
//              }
//          }

//          return allowed_methods;
//      }

//  protected:
//      /**
//       * Constructor of the class
//      **/
//      http_resource() {
//          resource_init(&method_state);
//      }

//      /**
//       * Copy constructor
//      **/

//  private:
//      friend class webserver;
//      std::map<std::string, bool> method_state;
// };

enum start_method_T {
     INTERNAL_SELECT = MHD_USE_SELECT_INTERNALLY | MHD_USE_AUTO,
     THREAD_PER_CONNECTION = MHD_USE_THREAD_PER_CONNECTION | MHD_USE_AUTO
};


class create_webserver {
 public:
     create_webserver() = default;
     create_webserver(const create_webserver& b) = default;
     create_webserver(create_webserver&& b) noexcept = default;
     create_webserver& operator=(const create_webserver& b) = default;
     create_webserver& operator=(create_webserver&& b) = default;

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
     bool stop();
     bool is_running();
     
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


     struct MHD_Daemon* daemon;

     static void request_completed(void *cls,
             struct MHD_Connection *connection, void **con_cls,
             enum MHD_RequestTerminationCode toe);

     static MHD_Result answer_to_connection(void* cls, MHD_Connection* connection, const char* url,
             const char* method, const char* version, const char* upload_data,
             size_t* upload_data_size, void** con_cls);

     static MHD_Result post_iterator(void *cls, enum MHD_ValueKind kind, const char *key,
             const char *filename, const char *content_type, const char *transfer_encoding,
             const char *data, uint64_t off, size_t size);

     static void upgrade_handler(void *cls, struct MHD_Connection* connection, void **con_cls, int upgrade_socket);

    // MHD_Result requests_answer_first_step(MHD_Connection* connection, struct details::modded_request* mr);

    //  MHD_Result requests_answer_second_step(MHD_Connection* connection,
    //          const char* method, const char* version, const char* upload_data,
    //          size_t* upload_data_size, struct details::modded_request* mr);

    //  MHD_Result finalize_answer(MHD_Connection* connection, struct details::modded_request* mr, const char* method);

    //  MHD_Result complete_request(MHD_Connection* connection, struct details::modded_request* mr, const char* version, const char* method);

     friend MHD_Result policy_callback(void *cls, const struct sockaddr* addr, socklen_t addrlen);
     friend void error_log(void* cls, const char* fmt, va_list ap);
     friend void access_log(webserver* cls, std::string uri);
     friend void* uri_log(void* cls, const char* uri);
     friend size_t unescaper_func(void * cls, struct MHD_Connection *c, char *s);
     friend class http_response;
};

MHD_Result policy_callback(void *, const struct sockaddr*, socklen_t);
void error_log(void*, const char*, va_list);
void* uri_log(void*, const char*);
void access_log(webserver*, string);
size_t unescaper_func(void*, struct MHD_Connection*, char*);

struct compare_value {
    bool operator() (const std::pair<int, int>& left, const std::pair<int, int>& right) const {
        return left.second < right.second;
    }
};

// WEBSERVER
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
    stop();
    pthread_mutex_destroy(&mutexwait);
    pthread_cond_destroy(&mutexcond);
}

void webserver::sweet_kill() {
    stop();
}

void webserver::request_completed(void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
    // These parameters are passed to respect the MHD interface, but are not needed here.
    std::ignore = cls;
    std::ignore = connection;
    std::ignore = toe;

    // details::modded_request* mr = static_cast<details::modded_request*>(*con_cls);
    // if (mr == nullptr) return;

    //delete mr;
    //mr = nullptr;
}

// bool webserver::register_resource(const std::string& resource, http_resource* hrm, bool family) {
//     if (single_resource && ((resource != "" && resource != "/") || !family)) {
//         throw std::invalid_argument("The resource should be '' or '/' and be marked as family when using a single_resource server");
//     }

//     details::http_endpoint idx(resource, family, true, regex_checking);

//     pair<map<details::http_endpoint, http_resource*>::iterator, bool> result = registered_resources.insert(map<details::http_endpoint, http_resource*>::value_type(idx, hrm));

//     if (result.second) {
//         registered_resources_str.insert(pair<string, http_resource*>(idx.get_url_complete(), result.first->second));
//     }

//     return result.second;
// }

bool webserver::start(bool blocking) {
    struct {
        MHD_OptionItem operator ()(enum MHD_OPTION opt, intptr_t val, void *ptr = nullptr) {
            MHD_OptionItem x = {opt, val, ptr};
            return x;
        }
    } gen;
    vector<struct MHD_OptionItem> iov;

    iov.push_back(gen(MHD_OPTION_NOTIFY_COMPLETED, (intptr_t) &request_completed, nullptr));
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
    if (bind_address == nullptr) {
        daemon = MHD_start_daemon(start_conf, port, NULL, this,
                &answer_to_connection, this, MHD_OPTION_ARRAY,
                &iov[0], MHD_OPTION_END);
    } else {
        daemon = MHD_start_daemon(start_conf, 1, NULL, this,
                &answer_to_connection, this, MHD_OPTION_ARRAY,
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

bool webserver::is_running() {
    return running;
}

bool webserver::stop() {
    if (!running) return false;

    pthread_mutex_lock(&mutexwait);
    running = false;
    pthread_cond_signal(&mutexcond);
    pthread_mutex_unlock(&mutexwait);

    MHD_stop_daemon(daemon);

    shutdown(bind_socket, 2);

    return true;
}

// void webserver::unregister_resource(const string& resource) {
//     // family does not matter - it just checks the url_normalized anyhow
//     details::http_endpoint he(resource, false, true, regex_checking);
//     registered_resources.erase(he);
//     registered_resources.erase(he.get_url_complete());
//     registered_resources_str.erase(he.get_url_complete());
// }

// void webserver::ban_ip(const string& ip) {
//     ip_representation t_ip(ip);
//     set<ip_representation>::iterator it = bans.find(t_ip);
//     if (it != bans.end() && (t_ip.weight() < (*it).weight())) {
//         bans.erase(it);
//         bans.insert(t_ip);
//     } else {
//         bans.insert(t_ip);
//     }
// }

// void webserver::allow_ip(const string& ip) {
//     ip_representation t_ip(ip);
//     set<ip_representation>::iterator it = allowances.find(t_ip);
//     if (it != allowances.end() && (t_ip.weight() < (*it).weight())) {
//         allowances.erase(it);
//         allowances.insert(t_ip);
//     } else {
//         allowances.insert(t_ip);
//     }
// }

// void webserver::unban_ip(const string& ip) {
//     bans.erase(ip_representation(ip));
// }

// void webserver::disallow_ip(const string& ip) {
//     allowances.erase(ip_representation(ip));
// }

// MHD_Result policy_callback(void *cls, const struct sockaddr* addr, socklen_t addrlen) {
//     // Parameter needed to respect MHD interface, but not needed here.
//     std::ignore = addrlen;

//     if (!(static_cast<webserver*>(cls))->ban_system_enabled) return MHD_YES;

//     if ((((static_cast<webserver*>(cls))->default_policy == http_utils::ACCEPT) &&
//                 ((static_cast<webserver*>(cls))->bans.count(ip_representation(addr))) &&
//                 (!(static_cast<webserver*>(cls))->allowances.count(ip_representation(addr)))) ||
//             (((static_cast<webserver*>(cls))->default_policy == http_utils::REJECT) &&
//              ((!(static_cast<webserver*>(cls))->allowances.count(ip_representation(addr))) ||
//               ((static_cast<webserver*>(cls))->bans.count(ip_representation(addr)))))) {
//         return MHD_NO;
//     }

//     return MHD_YES;
// }

// void* uri_log(void* cls, const char* uri) {
//     // Parameter needed to respect MHD interface, but not needed here.
//     std::ignore = cls;

//     struct details::modded_request* mr = new details::modded_request();
//     mr->complete_uri = new string(uri);
//     mr->second = false;
//     return reinterpret_cast<void*>(mr);
// }

// void error_log(void* cls, const char* fmt, va_list ap) {
//     // Parameter needed to respect MHD interface, but not needed here.
//     std::ignore = ap;

//     webserver* dws = static_cast<webserver*>(cls);
//     if (dws->log_error != nullptr) dws->log_error(fmt);
// }

// void access_log(webserver* dws, string uri) {
//     if (dws->log_access != nullptr) dws->log_access(uri);
// }

size_t unescaper_func(void * cls, struct MHD_Connection *c, char *s) {
    // Parameter needed to respect MHD interface, but not needed here.
    std::ignore = cls;
    std::ignore = c;

    // THIS IS USED TO AVOID AN UNESCAPING OF URL BEFORE THE ANSWER.
    // IT IS DUE TO A BOGUS ON libmicrohttpd (V0.99) THAT PRODUCING A
    // STRING CONTAINING '\0' AFTER AN UNESCAPING, IS UNABLE TO PARSE
    // ARGS WITH get_connection_values FUNC OR lookup FUNC.
    return std::string(s).size();
}


void webserver::upgrade_handler(void *cls, struct MHD_Connection* connection, void **con_cls, int upgrade_socket) {
    std::ignore = cls;
    std::ignore = connection;
    std::ignore = con_cls;
    std::ignore = upgrade_socket;
}


// const std::shared_ptr<http_response> webserver::not_found_page(details::modded_request* mr) const {
//     if (not_found_resource != nullptr) {
//         return not_found_resource(*mr->dhr);
//     } else {
//         return std::shared_ptr<http_response>(new string_response(NOT_FOUND_ERROR, http_utils::http_not_found));
//     }
// }

// const std::shared_ptr<http_response> webserver::method_not_allowed_page(details::modded_request* mr) const {
//     if (method_not_allowed_resource != nullptr) {
//         return method_not_allowed_resource(*mr->dhr);
//     } else {
//         return std::shared_ptr<http_response>(new string_response(METHOD_ERROR, http_utils::http_method_not_allowed));
//     }
// }

// const std::shared_ptr<http_response> webserver::internal_error_page(details::modded_request* mr, bool force_our) const {
//     if (internal_error_resource != nullptr && !force_our) {
//         return internal_error_resource(*mr->dhr);
//     } else {
//         return std::shared_ptr<http_response>(new string_response(GENERIC_ERROR, http_utils::http_internal_server_error, "text/plain"));
//     }
// }

// MHD_Result webserver::requests_answer_first_step(MHD_Connection* connection, struct details::modded_request* mr) {
//     mr->second = true;
//     mr->dhr = new http_request(connection, unescaper);

//     if (!mr->has_body) {
//         return MHD_YES;
//     }

//     mr->dhr->set_content_size_limit(content_size_limit);
//     const char *encoding = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, http_utils::http_header_content_type);

//     if (post_process_enabled &&
//         (nullptr != encoding &&
//             ((0 == strncasecmp(http_utils::http_post_encoding_form_urlencoded, encoding, strlen(http_utils::http_post_encoding_form_urlencoded))) ||
//              (0 == strncasecmp(http_utils::http_post_encoding_multipart_formdata, encoding, strlen(http_utils::http_post_encoding_multipart_formdata)))))) {
//         const size_t post_memory_limit(32 * 1024);  // Same as #MHD_POOL_SIZE_DEFAULT
//         mr->pp = MHD_create_post_processor(connection, post_memory_limit, &post_iterator, mr);
//     } else {
//         mr->pp = nullptr;
//     }
//     return MHD_YES;
// }

// MHD_Result webserver::requests_answer_second_step(MHD_Connection* connection, const char* method,
//         const char* version, const char* upload_data,
//         size_t* upload_data_size, struct details::modded_request* mr) {
//     if (0 == *upload_data_size) return complete_request(connection, mr, version, method);

//     if (mr->has_body) {
// #ifdef DEBUG
//         std::cout << "Writing content: " << std::string(upload_data, *upload_data_size) << std::endl;
// #endif  // DEBUG
//         // The post iterator is only created from the libmicrohttpd for content of type
//         // multipart/form-data and application/x-www-form-urlencoded
//         // all other content (which is indicated by mr-pp == nullptr)
//         // has to be put to the content even if put_processed_data_to_content is set to false
//         if (mr->pp == nullptr || put_processed_data_to_content) {
//             mr->dhr->grow_content(upload_data, *upload_data_size);
//         }

//         if (mr->pp != nullptr) {
//             mr->ws = this;
//             MHD_post_process(mr->pp, upload_data, *upload_data_size);
//             if (mr->upload_ostrm != nullptr && mr->upload_ostrm->is_open()) {
//                 mr->upload_ostrm->close();
//             }
//         }
//     }

//     *upload_data_size = 0;
//     return MHD_YES;
// }

// MHD_Result webserver::finalize_answer(MHD_Connection* connection, struct details::modded_request* mr, const char* method) {
//     int to_ret = MHD_NO;

//     map<string, http_resource*>::iterator fe;

//     http_resource* hrm;

//     bool found = false;
//     struct MHD_Response* raw_response;
//     if (!single_resource) {
//         const char* st_url = mr->standardized_url->c_str();
//         fe = registered_resources_str.find(st_url);
//         if (fe == registered_resources_str.end()) {
//             if (regex_checking) {
//                 map<details::http_endpoint, http_resource*>::iterator found_endpoint;

//                 details::http_endpoint endpoint(st_url, false, false, false);

//                 map<details::http_endpoint, http_resource*>::iterator it;

//                 size_t len = 0;
//                 size_t tot_len = 0;
//                 for (it = registered_resources.begin(); it != registered_resources.end(); ++it) {
//                     size_t endpoint_pieces_len = (*it).first.get_url_pieces().size();
//                     size_t endpoint_tot_len = (*it).first.get_url_complete().size();
//                     if (!found || endpoint_pieces_len > len || (endpoint_pieces_len == len && endpoint_tot_len > tot_len)) {
//                         if ((*it).first.match(endpoint)) {
//                             found = true;
//                             len = endpoint_pieces_len;
//                             tot_len = endpoint_tot_len;
//                             found_endpoint = it;
//                         }
//                     }
//                 }

//                 if (found) {
//                     vector<string> url_pars = found_endpoint->first.get_url_pars();

//                     vector<string> url_pieces = endpoint.get_url_pieces();
//                     vector<int> chunks = found_endpoint->first.get_chunk_positions();
//                     for (unsigned int i = 0; i < url_pars.size(); i++) {
//                         mr->dhr->set_arg(url_pars[i], url_pieces[chunks[i]]);
//                     }

//                     hrm = found_endpoint->second;
//                 }
//             }
//         } else {
//             hrm = fe->second;
//             found = true;
//         }
//     } else {
//         hrm = registered_resources.begin()->second;
//         found = true;
//     }

//     if (found) {
//         try {
//             if (hrm->is_allowed(method)) {
//                 mr->dhrs = ((hrm)->*(mr->callback))(*mr->dhr);  // copy in memory (move in case)
//                 if (mr->dhrs.get() == nullptr || mr->dhrs->get_response_code() == -1) {
//                     mr->dhrs = internal_error_page(mr);
//                 }
//             } else {
//                 mr->dhrs = method_not_allowed_page(mr);

//                 vector<string> allowed_methods = hrm->get_allowed_methods();
//                 if (allowed_methods.size() > 0) {
//                     string header_value = allowed_methods[0];
//                     for (auto it = allowed_methods.cbegin() + 1; it != allowed_methods.cend(); ++it) {
//                         header_value += ", " + (*it);
//                     }
//                     mr->dhrs->with_header(http_utils::http_header_allow, header_value);
//                 }
//             }
//         } catch(const std::exception& e) {
//             mr->dhrs = internal_error_page(mr);
//         } catch(...) {
//             mr->dhrs = internal_error_page(mr);
//         }
//     } else {
//         mr->dhrs = not_found_page(mr);
//     }

//     try {
//         try {
//             raw_response = mr->dhrs->get_raw_response();
//             if (raw_response == nullptr) {
//                 mr->dhrs = internal_error_page(mr);
//                 raw_response = mr->dhrs->get_raw_response();
//             }
//         } catch(const std::invalid_argument& iae) {
//             mr->dhrs = not_found_page(mr);
//             raw_response = mr->dhrs->get_raw_response();
//         } catch(const std::exception& e) {
//             mr->dhrs = internal_error_page(mr);
//             raw_response = mr->dhrs->get_raw_response();
//         } catch(...) {
//             mr->dhrs = internal_error_page(mr);
//             raw_response = mr->dhrs->get_raw_response();
//         }
//     } catch(...) {  // catches errors in internal error page
//         mr->dhrs = internal_error_page(mr, true);
//         raw_response = mr->dhrs->get_raw_response();
//     }
//     mr->dhrs->decorate_response(raw_response);
//     to_ret = mr->dhrs->enqueue_response(connection, raw_response);
//     MHD_destroy_response(raw_response);
//     return (MHD_Result) to_ret;
// }

// MHD_Result webserver::complete_request(MHD_Connection* connection, struct details::modded_request* mr, const char* version, const char* method) {
//     mr->ws = this;

//     mr->dhr->set_path(mr->standardized_url->c_str());
//     mr->dhr->set_method(method);
//     mr->dhr->set_version(version);

//     return finalize_answer(connection, mr, method);
// }
#define FILENAME "picture.txt"
MHD_Result webserver::answer_to_connection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
  struct MHD_Response *response;
  int fd;
  enum MHD_Result ret;
  struct stat sbuf;
  (void) cls;               /* Unused. Silent compiler warning. */
  (void) url;               /* Unused. Silent compiler warning. */
  (void) version;           /* Unused. Silent compiler warning. */
  (void) upload_data;       /* Unused. Silent compiler warning. */
  (void) upload_data_size;  /* Unused. Silent compiler warning. */
  (void) con_cls;           /* Unused. Silent compiler warning. */
  if (0 != strcmp (method, "GET"))
    return MHD_NO;

  if ( (-1 == (fd = open (FILENAME, O_RDONLY))) ||
       (0 != fstat (fd, &sbuf)) )
  {
    const char *errorstr =
      "<html><body>An internal server error has occurred!\
                              </body></html>";
    /* error accessing file */
    if (fd != -1)
      (void) close (fd);
    response =
      MHD_create_response_from_buffer (strlen (errorstr),
                                       (void *) errorstr,
                                       MHD_RESPMEM_PERSISTENT);
    if (NULL != response)
    {
      ret =
        MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR,
                            response);
      MHD_destroy_response (response);

      return ret;
    }
    else
      return MHD_NO;
   }

  response =
    MHD_create_response_from_fd_at_offset64 (sbuf.st_size, fd, 0);
  MHD_add_response_header (response, "Content-Type", "vlon");
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}

// MHD_Result webserver::answer_to_connection(void* cls, MHD_Connection* connection, const char* url, const char* method,
//         const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls) {
//     struct details::modded_request* mr = static_cast<struct details::modded_request*>(*con_cls);

//     if (mr->second != false) {
//         return static_cast<webserver*>(cls)->requests_answer_second_step(connection, method, version, upload_data, upload_data_size, mr);
//     }

//     const MHD_ConnectionInfo * conninfo = MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CONNECTION_FD);

//     if (static_cast<webserver*>(cls)->tcp_nodelay) {
//         int yes = 1;
//         setsockopt(conninfo->connect_fd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&yes), sizeof(int));
//     }

//     std::string t_url = url;

//     base_unescaper(&t_url, static_cast<webserver*>(cls)->unescaper);
//     mr->standardized_url = new string(http_utils::standardize_url(t_url));

//     mr->has_body = false;

//     access_log(static_cast<webserver*>(cls), *(mr->complete_uri) + " METHOD: " + method);

//     if (0 == strcasecmp(method, http_utils::http_method_get)) {
//         mr->callback = &http_resource::render_GET;
//     } else if (0 == strcmp(method, http_utils::http_method_post)) {
//         mr->callback = &http_resource::render_POST;
//         mr->has_body = true;
//     } else if (0 == strcasecmp(method, http_utils::http_method_put)) {
//         mr->callback = &http_resource::render_PUT;
//         mr->has_body = true;
//     } else if (0 == strcasecmp(method, http_utils::http_method_delete)) {
//         mr->callback = &http_resource::render_DELETE;
//         mr->has_body = true;
//     } else if (0 == strcasecmp(method, http_utils::http_method_patch)) {
//         mr->callback = &http_resource::render_PATCH;
//         mr->has_body = true;
//     } else if (0 == strcasecmp(method, http_utils::http_method_head)) {
//         mr->callback = &http_resource::render_HEAD;
//     } else if (0 ==strcasecmp(method, http_utils::http_method_connect)) {
//         mr->callback = &http_resource::render_CONNECT;
//     } else if (0 == strcasecmp(method, http_utils::http_method_trace)) {
//         mr->callback = &http_resource::render_TRACE;
//     } else if (0 ==strcasecmp(method, http_utils::http_method_options)) {
//         mr->callback = &http_resource::render_OPTIONS;
//     }

//     return static_cast<webserver*>(cls)->requests_answer_first_step(connection, mr);
// }

// }  


int main()
{
    webserver ws = create_webserver(8080)
        .start_method(THREAD_PER_CONNECTION)
        //.max_threads(2)
        .max_connections(2)
        .per_IP_connection_limit(2)
        .connection_timeout(7);

    ws.start();
    return 0;
}