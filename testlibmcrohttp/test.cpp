#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <string.h>
#include <microhttpd.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;
#define PORT 8888



class wibu {
    public:
        void say_hihi() {
            cout << "hihi ae nhe" << endl;
            cout << ae_wibu << endl;
            ae_wibu = " hello ae wwibu nhe\n";
        }

        void pass_argument() {
            struct MHD_Daemon *daemon;
            daemon = MHD_start_daemon (MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD,
                             PORT, NULL, NULL,
                             &answer_to_connection, this, MHD_OPTION_END);
        }

    private:
        static MHD_Result
            answer_to_connection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls);
        
        string ae_wibu = "kaka";


};

MHD_Result
wibu:: answer_to_connection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
  const char *page = "<html><body>Hello, browser!</body></html>";
  struct MHD_Response *response;
  enum MHD_Result ret;
  (void) cls;               /* Unused. Silent compiler warning. */
  (void) url;               /* Unused. Silent compiler warning. */
  (void) method;            /* Unused. Silent compiler warning. */
  (void) version;           /* Unused. Silent compiler warning. */
  (void) upload_data;       /* Unused. Silent compiler warning. */
  (void) upload_data_size;  /* Unused. Silent compiler warning. */
  (void) con_cls;           /* Unused. Silent compiler warning. */

  response =
    MHD_create_response_from_buffer (strlen (page), (void *) page,
                                     MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  static_cast<wibu*>(cls)->say_hihi();
  MHD_destroy_response (response);

  return ret;
}

int main (void)
{
    wibu Viet;
    Viet.pass_argument();
    getchar();
}


