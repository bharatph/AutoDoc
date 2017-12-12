#ifndef REGENX
#define AUTODOC "AutoDoc"
#include <iostream>
#include <thread>
#if defined(__linux__) || defined(__APPLE__)
#include <signal.h>
#include <unistd.h>
#else
#include "windows.h"
#endif
#include <sqlite3.h>
#define ENABLE_COLOR
#include "config.h"
extern "C" {
#include <clog/clog.h>
}
#include <Node/Node.hpp>
//#include "opencv_utils.h"
#include "shell.h"
#include <FLTKUI.hpp>
#include <UniversalUI.hpp>

using namespace uui;
using namespace node;

/* 0 on Successful connection to the server
 * 1 on Protocol Mismatch
 * 2 on Exhausted Retries..(Reasons: Server Down or The client is not connected
 * to the internet)
 */

#define AUTODOC_SERVER_ADDR "localhost"
#define MAX_RETRIES 20
#define RETRY_TIME 2 // in seconds
int connect_to_server(UI *ui, void *obj) {
  Node *n = (Node *)obj;
  int i = 0;
  while (n->connect(AUTODOC_SERVER_ADDR, DEFAULT_PORT) <
         0) { // TODO use fcntl methods to check for socket alive or implement
              // inside node library
    log_err(AUTODOC, "Cannot connect to server.. retrying");
    if (i++ == MAX_RETRIES) {
      ui->error("Connection Timeout to server");
      return -2;
    }
    sleep(RETRY_TIME); // add to config.h
  }
  n->writeln("ACK");
  std::string buffer = n->readln();
  log_inf(AUTODOC, "Sending: ACK, Received: %s, \n", buffer.c_str());
  if (buffer != "ACK_RECVD") {
    log_err(AUTODOC, "Protcol mismatch.. Either the Server [or Client] is "
                     "outdated [or connection attempted on wrong server]");
    // close socket
    ui->error("Protcol mismatch.. Either the Server [or Client] is "
              "outdated [or connection attempted on wrong server]");
    // TODO callback to [user interface] to try to reconnect [or use the
    // setting automatic retry]
    /* TODO give guidelines to the user to do the following things,
     * Check the server address
     * Check the client and server version and ask the user to update
     * accordingly
     */
    return -1;
  }
  log_inf(AUTODOC, "Connection to AutoDoc Server established succesfully");
  return 0;
}

int send_data(UI *ui, void *obj) {
  Node *n = (Node *)obj;
  n->writeln("send"); // TODO check the process thoroughly
  /*
  matwrite("tmp.raw", cv::Mat());
  FILE *fp = fopen("tmp.raw", "rb+");
  file_to_socket(fp, clifd);
   */
  std::string buffer = n->readln();
  ui->error("DIAGNOSTICS: " + buffer);
  return -1;
}

int list(int count, char **args) {
  // print send buffer with info
  return -1;
}

int remove(int count, char **args) { return -1; }

void exit_handler(int sig) {
  log_inf(AUTODOC, "Closing down AutoDoc Client. Bye!");
  // Error checking on closing?
  exit(EXIT_SUCCESS);
}

int exit_self(int count, char **args) {
  exit_handler(SIGINT);
  return 0;
}

/*
job jobs[] = {{"help", "dispaly this help message", help},
              {"ls", "lists the items in inventory with status", list},
              {"rm", "removes an item from the inventory", remove},
              {"send", "send image to the server for processing", send_data},
              {"exit", "exits the client", exit_self}};

int jlen = sizeof(jobs) / sizeof(job);

int help(int count, char **args) { return sh_help(jlen, jobs); }
*/

int load_db() { return -1; }

int main(int argc, char *argv[]) {
  //  signal(SIGINT, exit_handler);
  // printf("%s", autodoc_logo);
  UI *ui = new FLTKUI();
  if (load_db() < 0) {
  }
  Node *n = new Node();
  ui->set("connect", new std::vector<string>{"Connect to server"},
          connect_to_server);
  ui->set("send", new std::vector<string>{"Send data to server"}, send_data);
  Fl::run();
  ui->run("connect", n);
  ui->run("send", n);
  /*
  // start network thread
  if (argc < 2) {
    load_ui(jobs, jlen, true);
  } else
    load_ui(jobs, jlen);
  pthread_t net_thread;
  pthread_create(&net_thread, NULL, &connect_to_server, NULL);
  pthread_join(net_thread, NULL);
  */
  return 0;
}
#endif
