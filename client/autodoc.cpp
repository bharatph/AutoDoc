#ifndef REGENX
#define AUTODOC "AutoDoc"
#include<iostream>
#include<thread>
#if defined(__linux__) || defined(__APPLE__)
#include<unistd.h>
#include<signal.h>
#else
#include "windows.h"
#endif
#include <sqlite3.h>
#define ENABLE_COLOR
#include"config.h"
extern "C" {
#include <clog/clog.h>
}
#include <Node/Node.hpp>
//#include "opencv_utils.h"
#include"shell.h"
#include "ui.h"

int help(int, char **);
int clifd = -1;


/* 0 on Successful connection to the server
 * 1 on Protocol Mismatch
 * 2 on Exhausted Retries..(Reasons: Server Down or The client is not connected to the internet)
 */
#define MAX_RETRIES 256
#define RETRY_TIME 2//in seconds
node::Node serv;
int check_server_and_connect(const char *server_addr){
    int i = 0;
    while(clifd < 0){
        if((clifd = serv.connect(server_addr, DEFAULT_PORT)) < 0){
            log_err(AUTODOC, "Cannot connect to server.. retrying");
            if(i++ == MAX_RETRIES){
                return 2;
            }
            sleep(RETRY_TIME); //add to config.h
            continue;
        }
        break;
    }
    char *buffer = (char *)malloc(BUFFER_SIZE);
    write(clifd, "ACK", BUFFER_SIZE);
    read(clifd, buffer, BUFFER_SIZE);
    log_inf(AUTODOC, "Sending: ACK, Received: %s, \n", buffer);
    if(strcmp(buffer, "ACK_RECVD") < 0){
        log_err(AUTODOC, "Protocol mismatch...or [try updating REGENX Client]");
        close(clifd);
        return 1;
    }
    log_inf(AUTODOC, "Connection to AutoDoc Server established succesfully");
    return 0;
}

#define AUTODOC_SERVER_ADDR "localhost"

void *connect_to_server(void *l){
    int status = 0;
    bool approval = true; //set from [user interface] by making this globally accesible or making it static [or both]
    while(1){
        if(clifd < 0 && approval){ //TODO use fcntl methods to check for socket alive
            status = check_server_and_connect(AUTODOC_SERVER_ADDR);
            if(status == 1){
                log_err(AUTODOC, "Protcol mismatch.. Either the Server [or Client] is outdated [or connection attempted on wrong server]");
                /* TODO give guidelines to the user to do the following things,
                 * Check the server address
                 * Check the client and server version and ask the user to update accordingly
                 */
            } else if(status == 2){
                log_inf(AUTODOC, "Connection to the AutoDoc Server timed out");
                //TODO callback to [user interface] to try to reconnect [or use the setting automatic retry]
            }
        }
        sleep(10);
    }
    return NULL;
}
int send_data(int count, char **args){
    write(clifd, "send", BUFFER_SIZE); //TODO check the process thoroughly
    /*
    matwrite("tmp.raw", cv::Mat());
    FILE *fp = fopen("tmp.raw", "rb+");
    file_to_socket(fp, clifd);
     */
    char *buffer = (char *)malloc(BUFFER_SIZE);
    read(clifd, buffer, BUFFER_SIZE);
    gprintln("DIAGNOSTICS: %s", buffer);
    return -1;
}

int list(int count, char **args){
	//print send buffer with info
	return -1;
}

int remove(int count, char **args){
	return -1;
}

void exit_handler(int sig){
    log_inf(AUTODOC, "Closing down AutoDoc Client. Bye!");
    close(clifd); //Error checking on closing?
    exit(EXIT_SUCCESS);
}

int exit_self(int count, char **args){
	exit_handler(SIGINT);
	return 0;
}

job jobs[] = {
	{"help","dispaly this help message", help},
	{"ls", "lists the items in inventory with status", list},
	{"rm", "removes an item from the inventory", remove},
	{"send", "send image to the server for processing", send_data},
	{"exit", "exits the client", exit_self}
};

int jlen = sizeof(jobs)/sizeof(job);

int help(int count, char **args){
    return sh_help(jlen, jobs);
}

int load_db(){
    return -1;
}

int main(int argc, char *argv[]){
    signal(SIGINT, exit_handler);
	printf("%s", autodoc_logo);
	if(load_db() < 0){
	}
    //start network thread
    if(argc < 2){
    	load_ui(jobs, jlen, true);
    }else load_ui(jobs, jlen);
    pthread_t net_thread;
    pthread_create(&net_thread, NULL, &connect_to_server, NULL);
    pthread_join(net_thread, NULL);
    return 0;;
}
#endif
