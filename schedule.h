#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

typedef struct pnode{
	pid_t pid;
	char* prog;
	char** args;
	struct pnode* next;
} pnode;

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
	firstnode->prog = NULL;
	if(firstnode->args){
		free(firstnode->args);
	}
	firstnode->args = NULL;
	firstnode->pid = pid;

	*head = nextnode;	
	(*tail)->next = firstnode;
	*tail = firstnode;
	(*tail)->next = NULL;
}


void enqueue(pnode**head, pnode**tail, pnode* node){
	if(*head == NULL){
		*head = node;
		*tail = node;
		return;
	}
	(*tail)->next = node;
	(*tail) = node;
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
