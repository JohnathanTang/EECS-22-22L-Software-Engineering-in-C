all: client server 

clean:
	rm -rf client server

client: Client.c
	gcc -Wall -std=c99 Client.c -o client

server: ServerV3.c
	gcc ServerV3.c -Wall -std=gnu99 -lpthread -o server


