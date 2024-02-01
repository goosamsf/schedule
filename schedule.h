#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

typedef struct pnode{
	pid_t pid;
	char* prog;
	char** args;
	struct pnode* next;
} pnode;
pid_t g_pid = 0;

void print_next_prog(char*,char**);
void execute_next_prog(char* prog, char** nextargs);

void enqueue(pnode**, pnode**, pnode*);
void dequeue(pnode**head);
void push_back(pnode**head, pnode**tail, pid_t pid);

void queue_clear(pnode**head);
void node_printer(pnode** head, pnode** tail);

void start_timer(long quan);
void stop_timer();
void tick_handler();

pnode *createNode(char* prog, char** args, pid_t pid);
char **extract_next_prog(char**, int*,int,int);
int q_size(pnode** head);

