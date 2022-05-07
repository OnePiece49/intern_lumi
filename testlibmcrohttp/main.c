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


#define PORT 8888
#define FILENAME "picture.txt"
#define MIMETYPE "image/png"

int print_out_key (void *cls, enum MHD_ValueKind kind, 
                   const char *key, const char *value)
{
  char *hihi = (char *)cls;
  printf("kaka: %s\n", hihi);
  printf ("%s: %s\n", key, value);
  return MHD_YES;
}

struct MHD_Response* get_raw_response(char *content) {
        struct MHD_Response *response;
        response = MHD_create_response_from_buffer (strlen(content),
                                       (void *) content,
                                       MHD_RESPMEM_PERSISTENT);

            MHD_add_response_header(response, "hihihi", "it->second.c_str()");

        return response;
     }

static enum MHD_Result
answer_to_connection (void *cls, struct MHD_Connection *connection,
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

  // response =
  //   MHD_create_response_from_buffer (strlen (page), (void *) page,
  //                                    MHD_RESPMEM_PERSISTENT);

  response = get_raw_response(page);
     //MHD_add_response_header(response, "wibu", "hihi"); 
     //MHD_add_response_header(response, "wibu20", "hihi20");                                 
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}


int
main ()
{
  struct MHD_Daemon *daemon;
  struct MHD_OptionItem ops[] = {
          { MHD_OPTION_CONNECTION_LIMIT, 3, NULL },
          { MHD_OPTION_CONNECTION_TIMEOUT, 10, NULL },
          { MHD_OPTION_END, 0, NULL }
  };

  daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, PORT, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_ARRAY, ops ,MHD_OPTION_END);
  if (NULL == daemon)
    return 1;

  (void) getchar ();

  MHD_stop_daemon (daemon);

  return 0;
}