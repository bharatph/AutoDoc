#ifndef SHELL_H
#define SHELL_H
#define _SH_TAG "SHELL"
/*
 * shell.h
 *
 *  Created on: 23-Oct-2016
 *      Author: root
 */
#ifdef __GNUG__
#include <iostream>
#else
#include <stdio.h>
#endif
#include <string.h>
#include "logc.h"

#define SH_BUFSIZE 1024 //shell read buffer size
#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIM " \t\r\n\a" //shell token delimeters

typedef int (*sh_func_ptr)(char**);



/*
 * Return a vector by the using delimeters on the given string based on terminal/linux style args
 */
char **
shSplit(char * line) {
	int bufsize = SH_TOK_BUFSIZE;
	int position = 0;
	char **tokens = (char **) malloc(bufsize * sizeof(char*));
	char *token;

    if (!tokens){
		log_err(_SH_TAG, ": allocation error, not enough memory...closing");
        return NULL;
    }
    
#ifdef __GNUG__
	token = strtok(line, SH_TOK_DELIM);
#else
	token = strtok(line, SH_TOK_DELIM);
#endif
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position == bufsize) {
			bufsize += SH_TOK_BUFSIZE;
			tokens = (char **) realloc(tokens, bufsize * sizeof(char*));
            if (!tokens){
				log_fat(_SH_TAG, ": allocation error, not enough memory...closing");
                return NULL;
            }
		}
		#ifdef __GNUG__
		token = strtok(NULL, SH_TOK_DELIM);
		#else
		token = strtok(NULL, SH_TOK_DELIM);
		#endif

	}
	tokens[position] = NULL;
	return tokens;
}

/*
 * read input and return a char*
 */
char *
shRead() {
	int bufsize = SH_BUFSIZE;
	int position = 0;
	char c;
	char * buffer = (char *) malloc(sizeof(char) * bufsize);

	if (!buffer)
		log_fat(_SH_TAG, "allocation error, not enough memory...closing");

	while (1) {
		//read a character
		c = getchar();

		//if we hit EOF or '\n' update it as null '\0' and return
		if (c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		} else {
			buffer[position] = c;
		}
		position++;

		//if we reached the buffer size, reallocate
		if (position == bufsize) {
			bufsize += SH_BUFSIZE;
			buffer = (char *) realloc(buffer, sizeof(char) * bufsize);
			if (!buffer)
				log_fat(_SH_TAG, "allocation error, not enough memory...closingnn");
		}

	}
}

int sh_next(sh_func_ptr *_sh_func, char **_commands, char **_info, int _cmd_len, const char *_shell_name){
    char **args;
    char *line;
    printf("%s", _shell_name);
    line = shRead();
    args = shSplit(line);
    if (*args == NULL) {
        // An empty command was entered.
        return 0;
    }
    for (int i = 0; i < _cmd_len; i++) {
        if (strcmp(*args, _commands[i]) == 0) {
            return (*(_sh_func)[i])(args);
        }
    }
    printf("you clearly need 'help'\n");
    return 0;
}


#endif
