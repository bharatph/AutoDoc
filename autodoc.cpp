#ifndef REGENX
#define AUTODOC "AutoDOC"
#include<iostream>
#include<thread>
#if defined(__linux__) || defined(__APPLE__)
#include<unistd.h>
#include<signal.h>
#else
#include "windows.h"
#endif
#include <sqlite3.h>
#include"config.h"
#include"logc.h"
#include "network_utils.h"
#include "opencv_utils.h"
#include "ui.h"


int clifd = -1;


/* 0 on Successful connection to the server
 * 1 on Protocol Mismatch
 * 2 on Exhausted Retries..(Reasons: Server Down or The client is not connected to the internet)
 */
#define MAX_RETRIES 256
#define RETRY_TIME 2//in seconds
int check_server_and_connect(const char *server_addr){
    int i = 0;
    while(clifd < 0){
        if((clifd = connect_server(server_addr, DEFAULT_PORT)) < 0){
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
    log_inf(AUTODOC, "Connection to REGENX Server established succesfully");
    return 0;
}

#define REGENX_SERVER_ADDR "localhost"

void *connect_to_server(void *l){
    int status = 0;
    bool approval = true; //set from [user interface] by making this globally accesible or making it static [or both]
    while(1){
        if(clifd < 0 && approval){ //TODO use fcntl methods to check for socket alive
            status = check_server_and_connect(REGENX_SERVER_ADDR);
            if(status == 1){
                log_err(AUTODOC, "Protcol mismatch.. Either the Server [or Client] is outdated [or connection attempted on wrong server]");
                /* TODO give guidelines to the user to do the following things,
                 * Check the server address
                 * Check the client and server version and ask the user to update accordingly
                 */
            } else if(status == 2){
                log_inf(AUTODOC, "Connection to the REGENX Server timed out");
                //TODO callback to [user interface] to try to reconnect [or use the setting automatic retry]
            }
        }
        sleep(10);
    }
    return NULL;
}


char *info[] = {
    "dispaly this help message",
    "send image to the server for processing"
};

char *commands[] = {
    "help",
    "send"
};

int len = sizeof(commands)/sizeof(char *);

int help(char **args){
    for(int i = 0; i < len; i++){
        gprintln("%s\t- %s", commands[i], info[i]);
    }
    return 0;
}



int send_img(char **args){
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

int (*builtin_funcs[])(char **) = {
    &help,
    &send_img
};



int load_db(){
    return -1;
}

void exit_handler(int sig){
    log_inf(AUTODOC, "Closing down RegenX Client. Bye!");
    close(clifd); //Error checking on closing?
    exit(EXIT_SUCCESS);
}




int main(int argc, char *argv[]){
    signal(SIGINT, exit_handler);
	printf("%s", regenx_logo);    if(load_db() < 0){
		log_fat(AUTODOC, "Cannot load DB, terminating program");
    }
    //start network thread
    pthread_t net_thread;
    pthread_create(&net_thread, NULL, &connect_to_server, NULL);
    load_ui(argc, argv, builtin_funcs, commands, info, len);
    pthread_join(net_thread, NULL);
    return 0;;
}
#endif
