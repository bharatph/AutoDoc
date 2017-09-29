#ifndef AUTODOC_D
#define AUTODOC_D "AUTODOC_DAEMON"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <signal.h>

#include "config.h"
#include "clog.h"
#include "shell.h"
#include "network_utils.h"
#include"lp.h"

int help(int, char **);

int servfd = -1;
pthread_t net_thread;

int send_details(int clifd, char **args){
    //TODO
    //Step1: brace for impact...Receive binary file
    //Step2: process the received Mat file and scan for fractures
    //Step3: Check the database for knowledge about the fractures and return back info about that with prescription(based on the arguement given along with 'send')
    write(clifd, "[DUMMY]This guy is probably dead right now.....Just kidding, our servers are down. Please try again later", BUFFER_SIZE);
    return 0;
}

int quit(int clifd, char **args){
    writeln(clifd, "EXIT_ACK", BUFFER_SIZE);
    close(clifd);
    return 0;
}

job jobs[] = {
	{"help","displays this help message", help},
	{"send","send the details about the sent X-ray image", send_details},
	{"exit", "handle the request 'exit' for the client", quit}
};

int jlen = sizeof(jobs)/sizeof(job);

int help(int clifd, char **args){
	return sh_help(jlen, jobs);
}

void *handle_new_connection(void * sock_desc){
    int clifd = *((int *)sock_desc);
    //AUTODOC PROTOCOL
    //lp_init_server(clifd);//handle error
    int status = 0;
    while(status == 0){
        char *buffer = readln(clifd);
        status = sh_process(jlen, jobs, buffer);
    }
    return NULL;
}

void exit_handler(int sig){
    log_inf(AUTODOC_D, "Writing to DB before exiting...");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	std::ios_base::sync_with_stdio(false);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, exit_handler);
    /*
	pid_t pid;
	pid = fork();
	if(pid < 0){
		log_err(REGENX_D, "Cannot start daemon");
		return -1;
	}
	else if(pid > 0){
		log_inf(REGENX_D, "Daemon started successfully");
		return 0;
	}
     */
    int clifd = -1;
    while((clifd = start_server(DEFAULT_PORT)) > 0){
        log_inf(AUTODOC_D, "Connection accepted");
        //handle_new_connection(clifd);
        int *new_sockfd =  &clifd;
        pthread_create(&net_thread, NULL, &handle_new_connection, (void *)new_sockfd);
    }
    pthread_join(net_thread, NULL);
	return 0;
}
#endif
