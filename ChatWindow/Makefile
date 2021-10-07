CC = gcc
DEBUG = -g -DDEBUG
CFLAGS = -ansi -std=c99 -Wall -c
LFLAGS = -Wall

GTKINC = `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS = `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
INC = $(GTKINC)
LIBS = $(GTKLIBS)
OPTS = -g -std=c99

OBJS = ChatWindow.o

DESIGN = ChatWindow

all: $(DESIGN)

ChatWindow.o: ChatWindow.c
	$(CC) $(CFLAGS) ChatWindow.c $(INC) $(OPTS) -o ChatWindow.o 

ChatWindow: $(OBJS) 
	$(CC) $(LFLAGS) $(OBJS) $(LIBS) $(OPTS) -o ChatWindow

clean:
	rm -f *.o ChatWindow
