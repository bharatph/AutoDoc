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

int (**_funcs)(char **);
char **_info;
char **_commands;
int _len = 0;

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

int start_shell(){
    int status = 0;
    while(status != 100){
        status = sh_next(_funcs, _commands, _info, _len, "autoDOC>");
        log_inf(_UI_H, "The process returned the status %d", status);
    }
    return status;
}

void root_cb(Fl_Widget *obj, void *opt){
    root->hide();
    log_inf(_UI_H, "closing window");
    exit(EXIT_SUCCESS);
}

void hide_cb(Fl_Widget *obj, void *opt){
    log_inf(_UI_H, "Help Window closed");
    ((Fl_Window *)obj)->hide();
}

void menu_cb(Fl_Widget *obj, void*opt){
    char *menu_name = (char *)malloc(BUFFER_SIZE);
    //strcpy(menu_name, ((Fl_Menu_Bar *)obj)->label());
    
    //if(strcmp(menu_name, "help") == 0){
        Fl_Window *help_win = new Fl_Window(0,0, 128, 128, "Help");
        help_win->callback(menu_cb);
    help_win->show();
        root->add(help_win);
    //}
}

void btn_cb(Fl_Widget *obj, void *opt){
    char *btn_name = (char *)malloc(BUFFER_SIZE);
    strcpy(btn_name, ((Fl_Button *)obj)->label());
    for (int i = 0; i < _len; i++) {
        if (strcmp(btn_name, _commands[i]) == 0) {
            ((_funcs)[i])(NULL); //FIXME correct args
        }
    }
}

#define BTN_H 20
#define BTN_W 45
#define LOG_TXT_SIZE 12
int start_gui(){
    int y=0;
    root = new Fl_Window(0, 0, 256, 256, "AutoDOC");
    out_txt = new Fl_Multiline_Output(0, y, 256, 256, "LOG");
    Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0, 256, 25);
    menu->add("Help", FL_CTRL+'h', menu_cb);
    root->add(menu);
    for(int i =0; i<_len; i++){
        y += BTN_H;
        Fl_Button *btn = new Fl_Button(0,y,BTN_W, BTN_H, _commands[i]);
        btn->callback((Fl_Callback *)btn_cb);
        root->add(btn);
    }
    y += BTN_H;
    out_txt->Fl_Widget::resize(0, y, 256, 256);
    root->add(out_txt);
    root->callback((Fl_Callback *)root_cb);
    log_inf(_UI_H, "GUI Starting...");
    root->show();
    return Fl::run();
}

int load_ui(int argc, char *argv[], int (*__sh_funcs[])(char **), char **__commands, char **__info, int __len){
    _funcs = __sh_funcs;
    _commands = __commands;
    _info = __info;
    _len = __len;
    //add checks for CLI or GUI from args
    if(argc > 1 /*|| screen not found*/){
        //TODO if screen not found, run in CLI mode
        /*
         if(screen not found){
            println("screen not found continuing with command line interface");
         }
         */
        is_gui = false;
        if(strchr(argv[1], 'c') >= 0) start_shell();
    }
    else {
        is_gui = true;
        start_gui();
    }
    return 0;
}
#endif /* ui_h */
