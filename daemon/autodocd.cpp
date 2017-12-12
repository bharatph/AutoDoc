#ifndef AUTODOC_D
#define AUTODOC_D "AUTODOC_DAEMON"
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
extern "C" {
#include <clog/clog.h>
}
#include <Node/Node.hpp>

#include "config.h"
#include<lp.h>
#include "shell.h"

#define PROTOCOL_MISMATCH "Protocol is Mismatched, upgrade the server or the client"

int help(node::Node *, int, char **);

int send_details(node::Node *n, int count, char **args) {
  // TODO
  // Step1: brace for impact...Receive binary file
  // Step2: process the received Mat file and scan for fractures
  // Step3: Check the database for knowledge about the fractures and return back
  // info about that with prescription(based on the arguement given along with
  // 'send')
  n->writeln("[DUMMY]Our servers are down. Please try again later");
  return 0;
}

int quit(node::Node *n, int count, char **args) {
  n->writeln("EXIT_ACK");
  n->close();
  return 0;
}

job jobs[] = {
    {"help", "displays this help message", help},
    {"send", "send the details about the sent X-ray image", send_details},
    {"exit", "handle the request 'exit' for the client", quit}};

int jlen = sizeof(jobs) / sizeof(job);

int help(node::Node *n, int count, char **args) { return sh_help(jlen, jobs); }

void exit_handler(int sig) {
  log_inf(AUTODOC_D, "Writing to DB before exiting...");
  exit(EXIT_SUCCESS);
}


int handle_new_connection(Node *cli){
  if(!lp_init_session(cli)){
    fprintf(stderr, "%s\n", PROTOCOL_MISMATCH);
    return -1;
  }
  int status = 0;
  while (status == 0) {
    char *buffer = cli->readln();
    status = sh_process(jlen, jobs, cli, buffer);
  };
  return status;
}
int main(int argc, char *argv[]) {
  std::ios_base::sync_with_stdio(false);
  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, exit_handler);
  log_inf(AUTODOC_D, "Starting AutoDoc server");
    node::Node *n = new node::Node();
    while (node::Node *cli = n->accept(DEFAULT_PORT)) {
      log_inf(AUTODOC_D, "Connection accepted");
        pid_t pid = fork();
        if (pid < 0) {
          log_err(AUTODOC_D, "Cannot start handler");
          //cannot start subprocess, continue to listen anyway
          //TODO make server handle out_of_memory coditions
          return -1;
        } else if (pid > 0) {
          // AUTODOC PROTOCOL
          // lp_init_server(clifd);//handle error
          log_inf(AUTODOC_D, "Handler started successfully");
          handle_new_connection(cli);
        }
    }
  return 0;
}
#endif
