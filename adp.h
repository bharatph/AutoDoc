/*
 * adp.h
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
#ifndef ADP_H
#define ADP_H "ADP_H"

#include<iostream>
#include<vector>
#include<map>
#include<unistd.h>
#include<string>
#if defined(__linux__)
#include<sys/sendfile.h>
#endif

#include "logc.h"

#define ADP_CON_REQ "CON_REQ"
#define ADP_BIN_SEN "BIN_SEN"
#define ADP_TXT_SEN "TXT_SEN"
#define ADP_SEN_CMD "SEN_CMD"
#define ADP_DIAG_ST "DIAG_ST"
#define ADP_DIAG_CA "DIAG_CA"
#define ADP_SET_BUF "SET_BUF"
#define ADP_GET_BUF "GET_BUF"

#define ADP_REQ_ACK "REQ_ACK"
#define ADP_BINA_OK "BINA_OK"
#define ADP_BINA_ER "BINA_ER"
#define ADP_TEXT_OK "TEXT_OK"
#define ADP_TEXT_ER "TEXT_ER"
#define ADP_DIAG_OK "DIAG_OK"
#define ADP_DIAG_ER "DIAG_ER"

#define ADP_MSG_SIZE 7

#define ADP_SERVER 0
#define ADP_CLIENT 1

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 256
#endif

using namespace std;

map<int, char *> session_ids;

void init_protocol(){
	session_ids[0] = "__init__";
}

int adp_init_connection(int _sockfd, int _type){
	switch(_type){
	case ADP_SERVER:
		char *_str = (char *)malloc(ADP_MSG_SIZE);
		if(read(_sockfd, ADP_CON_REQ, ADP_MSG_SIZE) == 7){
			if(strncmp(_str, ADP_CON_REQ, ADP_MSG_SIZE) == 0){
				log_inf(ADP_H, "Client found");
				write(_sockfd, ADP_REQ_ACK, ADP_MSG_SIZE);
				log_inf(ADP_H, "Client Acknowledged");
				return 0;
			}
			return -1;
		}
		break;

	case ADP_CLIENT:
		if(write(_sockfd, ADP_CON_REQ, ADP_MSG_SIZE) == 7){
				char *_str = (char *)malloc(ADP_MSG_SIZE);
				read(_sockfd, _str, ADP_MSG_SIZE);
				if(strncmp(_str, ADP_REQ_ACK, ADP_MSG_SIZE) == 0){
						log_inf(ADP_H, "Connection successful");
						return 0;
				}
				log_inf(ADP_H, "Connection failed");
				return -1;
		}
		break;
	}
	return -1;
}

/*
 * same as write(int, void*, size_t) from <unistd.h>
 * but returns 0 or -1 indicating success or failure
 */
int atomic_write(int _sockfd, const void *_msg, size_t _size){
	static int _try;
	if(_size == 0)return -1;//TEST _size <= 0 ?
	int _bwritten = write(_sockfd, _msg, _size);

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
int adp_send_text(int sockfd, const char *text){
	return -1;
}

int adp_send_binary(int sockfd, const char *binary){
	FILE *fp = fopen(binary, "rb");
	if(fp){
		log_err(ADP_H, "Cannot open the binary file");
		return -1;
	}
	atomic_write(sockfd, ADP_BIN_SEN, ADP_MSG_SIZE);
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
			log_err(ADP_H, "Error occurred when reading file");
			return -1;
		}
		void *p = _buffer;
		while(_bread > 0){
			int _bwritten = write(sockfd, p, _bread);
			if(_bwritten <=0){
				//handle errors
				log_err(ADP_H, "Error occurred while writing the file");
				return -1;
			}
			_bread -= _bwritten;
			p += _bwritten;
		}
	}
#endif
	return 0;
}

#endif /* ADP_H */
