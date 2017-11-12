/*
 * lp.h
 *
 *  Created on: Jul 6, 2017
 */


/*
 *
The client can ask the following requests
+---------+---------------------------------------+
|  CODE   |              DESCRIPTION              |
+---------+---------------------------------------+
| CON_REQ | Requesting for connection             |
| BIN_SEN | Sending a binary file                 |
| TXT_SEN | Sending a text file                   |
| SEN_CMD | Request for available commands        |
| DIAG_ST | Start diagnosis with provided data    |
| DIAG_CA | cancel the current diagnosis          |
| SET_BUF | sets the buffer to the given value    |
| GET_BUF | gets the buffer value from the server |
+---------+---------------------------------------+

Server replies with the following
+---------+--------------------------------------------+
|  CODE   |                DESCRIPTION                 |
+---------+--------------------------------------------+
| REQ_ACK | The request for connection is acknowledged |
| BINA_OK | Binary file is received                    |
| BINA_ER | Binary file is not received                |
| TEXT_OK | Text file is received                      |
| TEXT_ER | Text file is not received                  |
| DIAG_OK | Diagnosis completed successfully           |
| DIAG_ER | Diagnosis failed                           |
+---------+--------------------------------------------+
 *
 */
#ifndef LP_H
#define LP_H "LP_H"

#include<iostream>
#include<vector>
#include<map>
#include<unistd.h>
#include<string>
#if defined(__linux__)
#include<sys/sendfile.h>
#endif

<<<<<<< HEAD
#include <clog/clog.h>
=======
#include "clog.h"
>>>>>>> b6b441a863367900f343a8df28f5de0def3843b1
#define LP_CON "CON"
#define LP_ACK "ACK"
#define LP_CMD "CMD"
#define LP_BIN "BIN"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 256
#endif

using namespace std;

map<int, const char *> session_ids;

void init_protocol(){
	session_ids[0] = "__init__";
}
/*
int lp_init_server(int _sockfd){
	char *_str = (char *)malloc(LP_MSG_SIZE);
	if(read(_sockfd, _str, LP_MSG_SIZE) == 7){
			if(strncmp(_str, LP_CON_REQ, LP_MSG_SIZE) == 0){
				log_inf(LP_H, "Client found");
				write(_sockfd, LP_REQ_ACK, LP_MSG_SIZE);
				log_inf(LP_H, "Client Acknowledged");
				return 0;
			}
			return -1;
	}
	return -1;
}

int lp_init_client(int _sockfd){
		if(write(_sockfd, LP_CON_REQ, LP_MSG_SIZE) == 7){
				char *_str = (char *)malloc(LP_MSG_SIZE);
				read(_sockfd, _str, LP_MSG_SIZE);
				if(strncmp(_str, LP_REQ_ACK, LP_MSG_SIZE) == 0){
						log_inf(LP_H, "Connection successful");
						return 0;
				}
				log_inf(LP_H, "Connection failed");
				return -1;
		}
		return -1;
}

*/
/*
 * same as write(int, void*, size_t) from <unistd.h>
 * but returns 0 or -1 indicating success or failure
 */

/*
int atomic_write(int _sockfd, const void *_msg, size_t _size){
	static int _try;
	if(_size == 0)return -1;//TEST _size <= 0 ?
	int _bwritten = write(_sockfd, _msg, _size);
	
	if(_try++ > 6){
		return -1;
	}
	atomic_write(_sockfd, _msg, _size - _bwritten); //deduct the written bytes from the actual size
	if(_bwritten == 0){
		return -1;
	}else if(_bwritten < 1){
		//handle errors
		return -1;
	}
	return 0;
}

//safe to call no-op
int lp_send_text(int sockfd, const char *text){
	return -1;
}

int lp_send_binary(int sockfd, const char *binary){
	FILE *fp = fopen(binary, "rb");
	if(fp){
		log_err(LP_H, "Cannot open the binary file");
		return -1;
	}
	atomic_write(sockfd, LP_BIN_SEN, LP_MSG_SIZE);
#if defined(__linux__)
	sendfile(sockfd, fileno(fp), NULL, 1);
#elif
	while(1){
		void *_buffer = (void *)malloc(sizeof(BUFFER_SIZE));
		int _bread = read(fileno(fp), _buffer, BUFFER_SIZE);

		if(_bread == 0){
			break;
		}
		else if(_bread < 0){
			//handle errors
			log_err(LP_H, "Error occurred when reading file");
			return -1;
		}
		void *p = _buffer;
		while(_bread > 0){
			int _bwritten = write(sockfd, p, _bread);
			if(_bwritten <=0){
				//handle errors
				log_err(LP_H, "Error occurred while writing the file");
				return -1;
			}
			_bread -= _bwritten;
			p += _bwritten;
		}
	}
#endif
	return 0;
}
*/
#endif /* LP_H */
