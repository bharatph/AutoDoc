#ifndef AUTODOC_D
#define AUTODOC_D "AUTODOC_DAEMON"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <signal.h>

#include "config.h"
#include "logc.h"
#include "shell.h"
#include "network_utils.h"

int servfd = -1;
pthread_t net_thread;
char *commands[] = {
    "help",
    "send",
    "exit"
};

char *info[] = {
    "displays this help message",
    "send the details about the sent X-ray image",
    "handle the request 'exit' for the client"
};

int help(char **args, int clifd){
    int len = sizeof(commands)/sizeof(char *);
    char *buffer = (char *)malloc(BUFFER_SIZE * len);
    for(int i = 0 ; i < len; i++){
        char tmp[BUFFER_SIZE];
        sprintf(tmp, "%s\t- %s\n",commands[i], info[i] );
        strcat(buffer, tmp);
    }
    write(clifd, buffer, BUFFER_SIZE *len);
    return 0;
}

int send_details(char **args, int clifd){
    //TODO
    //Step1: brace for impact...Receive binary file
    //Step2: process the received Mat file and scan for fractures
    //Step3: Check the database for knowledge about the fractures and return back info about that with prescription(based on the arguement given along with 'send')
    write(clifd, "[DUMMY]This guy is probably dead right now.....Just kidding, our servers are down. Please try again later", BUFFER_SIZE);
    return 0;
}

int quit(char **args, int clifd){
    write(clifd, "EXIT_ACK", BUFFER_SIZE);
    close(clifd);
    return 0;
}

int (*builtin_funcs[])(char **, int) = {
    help,
    send_details,
    quit
};

int process(char *command, int clifd){
    int len = sizeof(commands)/sizeof(char *);
    int status = -1;
    char **args = shSplit(command);
    if (*args == NULL) {
        // An empty command was entered.
        return status;
    }
    for (int i = 0; i < len; i++) {
        if (strcmp(*args, commands[i]) == 0) {
            log_inf(AUTODOC_D, *args);
            return (*(builtin_funcs)[i])(args, clifd);
        }
    }
    return status;
}

void *handle_new_connection(void * sock_desc){
    int clifd = *((int *)sock_desc);
    //REGENX PROTOCOL
    char *buffer = (char *)malloc(BUFFER_SIZE);
    read(clifd, buffer, BUFFER_SIZE);
    if(strcmp(buffer, "ACK") < 0){
        write(clifd, "protocol mismatch", BUFFER_SIZE);
        close(clifd);
        return NULL;
    }
    write(clifd, "ACK_RECVD", BUFFER_SIZE);
    int status = 0;
    while(status == 0){
        read(clifd, buffer, BUFFER_SIZE);
        status = process(buffer, clifd);
    }
    return NULL;
}

int initialize_server(int port){
    //Create socket
    servfd = socket(PF_INET , SOCK_STREAM , 0);
    if (servfd == -1){
        log_err(CLIENT_H, "could not create socket");
        return -1;
    }
    
    struct sockaddr_in server;
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
    //Bind
    if( bind(servfd,(struct sockaddr *)&server , sizeof(server)) < 0){
        log_err(CLIENT_H, "bind failed");
        return -1;
    }
    //Listen
    listen(servfd , SERV_BACKLOG);
    //Accept and incoming connection
    log_inf(CLIENT_H, "Waiting for incoming connections...");
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
    init_log(); //for mutex locks
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
        log_inf(CLIENT_H, "Connection accepted");
        //handle_new_connection(clifd);
        new_sockfd = (int *)malloc(sizeof(int));
        new_sockfd =  &clifd;
        pthread_create(&net_thread, NULL, &handle_new_connection, (void *)new_sockfd);
    }
    pthread_join(net_thread, NULL);
	return 0;
}
#endif
