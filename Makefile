all:	target

target:	server.o    client.o
	gcc -o server server.o -g -W -ggdb3
	gcc -o client client.o -g -W -ggdb3

server.o:	server.c
		gcc -c -ggdb3 server.c

client.o:	client.c
		gcc -c -ggdb3  client.c

clean:
	rm -rf server client *.o
