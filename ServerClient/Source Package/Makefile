#Makefile for ZotChess, EECS22L Winter 2019 
#Author: Team 5 (4OUR) 
#Date: 01/30/2019

BIN = ./bin
SOURCE = ./src

CC	= gcc
DEBUG	= -g -DDEBUG
CFLAGS	= -ansi -std=c99 -Wall -c 
LFLAGS	= -Wall

GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
INC	= $(GTKINC)
LIBS	= $(GTKLIBS)
OPTS	= -g -std=c99

OBJS	= $(BIN)/GUI.o $(BIN)/IsValid.o $(BIN)/Initialize.o $(BIN)/PrintBoard.o $(BIN)/ReadInput.o $(BIN)/Move.o $(BIN)/Check.o $(BIN)/AI.o $(BIN)/GUISTUFF.o $(BIN)/Logfile.o $(BIN)/IsValidCheck.o

DEPS =  $(SOURCE)/IsValid.h $(SOURCE)/Initialize.h $(SOURCE)/PrintBoard.h $(SOURCE)/ReadInput.h $(SOURCE)/Move.h $(SOURCE)/Check.h $(SOURCE)/GUISTUFF.h $(SOURCE)/AI.h $(SOURCE)/Logfile.h $(SOURCE)/IsValidCheck.h

DESIGN	= $(BIN)/Chess
all: $(DESIGN)

$(BIN)/GUI.o: $(SOURCE)/GUI.c $(DEPS) 
	$(CC) $(CFLAGS) $(SOURCE)/GUI.c $(INC) $(OPTS) -o $(BIN)/GUI.o

$(BIN)/GUISTUFF.o: $(SOURCE)/GUISTUFF.c $(SOURCE)/GUISTUFF.h $(SOURCE)/IsValid.h $(SOURCE)/Move.h $(SOURCE)/Check.h $(SOURCE)/Constants.h $(SOURCE)/AI.h $(SOURCE)/Logfile.h $(SOURCE)/IsValidCheck.h
	$(CC) $(CFLAGS) $(SOURCE)/GUISTUFF.c -o $(BIN)/GUISTUFF.o

$(BIN)/AI.o: $(SOURCE)/AI.c $(SOURCE)/AI.h $(SOURCE)/Move.h
	$(CC) $(CFLAGS) $(SOURCE)/AI.c -o $(BIN)/AI.o 
	
$(BIN)/Check.o: $(SOURCE)/Check.c $(SOURCE)/Check.h $(SOURCE)/IsValid.h
	$(CC) $(CFLAGS) $(SOURCE)/Check.c -o $(BIN)/Check.o 
	
$(BIN)/IsValid.o: $(SOURCE)/IsValid.c $(SOURCE)/IsValid.h $(SOURCE)/Check.h
	$(CC) $(CFLAGS) $(SOURCE)/IsValid.c -o $(BIN)/IsValid.o 

$(BIN)/IsValidCheck.o: $(SOURCE)/IsValidCheck.c $(SOURCE)/IsValidCheck.h
	$(CC) $(CFLAGS) $(SOURCE)/IsValidCheck.c -o $(BIN)/IsValidCheck.o

$(BIN)/Initialize.o: $(SOURCE)/Initialize.c $(SOURCE)/Initialize.h 
	$(CC) $(CFLAGS) $(SOURCE)/Initialize.c -o $(BIN)/Initialize.o 

$(BIN)/Logfile.o: $(SOURCE)/Logfile.c $(SOURCE)/Logfile.h $(SOURCE)/Constants.h
	$(CC) $(CFLAGS) $(SOURCE)/Logfile.c -o $(BIN)/Logfile.o

$(BIN)/PrintBoard.o: $(SOURCE)/PrintBoard.c $(SOURCE)/PrintBoard.h 
	$(CC) $(CFLAGS) $(SOURCE)/PrintBoard.c -o $(BIN)/PrintBoard.o 

$(BIN)/ReadInput.o: $(SOURCE)/ReadInput.c $(SOURCE)/ReadInput.h 
	$(CC) $(CFLAGS) $(SOURCE)/ReadInput.c -o $(BIN)/ReadInput.o 

$(BIN)/Move.o: $(SOURCE)/Move.c $(SOURCE)/Move.h 
	$(CC) $(CFLAGS) $(SOURCE)/Move.c -o $(BIN)/Move.o

$(BIN)/Chess: $(OBJS) 
	$(CC) $(LFLAGS) $(OBJS) $(LIBS) $(OPTS) -o $(BIN)/Chess 

clean:
	rm -f $(BIN)/*.o $(BIN)/Chess Chess_V1.0_src.tar.gz
	
tar:
	tar -cvf Chess_V1.0_src.tar.gz $(DEPS) README.md $(SOURCE)/GUI.c $(SOURCE)/GUISTUFF.c $(SOURCE)/AI.c $(SOURCE)/Check.c $(SOURCE)/IsValid.c $(SOURCE)/IsValidCheck.c $(SOURCE)/Initialize.c $(SOURCE)/Logfile.c $(SOURCE)/PrintBoard.c $(SOURCE)/ReadInput.c $(SOURCE)/Move.c $(DEPS)

