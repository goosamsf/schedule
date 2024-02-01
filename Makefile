CC = gcc
CFLAGS = -g -Wall -Werror 

all : schedule
PROGNAME: schedule 
			$(CC) $(CFLAGS) -o $@ schedule.c 
ARGS = $1			

run: 
	$(shell echo $ARGS)
	
