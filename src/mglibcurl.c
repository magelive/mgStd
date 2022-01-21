#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "mainloop.h"

void check_multi_info(CURLM *multi) {
    char *done_url;
    CURLMsg *msg;
    int pending;
    char *szUrl;
    int http_status_code;

    while ((msg = curl_multi_info_read(multi, &pending))) {
         if (msg->msg == CURLMSG_DONE) {

            if(msg->data.result != CURLE_OK) {
                fprintf(stderr, "CURL error code: %d\n", msg->data.result);
                continue;
            }

            // Get HTTP status code
            http_status_code=0;
            szUrl = NULL;

            curl_easy_getinfo(msg->easy_handle, CURLINFO_RESPONSE_CODE, &http_status_code);
            curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &szUrl);

            if(http_status_code==200) {
                printf("200 OK for %s\n", szUrl);
            } else {
                fprintf(stderr, "GET of %s returned http status code %d\n", szUrl, http_status_code);
            }

            curl_multi_remove_handle(multi, msg->easy_handle);
            curl_easy_cleanup(msg->easy_handle);
        }
        else {
            fprintf(stderr, "error: after curl_multi_info_read(), CURLMsg=%d\n", msg->msg);
        }
        /*
        switch (message->msg) {
        case CURLMSG_DONE:
            curl_easy_getinfo(message->easy_handle, CURLINFO_EFFECTIVE_URL,
                            &done_url);
            printf("%s DONE\n", done_url);

            curl_multi_remove_handle(curl_handle, message->easy_handle);
            curl_easy_cleanup(message->easy_handle);
            break;

        default:
            fprintf(stderr, "CURLMSG default\n");
            abort();
        }
        */
    }
}

void event_cb(int fd, events_t events, void *user_data)
{
    //printf("events .....%d\n", events);

    CURLM *multi = (CURLM *)user_data;
    int flags = 0;
    if (events & EVENT_ERR) {
        flags = CURL_CSELECT_ERR;
    } else {
        if (events & EVENT_IN) {
            flags |= CURL_CSELECT_IN;
        }

        if (events & EVENT_OUT) {
            flags |= CURL_CSELECT_OUT;
        }
    }

    int running_handles;
    curl_multi_socket_action(multi, fd, flags, &running_handles);  
    check_multi_info(multi); 

    return;
}

int handle_socket(CURL *easy, curl_socket_t s, int action, void *userp, void *socketp)
{
    //printf("AAAAAAAAAAAAAA handle socket..\n");
    if (action == CURL_POLL_REMOVE) {
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>remove fd..\n");
        mainloop_remove_fd(s);
        printf("eeeeeeeeeeeeeeeeeeeeeeee>>remove fd..\n");
        //mainloop_quit();
        return 0;
    }
    CURLM *multi = (CURLM *)userp;

    events_t event = EVENT_ERR;
    switch(action) {
        case CURL_POLL_IN:
            event |= EVENT_IN;
        break;
        case CURL_POLL_OUT:
            event |= EVENT_OUT;
            break;
        case CURL_POLL_INOUT:
            event |= (EVENT_IN | EVENT_OUT);
            break;
    }

    if (!socketp) {
        printf("Add fd....\n");
        mainloop_add_fd(s, event, event_cb, NULL, userp);
        curl_multi_assign(multi, s, &s);
        return 0;
    } 

    mainloop_modify_fd(s, event);
    return 0;
}

static void timer_cb(int fd, void *user_data)
{

    CURLMcode rc;
    CURLM *multi = (CURLM *)user_data;
    int still_running;
    rc = curl_multi_socket_action(multi,
                                    CURL_SOCKET_TIMEOUT, 0, &still_running);
    //printf("timer_cb: curl_multi_socket_action rc = %d\n", rc);
    check_multi_info(multi);
}

static int multi_timer_cb(CURLM *multi, long timeout_ms, void *usrp)
{
  /*
   * if timeout_ms is -1, just delete the timer
   *
   * For all other values of timeout_ms, this should set or *update* the timer
   * to the new value
   */

    if(timeout_ms == -1)
        mainloop_remove_timeout(*(int *)usrp);
    else /* includes timeout zero */
        mainloop_modify_timeout(*(int *)usrp, timeout_ms);

    return 0;
}


static size_t write_cb(char *d, size_t n, size_t l, void *p)
{
  /* take care of the data here, ignored in this example */
  fprintf(stdout, "Write callback >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  fprintf(stdout, "%s\n", d);
  (void)d;
  (void)p;
  return n*l;
}

static int prog_cb(void *p, double dltotal, double dlnow, double ult,
                   double uln)
{
  char *url = (char *)p;
  (void)ult;
  (void)uln;
 
  fprintf(stdout, "Progress: %s (%g/%g)\n", url, dlnow, dltotal);
  return 0;
}

static const char *urls[] = {
  "www.baidu.com",
  "https://www.google.com",
  "android.com",
  "test.com"
};

int main(int argc, char **argv) 
{
    int i;
    mainloop_init();
    curl_global_init(CURL_GLOBAL_ALL);    
    CURLM *multi = curl_multi_init();
    int tfd = mainloop_add_timeout(1000, timer_cb, NULL, multi);

    curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, handle_socket);
    curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, multi);
    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, multi_timer_cb);
    curl_multi_setopt(multi, CURLMOPT_TIMERDATA, &tfd);

    for (i = 0; i < sizeof(urls)/sizeof(char *); ++i) {
        printf("....add usrl: %s\n", urls[i]);
        CURL *eh = curl_easy_init();
        curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
        curl_easy_setopt(eh, CURLOPT_URL, urls[i]);
        curl_easy_setopt(eh, CURLOPT_PRIVATE, urls[i]);
        curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(eh, CURLOPT_PROGRESSFUNCTION, prog_cb);
        curl_easy_setopt(eh, CURLOPT_PROGRESSDATA, urls[i]);
        //curl_easy_setopt(eh, CURLOPT_WRITEDATA, conn);
        int rc = curl_multi_add_handle(multi, eh);
        printf("Add url rc = %d\n", rc);
    }
    
    /*
    int still_running;
    int rc = curl_multi_socket_action(multi,
                                  CURL_SOCKET_TIMEOUT, 0, &still_running);
    //printf("AAAAAAA rc = %d, still_running = %d\n", rc, still_running);
    */
    mainloop_run();
    curl_multi_cleanup(multi);
    return 0;
}