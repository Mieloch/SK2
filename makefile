server: server.c measure.c
	gcc -o server server.c measure.c
client: client.c
	gcc -o client client.c
