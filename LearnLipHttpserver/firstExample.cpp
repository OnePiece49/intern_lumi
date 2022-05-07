/*
#include <iostream>

#include <httpserver.hpp>
using namespace httpserver;
class hello_world_resource : public httpserver::http_resource {
 public:
     const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request&);
     void set_some_data(const std::string &s) {data = s;}
     std::string data;
};

class hello_world_resource1 : public http_resource {
public:
    const std::shared_ptr<http_response> render(const http_request&) {
        return std::shared_ptr<http_response>(new string_response("Hello, 12345\n!", 200));
    }
};


// Using the render method you are able to catch each type of request you receive
const std::shared_ptr<httpserver::http_response> hello_world_resource::render(const httpserver::http_request& req) {
    // It is possible to store data inside the resource object that can be altered through the requests
    std::cout << "Data was: " << data << std::endl;
    std::string datapar = req.get_arg("data");
    set_some_data(datapar == "" ? "no data passed!!!" : datapar);
    std::cout << "Now data is:" << data << std::endl;
    sleep(5);
    // It is possible to send a response initializing an http_string_response that reads the content to send in response from a string.
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("Hello World!!!", 200));
}


int main() {
    // It is possible to create a webserver passing a great number of parameters. In this case we are just passing the port and the number of thread running.
    httpserver::webserver ws = httpserver::create_webserver(8081).start_method(httpserver::http::http_utils::INTERNAL_SELECT).max_connections(1).max_threads(3)
                                            .per_IP_connection_limit(2).connection_timeout(1);

    hello_world_resource hwr;
     hello_world_resource1 hwr1;
    // This way we are registering the hello_world_resource to answer for the endpoint
    // "/hello". The requested method is called (if the request is a GET we call the render_GET
    // method. In case that the specific render method is not implemented, the generic "render"
    // method is called.
    ws.register_resource("/hello", &hwr, true);
    ws.register_resource("/hihi", &hwr1, true);
    // This way we are putting the created webserver in listen. We pass true in order to have
    // a blocking call; if we want the call to be non-blocking we can just pass false to the method.
    ws.start(true);
    return 0;
}

*/

/* custom các lỗi server
#include <httpserver.hpp>

  using namespace httpserver;

  const std::shared_ptr<http_response> not_found_custom(const http_request& req) {
      return std::shared_ptr<string_response>(new string_response("Not found custom", 404, "text/plain"));
  }

  const std::shared_ptr<http_response> not_allowed_custom(const http_request& req) {
      return std::shared_ptr<string_response>(new string_response("Not allowed custom", 405, "text/plain"));
  }

  class hello_world_resource : public http_resource {
  public:
      const std::shared_ptr<http_response> render(const http_request&) {
          return std::shared_ptr<http_response>(new string_response("Hello, World!"));
      }
  };

  int main(int argc, char** argv) {
      webserver ws = create_webserver(8080)
          .not_found_resource(not_found_custom)
          .method_not_allowed_resource(not_allowed_custom);

      hello_world_resource hwr;
      hwr.disallow_all();
      hwr.set_allowing("GET", true);
      ws.register_resource("/hello", &hwr);
      ws.start(true);

      return 0;
  }

  */


#include <httpserver.hpp>
#include <iostream>

using namespace httpserver;
using namespace std;

void custom_access_log(const std::string& url) {
    std::cout << "ACCESSING: " << url << " chill chill" << std::endl;
}

void custom_access_log_error(const std::string& url) {
    std::cout << "ACCESSING: " << url << " chill errror" << std::endl;
}

const std::shared_ptr<httpserver::http_response> not_found_custom(const httpserver::http_request&) {
    return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("Not found custom", 404, "text/plain"));
}


const std::shared_ptr<httpserver::http_response> not_allowed_custom(const httpserver::http_request&) {
    return std::shared_ptr<httpsevrer::string_response>(new httpserver::string_response("Not allowed custom", 405, "text/plain"));
}

class hello_world_resource : public http_resource {
public:
    const std::shared_ptr<http_response> render(const http_request&) {
        return std::shared_ptr<http_response>(new string_response("Hello, World!"));
    }
};

