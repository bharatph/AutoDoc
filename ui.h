//
//  ui.h
//  _UI_H
//
//  Created by Bharatvaj on 6/17/17.
//
//

#ifndef _UI_H
#define _UI_H "UI"

#include<FL/Fl.H>
#include<FL/Fl_Window.H>
#include<FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Menu_Bar.H>
#include "shell.h"
Fl_Window *root;
Fl_Multiline_Output *out_txt;

bool is_gui = false;

void gprintln(char *msg, ...){
    char *str = (char *)malloc(BUFFER_SIZE);
    va_list vl;
    va_start(vl, msg);
    vsprintf(str, msg, vl);
    va_end(vl);
    char *buffer = (char *)malloc(BUFFER_SIZE);
    sprintf(buffer, "%s\n", str);
    if(is_gui){
        out_txt->insert(buffer);
    }
    else {
        printf("%s", buffer);
    }
}

int start_shell(job jbs[], int jlen){
    int status = 0;
    while(status != 100){
        status = sh_next(jbs, jlen, "autodoc>");
        log_inf(_UI_H, "The process returned the status %d", status);
    }
    return status;
}

void root_exit(Fl_Widget *obj, void *opt){
    root->hide();
    log_inf(_UI_H, "closing window");
    exit(EXIT_SUCCESS);
}

int _jlen = 0; //tech dept
//TODO detect available commands and do the what has to be done with list
void btn_cb(Fl_Widget *obj, void *opt){
    job *jbs = (job *)opt;
    char *btn_name = (char *)malloc(BUFFER_SIZE);
    strcpy(btn_name, ((Fl_Button *)obj)->label());
    for (int i = 0; i < _jlen; i++) {
        if (strcmp(btn_name, jbs[i].command) == 0) {
            (jbs[i].function)(0, NULL); //FIXME correct args and count
        }
    }
}

Fl_Button *send_btn = NULL;

int resize(Fl_Widget *obj, void *opt){
	//send_btn.resize(root->windowWidth/2, 0, 45, 20);
}

int detect_display_and_set_max_and_min_size(){
	return -1;
}

//TODO load settings from a configuration file

#define LOG_TXT_SIZE 12
int start_gui(job jbs[], int jlen){
    send_btn = new Fl_Button(0, 0, 45, 20);
    root = new Fl_Window(0, 0, 256, 256, "AutoDoc");
    //TODO set max and min size for Fl_Window
    root->add(send_btn);
    root->callback(root_exit);
    log_inf(_UI_H, "GUI Starting...");
    root->show();
    return Fl::run();
}

int load_ui(job jbs[], int jlen, bool _is_gui = 1){
    //add checks for CLI or GUI from args
    	is_gui = _is_gui;
	if(!_is_gui /*|| screen not found*/){
        //TODO if screen not found, run in CLI mode
        /*
         if(screen not found){
            println("screen not found continuing with command line interface");
         }
         */
		start_shell(jbs, jlen);
	}
	else start_gui(jbs, jlen);
	return 0;
}
#endif /* ui_h */
