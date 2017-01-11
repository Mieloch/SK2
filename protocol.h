#ifndef PROTOCOL_H
#define PROTOCOL_H


#define GET_CPU_USAGE 0
#define EXECUTE_JOB 1
typedef struct client_request{
	int code;
	char payload[1024];
}client_request;
#endif
