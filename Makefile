all:	target

target:	server.o    client.o
	gcc -o server server.o -g -W
	gcc -o client client.o -g -W

server.o:	server.c
		gcc -c server.c

client.o:	client.c
		gcc -c  client.c

clean:
	rm -rf server client *.o
