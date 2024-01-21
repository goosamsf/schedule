CC = gcc
CFLAGS = -g -Wall -Werror

all: schedule 

PROGNAME: schedule 
			$(CC) $(CFLAGS) -o $@ schedule.c 
			
clean:
