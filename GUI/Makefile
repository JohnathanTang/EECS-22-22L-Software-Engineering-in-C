#Makefile for ZotChat, EECS22L Winter 2019 
#Author: Team 5 (4OUR) 
#Date: 02/20/2019
CC	= gcc
DEBUG	= -g -DDEBUG
CFLAGS	= -ansi -std=c99 -Wall -c 
LFLAGS	= -Wall

GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
INC	= $(GTKINC)
LIBS	= $(GTKLIBS)
OPTS	= -g -std=c99

OBJS	= FriendRequest.o 

#DEPS =  

DESIGN	= AddFriend FR

all: $(DESIGN)

FriendRequest.o: FriendRequest.c
	$(CC) $(CFLAGS) FriendRequest.c $(INC) $(OPTS) -o FriendRequest.o

FR: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(LIBS) $(OPTS) -o FR

AddFriend.o: AddFriend.c
	$(CC) $(CFLAGS) AddFriend.c $(INC) $(OPTS) -o AddFriend.o

AddFriend: AddFriend.o
	$(CC) $(LFLAGS) AddFriend.o $(LIBS) $(OPTS) -o AddFriend

clean:
	rm -f *.o FR AddFriend
