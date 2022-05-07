Vd1:
        #include <httpserver.hpp>

        using namespace httpserver;

        class hello_world_resource : public http_resource {
        public:
            const std::shared_ptr<http_response> render(const http_request&) {
                return std::shared_ptr<http_response>(new string_response("Hello, World!"));
            }
        };

        int main(int argc, char** argv) {
            webserver ws = create_webserver(8080);

            hello_world_resource hwr;
            ws.register_resource("/hello", &hwr);
            ws.start(true);
            
            return 0;
        }
    
    *Các tham số quan trọng:
        +, webserver: Represents the daemon listening on a socket for HTTP traffic.
            +, create_webserver: Builder class to support the creation of a webserver.
        +, http_resource: Represents the resource associated with a specific http endpoint.
        +, http_request: Represents the request received by the resource that process it.
        +, http_response: Represents the response sent by the server once the resource finished its work.
            +, string_response: A simple string response.
            +, file_response: A response getting content from a file.
            +, basic_auth_fail_response: A failure in basic authentication.
            +, digest_auth_fail_response: A failure in digest authentication.
            +, deferred_response: A response getting content from a callback.

    +, register_resource(const std::string& resource, http_resource* res, bool family = false);
        Method used to register a resource with the webserver.
      * @param resource The url pointing to the resource. This url could be also parametrized in the form /path/to/url/{par1}/and/{par2}
      *                 or a regular expression.
      * @param http_resource http_resource pointer to register.
      * @param family boolean indicating whether the resource is registered for the endpoint and its child or not.
      * @return true if the resource was registered