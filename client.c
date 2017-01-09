#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "protocol.h"

#define BUFSIZE 10000

char *server = "127.0.0.1";	/* adres IP pętli zwrotnej */
char *protocol = "tcp";
short service_port = 1234;	/* port usługi daytime */

int bufor;

int main ()
{
	struct sockaddr_in sck_addr;

	int sck, odp;

	printf ("Usługa %d na %s z serwera %s :\n", service_port, protocol, server);

	memset (&sck_addr, 0, sizeof sck_addr);
	sck_addr.sin_family = AF_INET;
	inet_aton (server, &sck_addr.sin_addr);
	sck_addr.sin_port = htons (service_port);

	if ((sck = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror ("Nie można utworzyć gniazdka");
		exit (EXIT_FAILURE);
	}

	if (connect (sck, (struct sockaddr*) &sck_addr, sizeof sck_addr) < 0) {
		perror ("Brak połączenia");
		exit (EXIT_FAILURE);
	}
        struct client_request *request = malloc(sizeof(struct client_request));
	request->code = 0;	
	write (sck, request, sizeof(struct client_request));
	while ((odp = read (sck, &bufor, sizeof(int))) > 0)
		printf("\n%d\n",bufor);
	close (sck);

	exit (EXIT_SUCCESS);
}