class vietpro__resource : public http_resource {
public:
    const std::shared_ptr<http_response> render(const http_request& req) {
        cout << req.get_header("Content-Type") << endl;
        return std::shared_ptr<http_response>(new string_response("hihi: " + req.get_path() + " he " + req.get_path_piece(2) + req.get_method()));
    }
    
    const std::shared_ptr<http_response> render1(const http_request&req) {
        return std::shared_ptr<http_response>(new string_response("Hello vietpro GET!"));
    }

    
    const std::shared_ptr<http_response> render_CONNECT(const http_request&) {
        return std::shared_ptr<http_response>(new string_response("Hello vietpro POST!"));
    }

    const std::shared_ptr<http_response> render_POST(const http_request&) {
        std::shared_ptr<http_response> response(new string_response("Hello, World!"));
        response->with_header("Vietvipbrp", "MyValueViet");
        return response;
    }

    const std::shared_ptr<http_response> render_GET(const http_request&) {
         std::shared_ptr<http_response>response = shared_ptr<http_response>(new string_response("Hello wibu!"));
         response->with_header("Huy depzai vkl hehe", "hiih");
         response->with_footer("Hello ae hn", "hello_vn");
         response->with_cookie("hello cookie", "hello ae");
         //sleep(5);
         return response;
    }
    
    const std::map<std::string, std::string, http::header_comparator>& get_headers() (const http_request&req) {
        return std::map<std::string, std::string, http::header_comparator>()
    }
};


class user_pass_resource : public httpserver::http_resource {
 public:
     const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request& req) {
         if (req.get_user() != "myuser" || req.get_pass() != "mypass") {
             return std::shared_ptr<httpserver::basic_auth_fail_response>(new httpserver::basic_auth_fail_response("FAIL", "test@example.com"));
         } else
            return std::shared_ptr<httpserver::string_response>(new httpserver::string_response(req.get_user() + " " + req.get_pass(), 200, "text/plain"));
     }
    const std::shared_ptr<http_response> render_POST(const http_request&) {
        std::shared_ptr<http_response> response(new string_response("Hello, World!"));
        response->with_header("Vietvipbro", "MyValueViet");
        return response;
    }
     

};
static int counter = 0;

ssize_t test_callback (std::shared_ptr<void> closure_data, char* buf, size_t max) {
    if (counter == 2) {
        return -1;
    }
    else {
        memset(buf, 0, max);
        strcat(buf, " test ");
        counter++;
        return std::string(buf).size();
    }
}

class deferred_resource : public http_resource {
    public:
        const std::shared_ptr<http_response> render_GET(const http_request& req) {
            return std::shared_ptr<deferred_response<void> >(new deferred_response<void>(test_callback, nullptr, "cycle callback response"));
        }
};

// const std::map<std::string, std::string, http::header_comparator> get_headers() const{
    
// }

int main(int argc, char** argv) {
    webserver ws = create_webserver(8080)
        .start_method(httpserver::http::http_utils::INTERNAL_SELECT)
        .max_threads(2)
        .max_connections(2)
        .log_access(custom_access_log)
        .log_error(custom_access_log_error)
        .not_found_resource(not_found_custom)
        .method_not_allowed_resource(not_allowed_custom)
        .default_policy(http::http_utils::REJECT)
        .per_IP_connection_limit(2)
        .connection_timeout(7);

    vietpro__resource hwr;
    user_pass_resource upr;
    deferred_resource dre;
   


    ws.allow_ip("127.0.0.1");

    ws.register_resource("/", &hwr);
    ws.register_resource("/hello/hohoi/hihi", &hwr);
    ws.register_resource("/family", &hwr, true);
    ws.register_resource("/with_regex_[0-9]+", &hwr);

    ws.register_resource("/hihi", &hwr, true);

    ws.register_resource("/hihiae", &upr);
    ws.register_resource("/hihi123", &dre);
    ws.start(true);
        
        hwr.disallow_all();
			// Xóa tất cả các phương thức.
		 hwr.allow_all();
			// Sử dụng tất cả các phương thức
		 hwr.set_allowing("GET", true);
			// Thêm phương thức GET
		 ws.register_resource("/hello", &hwr); 
    
    return 0;
}

