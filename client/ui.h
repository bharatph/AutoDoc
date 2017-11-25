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
#include<FL/Fl_Double_Window.H>
#include<FL/Fl_Button.H>
#include<FL/Fl_Group.H>
#include<FL/Fl_PNG_Image.H>
#include<FL/Fl_Box.H>
#include<FL/fl_draw.H>
#include<FL/Fl_Table_Row.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Menu_Bar.H>
#include "shell.h"
#include "layout.h"
Fl_Double_Window *root;
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

void send_medical_data(Fl_Button *btn, void *arg){
	gprintln("Sent");
}

void root_exit(Fl_Widget *obj, void *opt){
    root->hide();
    log_inf(_UI_H, "closing window");
    exit(EXIT_SUCCESS);
}

class Auto_Fl_Table : public Fl_Table_Row {
protected:
	void draw_cell(TableContext context, int R =0, int C=0, int X=0, int Y=0, int W=0, int H=0){
		static char s[40];
		sprintf(s, "%d/%d", R, C);
		switch(context){
			case CONTEXT_STARTPAGE:
				fl_font(FL_HELVETICA, 16);
				return;
			case CONTEXT_ROW_HEADER:
			case CONTEXT_COL_HEADER:
				fl_push_clip(X, Y, W, H);
				{
					fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, color());
					fl_color(FL_BLACK);
					fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
				}
				fl_pop_clip();
				return;
			case CONTEXT_CELL:
				fl_push_clip(X, Y, W, H);
				{
					fl_color(row_selected(R) ? selection_color() : FL_WHITE);
					fl_rectf(X, Y, W, H);
					fl_color(FL_BLACK);
					fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);

					fl_color(FL_LIGHT2);
					fl_rect(X, Y, W, H);
				}
				fl_pop_clip();
				return;
			default:
				return;
		}
	}
public:
	Auto_Fl_Table(int x, int y, int w, int h, const char *l=0):Fl_Table_Row(x,y,w,h,l){
		end();
	}
		~Auto_Fl_Table(){
		}
};

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
	return -1;
}

int detect_display_and_set_max_and_min_size(){
	return -1;
}

//TODO load settings from a configuration file


Fl_Group *main_view(){
	Fl_Group *grp = new Fl_Group(0,0, 256, 256);
	Fl_Box *drop_box = new Fl_Box(0,0, 256, 216);
	drop_box -> image(new Fl_PNG_Image("res/drag_and_drop.png"));
	drop_box -> redraw();
	Fl_Button *send_btn = new Fl_Button((grp -> x() + grp -> w()) / 2, grp -> h() - 40, 65, 20, "upload");
	grp->add(drop_box);
	grp->add(send_btn);
	return grp;
}


#define LOG_TXT_SIZE 12
int start_gui(job jbs[], int jlen){
/*
    root = new Fl_Double_Window(0, 0, 256, 256, "AutoDoc");
    //TODO set max and min size for Fl_Window
    root->add(main_view());
    root->add(new Auto_Fl_Table(0, 256, 256, 256, "Queue"));
*/
    root = make_window();
    root->callback(root_exit);
    log_inf(_UI_H, "GUI Starting...");
    root->show();
    return Fl::run();
}

int load_ui(job jbs[], int jlen, bool _is_gui = false){
    //add checks for CLI or GUI from args
    	is_gui = _is_gui;
	if(!_is_gui /*|| screen not found*/){
        //TODO if screen not found, run in CLI mode
        /*
         if(screen not found){
            println("screen not found continuing with command line interface");
         }
         */
		start_shell(jlen, jbs, "autodoc> ");
	}
	else start_gui(jbs, jlen);
	return 0;
}
#endif /* ui_h */
