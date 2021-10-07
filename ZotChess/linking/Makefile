#Makefile for ZotChess, EECS22L Winter 2019 
#Author: Team 5 (4OUR) 
#Date: 01/30/2019

CC	= gcc
DEBUG	= -g -DDEBUG
CFLAGS	= -ansi -std=c99 -Wall -c 
LFLAGS	= -Wall

GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
INC	= $(GTKINC)
LIBS	= $(GTKLIBS)
OPTS	= -g -std=c99

OBJS	= GUI.o IsValid.o Initialize.o PrintBoard.o ReadInput.o Move.o Check.o AI.o GUISTUFF.o

DEPS =  IsValid.h Initialize.h PrintBoard.h ReadInput.h Move.h Check.h GUISTUFF.h AI.h

DESIGN	= Chess
all: $(DESIGN)

GUI.o: GUI.c $(DEPS) 
	$(CC) $(CFLAGS) GUI.c $(INC) $(OPTS) -o GUI.o

GUISTUFF.o: GUISTUFF.c GUISTUFF.h IsValid.h Move.h Check.h Constants.h AI.h PrintBoard.h ReadInput.h
	$(CC) $(CFLAGS) GUISTUFF.c -o GUISTUFF.o

AI.o: AI.c AI.h Move.h
	$(CC) $(CFLAGS) AI.c -o AI.o 
	
Check.o: Check.c Check.h IsValid.h
	$(CC) $(CFLAGS) Check.c -o Check.o 
	
IsValid.o: IsValid.c IsValid.h Check.h
	$(CC) $(CFLAGS) IsValid.c -o IsValid.o 

Initialize.o: Initialize.c Initialize.h 
	$(CC) $(CFLAGS) Initialize.c -o Initialize.o 

PrintBoard.o: PrintBoard.c PrintBoard.h 
	$(CC) $(CFLAGS) PrintBoard.c -o PrintBoard.o 

ReadInput.o: ReadInput.c ReadInput.h 
	$(CC) $(CFLAGS) ReadInput.c -o ReadInput.o 

Move.o: Move.c Move.h 
	$(CC) $(CFLAGS) Move.c -o Move.o 

Chess: $(OBJS) 
	$(CC) $(LFLAGS) $(OBJS) $(LIBS) $(OPTS) -o Chess 

clean:
	rm -f *.o Chess
