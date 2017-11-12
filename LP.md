# Lazy Protocol (LP)
Lazy is a session-aware protocol for maintaining anonymous

## Few characteristics of the Lazy Protocol
Non-Authenticative
Connection is alive through sessions

## Transport Format
The following algorithm is used for sending and retreiving the information to and from the server


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

## Sessions
LP maintains a session-id to identify a active session. This session-id is used when the connection is interupted when a file is being uploaded to the server. This ensures that the uploaded files are not uploaded again when reconnecting.

+------------+-------------------+
| session-id |     inte_file     |
+------------+-------------------+
|          0 | x_ray.jpg         |
|          1 | blood_report.docx |
|          . | .                 |
|          . | .                 |
|          . | .                 |
|          . | .                 |
+------------+-------------------+
The session-id is expired after 12hrs

## Internal Working
The following is algorithm is the usage of this protocol

##C API
int lp_init_server(int sock) -- initialize server
int lp_init_client(int sock) -- initialize connection to the server
int lp_send_binary(int sock, const char *binary) -- send file to server
int lp_send_text(int sock, const char *text) -- send text to server
int lp_start_diagnosis(int sock) -- starts the diagnosis process
int lp_cancel_diagnosis(int sock) -- cancels diagnosis if running
int lp_remove_session(int sock) -- deletes the client session
