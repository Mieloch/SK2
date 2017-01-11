CFLAGS = `python3.5-config --cflags`
LDFLAGS = `python3.5-config --ldflags`

server: server.o measure.o
	gcc server.o measure.o -o server $(LDFLAGS)

server.o: server.c
	gcc -c $(CFLAGS) server.c

measure.o: measure.c
	gcc -c measure.c
	
client: client.c
	gcc -o client client.c

.PHONY: clean

clean:
	rm *.o
