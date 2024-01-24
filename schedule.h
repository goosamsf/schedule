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

char **extract_next_prog(char**, int*,int,int);
void print_next_prog(char*,char**);
void execute_next_prog(char* prog, char** nextargs);
void enqueue(pnode**, pnode**, pnode*);

pnode *createNode(char* prog, char** args, pid_t pid){
	pnode* newnode = NULL;
	if(NULL == (newnode = malloc(sizeof(pnode)))){
		perror("malloc");
		exit(-1);
	}
	newnode->prog = prog;
	newnode->args = args;
	newnode->pid = pid;
	newnode->next = NULL;
	return newnode;
}

void push_back(pnode**head, pnode**tail, pid_t pid){
	pnode *firstnode = *head;
	pnode *nextnode = firstnode->next;
	if(firstnode->args){
		free(firstnode->args);
	}
	firstnode->prog = NULL;
	firstnode->args = NULL;
	firstnode->pid = pid;

	*head = nextnode;	
	(*tail)->next = firstnode;
	*tail = firstnode;
	(*tail)->next = NULL;
	if(*head){
		g_pid = (*head)->pid;
	}
}


void enqueue(pnode**head, pnode**tail, pnode* node){
	if(*head == NULL){
		*head = node;
		*tail = node;
		return;
	}
	(*tail)->next = node;
	(*tail) = node;
	g_pid = (*head)->pid;
}

void dequeue(pnode**head){
	pnode* firstnode = *head;
	pnode* nextnode = firstnode->next;
	*head = nextnode;
	if(firstnode->args){
		free(firstnode->args);
		firstnode->args = NULL;
	}
	free(firstnode);
	firstnode = NULL;
	if(nextnode){
		g_pid = nextnode->pid;
	}
}

void queue_clear(pnode**head){
	pnode* firstnode = *head;	
	pnode* nextnode;
	while(firstnode){
		nextnode = firstnode->next;
		dequeue(head);
		firstnode = nextnode; 
	}
}


int q_size(pnode** head){
	int i = 0;
	pnode* firstnode = *head;
	while(firstnode){
		i++;
		firstnode = firstnode->next;
	}
	return i;
}

void node_printer(pnode** head, pnode** tail){
	if(!head){
		fprintf(stderr, "head is null\n");
		exit(-1);
	}
	pnode* firstnode = *head;
	int i;
	char **args; 
	while(firstnode){
		args = firstnode->args;
		printf("addr : %p ", firstnode);
		printf("[prog : %s, args : [ ", firstnode->prog);
		if(args){
			for(i =0; args[i] != NULL; i++) {
				printf("%s ", args[i]);
			}
		}else{
			printf("NULL ]");
		}
		printf("]pid : %d ]\n", firstnode->pid);
		firstnode = firstnode ->next;
	}
	printf("head : %p, tail : %p\n", *head, *tail);
}
//myqueue
void start_timer(long quan){
	struct itimerval tbuf;
	tbuf.it_interval.tv_sec = 0; 
	tbuf.it_interval.tv_usec = 0;
	if(quan >= 1000){
		tbuf.it_value.tv_sec = quan/1000;
		tbuf.it_value.tv_usec = 0;
	}else{
		tbuf.it_value.tv_sec = 0;
		tbuf.it_value.tv_usec = quan*1000;  
	}
	if(setitimer(ITIMER_REAL, &tbuf, NULL) == -1){
		perror("setitimer");
		exit(-1);
	}
}
void stop_timer(){
	struct itimerval tbuf;

	tbuf.it_interval.tv_sec = 0; 
	tbuf.it_interval.tv_usec = 0;
  tbuf.it_value.tv_sec = 0;	
	tbuf.it_value.tv_usec = 0; 

	if(setitimer(ITIMER_REAL, &tbuf, NULL) == -1){
		perror("setitimer");
		exit(-1);
	}
}

void tick_handler(){
	kill(g_pid, SIGSTOP);
}
