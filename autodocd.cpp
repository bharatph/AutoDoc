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
#include"adp.h"

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
    write(clifd, "EXIT_ACK", BUFFER_SIZE);
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
	return sh_help(jobs, jlen);
}

void *handle_new_connection(void * sock_desc){
    int clifd = *((int *)sock_desc);
    //AUTODOC PROTOCOL
    adp_init_server(clifd);//handle error
    char *buffer = (char *)malloc(BUFFER_SIZE);
    int status = 0;
    while(status == 0){
        read(clifd, buffer, BUFFER_SIZE);
        status = sh_process(jobs, jlen, buffer);
    }
    return NULL;
}

int initialize_server(int port){
    //Create socket
    servfd = socket(PF_INET , SOCK_STREAM , 0);
    if (servfd == -1){
        log_err(AUTODOC_D, "could not create socket");
        return -1;
    }
    
    struct sockaddr_in server;
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
    //Bind
    if( bind(servfd,(struct sockaddr *)&server , sizeof(server)) < 0){
        log_err(AUTODOC_D, "bind failed");
        return -1;
    }
    //Listen
    listen(servfd , SERV_BACKLOG);
    //Accept and incoming connection
    log_inf(AUTODOC_D, "Waiting for incoming connections...");
    return servfd;
}

void exit_handler(int sig){
    log_inf(AUTODOC_D, "Writing to DB before exiting...");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	std::ios_base::sync_with_stdio(false);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, exit_handler);
    init_clog(); //for mutex locks
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
    if((servfd = initialize_server(DEFAULT_PORT)) < 0){
        log_fat(AUTODOC_D, "Cannot initialize server");
        return -1;
    }
    log_inf(AUTODOC_D, "Server Initialized successfully");
    int *new_sockfd;
    struct sockaddr_in client;
    socklen_t cli_size = sizeof(struct sockaddr_in);
    //accept connection from an incoming client
    int clifd = -1;
    while((clifd = accept(servfd, (struct sockaddr *)&client, &cli_size)) > 0){
        log_inf(AUTODOC_D, "Connection accepted");
        //handle_new_connection(clifd);
        new_sockfd = (int *)malloc(sizeof(int));
        new_sockfd =  &clifd;
        pthread_create(&net_thread, NULL, &handle_new_connection, (void *)new_sockfd);
    }
    pthread_join(net_thread, NULL);
	return 0;
}
#endif
