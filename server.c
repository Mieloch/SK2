#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <python3.5/Python.h>
#include "protocol.h"
#include "measure.h"

#define SERVER_PORT 1234
#define QUEUE_SIZE 5

void run_python_hello_world(char *name)
{
	wchar_t *program = Py_DecodeLocale(name, NULL);
	if(program == NULL){
		fprintf(stderr, "Fatal error: cannot decode name\n");
		exit(1);
	}
	Py_SetProgramName(program);
	Py_Initialize();
	PyRun_SimpleString("print('Hello World')\n");
	Py_Finalize();
	PyMem_RawFree(program);
}

void handle_cpu_usage_request(int clientSocket)
{
	int cpu_usage = get_cpu_usage();
	write(clientSocket, &cpu_usage, sizeof(int));
}

void handle_execute_job_request(int clientSocket, char* script)
{
	printf("EXECUTE_JOB\n");

	wchar_t *program = Py_DecodeLocale("server", NULL);
	if(program == NULL){
		fprintf(stderr, "Fatal error: DecodeLocale failure\n");
		exit(1);
	}
	Py_SetProgramName(program);
	Py_Initialize();
	clock_t start = clock();
	PyRun_SimpleString(script);
	clock_t stop = clock();
	Py_Finalize();
	PyMem_RawFree(program);
	double time_elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
	int time_elapsed_ms = (int)(time_elapsed);
	char response[20];
	sprintf(response, "%d", time_elapsed_ms);
	printf("\nresponse = %s\n\n", response);	
write(clientSocket, response, strlen(response) * sizeof(char));
}

int reverse(int num)
{
	return ((num>>24)&0xff) | // move byte 3 to byte 0
                    ((num<<8)&0xff0000) | // move byte 1 to byte 2
                    ((num>>8)&0xff00) | // move byte 2 to byte 1
                    ((num<<24)&0xff000000);
}

void process_request(int clientSocket)
{
	struct client_request *request = malloc(sizeof(struct client_request));

	read(clientSocket, request, (sizeof(struct client_request)));
	request->code = reverse(request->code);



	if(request->code == GET_CPU_USAGE)
	{
		handle_cpu_usage_request(clientSocket);
	}
	else if(request->code == EXECUTE_JOB)
	{
		printf("script payload = %s\n", request->payload);  	   
		handle_execute_job_request(clientSocket,request->payload);
	}
}

int main(int argc, char* argv[])
{
	int nSocket, nClientSocket;
	int nBind, nListen;
	int nFoo = 1;
	int pid;
	socklen_t nTmp;
	struct sockaddr_in stAddr, stClientAddr;


	/* address structure */
	memset(&stAddr, 0, sizeof(struct sockaddr));
	stAddr.sin_family = AF_INET;
	stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stAddr.sin_port = htons(SERVER_PORT);

	/* create a socket */
	nSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nSocket < 0)
	{
		fprintf(stderr, "%s: Can't create a socket.\n", argv[0]);
		exit(1);
	}
	setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

	/* bind a name to a socket */
	nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
	if (nBind < 0)
	{
		fprintf(stderr, "%s: Can't bind a name to a socket.\n", argv[0]);
		exit(1);
	}
	/* specify queue size */
	nListen = listen(nSocket, QUEUE_SIZE);
	if (nListen < 0)
	{
		fprintf(stderr, "%s: Can't set queue size.\n", argv[0]);
	}

	printf("Server started\n");

	while(1)
	{
		/* block for connection request */
		nTmp = sizeof(struct sockaddr);
		nClientSocket = accept(nSocket, (struct sockaddr*)&stClientAddr, &nTmp);
		if (nClientSocket < 0)
		{
			fprintf(stderr, "%s: Can't create a connection's socket.\n", argv[0]);
			exit(1);
		}
		//printf("%s: [connection from %s]\n", argv[0], inet_ntoa((struct in_addr)stClientAddr.sin_addr));
		pid = fork();
		if(pid < 0)
		{
			close(nClientSocket);
			fprintf(stderr, "%s: Can't create process.\n", argv[0]);
			exit(1);
		}

		if(pid == 0)
		{
			process_request(nClientSocket);
			close(nClientSocket);
		}
		else if(pid > 0)
		{
			close(nClientSocket);
		}
		
	}
	close(nSocket);
	return(0);
}
