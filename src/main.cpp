#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <thread>
#include <iostream>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/un.h>
#include <curl/curl.h>

 
static const char *urls[] = {
  "https://www.microsoft.com",
  "https://opensource.org",
  "https://www.google.com",
  "https://www.yahoo.com",
  "https://www.ibm.com",
  "https://www.mysql.com",
  "https://www.oracle.com",
  "https://www.ripe.net",
  "https://www.iana.org",
  "https://www.amazon.com",
  "https://www.netcraft.com",
  "https://www.heise.de",
  "https://www.chip.de",
  "https://www.ca.com",
  "https://www.cnet.com",
  "https://www.mozilla.org",
  "https://www.cnn.com",
  "https://www.wikipedia.org",
  "https://www.dell.com",
  "https://www.hp.com",
  "https://www.cert.org",
  "https://www.mit.edu",
  "https://www.nist.gov",
  "https://www.ebay.com",
  "https://www.playstation.com",
  "https://www.uefa.com",
  "https://www.ieee.org",
  "https://www.apple.com",
  "https://www.symantec.com",
  "https://www.zdnet.com",
  "https://www.fujitsu.com/global/",
  "https://www.supermicro.com",
  "https://www.hotmail.com",
  "https://www.ietf.org",
  "https://www.bbc.co.uk",
  "https://news.google.com",
  "https://www.foxnews.com",
  "https://www.msn.com",
  "https://www.wired.com",
  "https://www.sky.com",
  "https://www.usatoday.com",
  "https://www.cbs.com",
  "https://www.nbc.com/",
  "https://slashdot.org",
  "https://www.informationweek.com",
  "https://apache.org",
  "https://www.un.org",
};

#define SERVER_PATH     "/tmp/server.dat"
#define BUFFER_LENGTH    250
#define FALSE              0
#define MAX_PARALLEL 10  
#define NUM_URLS sizeof(urls)/sizeof(char *)

using namespace std;

void test(int arg1){
  for (int index = 0; index < arg1; index++)  {
    cout << "This is: " <<index  << "\n";
  }
}

static size_t write_cb(char *data, size_t n, size_t l, void *userp)
{
  /* take care of the data here, ignored in this example */ 
  (void)data;
  (void)userp;
  return n*l;
}
 
static void add_transfer(CURLM *cm, int i)
{
  CURL *eh = curl_easy_init();
  curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(eh, CURLOPT_URL, urls[i]);
  curl_easy_setopt(eh, CURLOPT_PRIVATE, urls[i]);
  curl_multi_add_handle(cm, eh);
}
 

class threadObj{
  public: 
    void operator()(int x){
      for (int i =0; i< x; i++){
        cout <<i;
      }
    }
};
int main(int argc, char **argv)
{
  cout<<"---Start" <<"\n";
  int    sd=-1, sd2=-1;
  int    rc, length;
  char   buffer[BUFFER_LENGTH];
  struct sockaddr_un serveraddr;
  

  CURLM *cm;
  CURLMsg *msg;
  unsigned int transfers = 0;
  int msgs_left = -1;
  int still_alive = 1;
 
  curl_global_init(CURL_GLOBAL_ALL);
  cm = curl_multi_init();

/* Limit the amount of simultaneous connections curl should allow: */ 
  curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)MAX_PARALLEL);
 
  for(transfers = 0; transfers < MAX_PARALLEL; transfers++)
    add_transfer(cm, transfers);
 
  do {
    curl_multi_perform(cm, &still_alive);
 
    while((msg = curl_multi_info_read(cm, &msgs_left))) {
      if(msg->msg == CURLMSG_DONE) {
        char *url;
        CURL *e = msg->easy_handle;
        curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
        fprintf(stderr, "R: %d - %s <%s>\n",
                msg->data.result, curl_easy_strerror(msg->data.result), url);
        curl_multi_remove_handle(cm, e);
        curl_easy_cleanup(e);
      }
      else {
        fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
      }
      if(transfers < NUM_URLS)
        add_transfer(cm, transfers++);
    }
    if(still_alive)
      curl_multi_wait(cm, NULL, 0, 1000, NULL);
 
  } while(still_alive || (transfers < NUM_URLS));
 
  curl_multi_cleanup(cm);
  curl_global_cleanup();
  
  do{

  }while(FALSE);
  if (sd != -1) close(sd);
  if (sd2 != -1) close(sd2);
  unlink(SERVER_PATH);
  cout<<"truongdeptrai" <<"\n";
  return 0;
}
