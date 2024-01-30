CC = gcc
CFLAGS = -g -Wall 

all: schedule 

PROGNAME: schedule 
			$(CC) $(CFLAGS) -o $@ schedule.c 
			
run: 
	PATH=$(PWD):$$PATH; ./schedule $(ARGS)
	
