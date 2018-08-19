#title: Makefile
#Author: Sholaja Yusuf
#Date: February 17 2017
#!/usr/bin/make

CC = gcc
CFLAGS = -Wall -lncurses -pthread -lpthread

PROG = run 
HDRS = console.h game.h List.h bullet.h enemy.h player.h
SRCS1 = comp3430_A2.c 
SRCS2 = game.c
SRCS3 = console.c
SRCS4 = List.c
SRCS5 = bullet.c
SRCS6 = enemy.c
SRCS7 = player.c
SRCS8 = globals.c

OBJDIR = object
OBJS1 = $(OBJDIR)/comp3430_A2.o
OBJS2 =	$(OBJDIR)/game.o
OBJS3 = $(OBJDIR)/console.o
OBJS4 = $(OBJDIR)/List.o
OBJS5 = $(OBJDIR)/bullet.o
OBJS6 = $(OBJDIR)/enemy.o
OBJS7 = $(OBJDIR)/player.o
OBJS8 =  $(OBJDIR)/globals.o
# compiling rules

# WARNING: *must* have a tab before each definition
$(PROG): $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) $(OBJS7) $(OBJS8) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) $(OBJS7) $(OBJS8) -o $(PROG)

$(OBJS1): $(SRCS1) $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCS1) -o $(OBJS1)

$(OBJS2): $(SRCS2) $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCS2) -o $(OBJS2)

$(OBJS3): $(SRCS3) $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCS3) -o $(OBJS3)

$(OBJS4): $(SRCS4) $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCS4) -o $(OBJS4)

$(OBJS5): $(SRCS5) $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCS5) -o $(OBJS5)

$(OBJS6): $(SRCS6) $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCS6) -o $(OBJS6)

$(OBJS7): $(SRCS7) $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCS7) -o $(OBJS7)

$(OBJS8): $(SRCS8) $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCS8) -o $(OBJS8)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -f $(PROG) $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) $(OBJS7) $(OBJS8)

