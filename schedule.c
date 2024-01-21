#include "schedule.h"

static void print_usage(char *prog){
	fprintf(stderr,"usage: %s quantum [prog1 [args] [: prog2 [args] [: prog3] [args] [: ...]]]\n",prog);
}

int main(int argc, char* argv[]){
	if(argc < 3){
		fprintf(stderr, "Invalid Arguments\n");
		print_usage(argv[0]);
		exit(1);
	}
	pnode* head = NULL;
	pnode* tail = NULL;
	pnode* node = NULL;
  char *end;
	char *prog = NULL;
	char **nextargs = NULL;
	int i,j=2;
	int quantum = strtol(argv[1], &end, 10);
	if(*end){
		fprintf(stderr, "Invalid Quantum\n");
		exit(1);
	}
	printf("quantum is %d\n", quantum); 
	//
	/*
	 *There are two cases where we parse differently
	 *1. ":" case
	 *2. last program
	 */
	for(i=2; i < argc; i++) {
		if(!strcmp(argv[i], ":")){
			prog = argv[j];
			nextargs= extract_next_prog(argv, &j, i , i-j+1);
			node = createNode(prog,nextargs, 0);
			enqueue(&head,&tail, node);

			//print_next_prog(prog, nextargs);
			//push_back prog,nextargs, pid with 0
			//execute_next_prog(prog, nextargs);
			//free(nextargs);
			continue;
		}
		if(i == argc-1){
			prog = argv[j];
			nextargs = extract_next_prog(argv, &j, i+1, i-j+2);
			node = createNode(prog,nextargs, 0);
			enqueue(&head,&tail, node);
			//print_next_prog(prog, nextargs);
			//push_back prog,nextargs, pid 
			//execute_next_prog(prog, nextargs);
			//free(nextargs);
		}
		
		
	}
	node_printer(&head, &tail);

	push_back(&head, &tail, 4);
	printf("\n");
	node_printer(&head, &tail);

	printf("queue size : %d\n", q_size(&head));
	queue_clear(&head);
	printf("queue size : %d\n", q_size(&head));


	return 0;
}

void execute_next_prog(char* prog, char** nextargs){
	pid_t child;
	int status;
	if((child = fork()) == -1){
		perror("fork");
		exit(-1);
	}
	if( child != 0){
		while(wait(&status) == -1)	
			;
	}else{
		if(-1 == execvp(prog, nextargs)){
			perror("execvp");
			exit(-1);
		}
	}
}

char** extract_next_prog(char **argv, int *j, int i, int size){
	char **nextprog = NULL;
	int k = *j;
	int n = *j;
	if(NULL == (nextprog = (char**)malloc(sizeof(char*)* size))){
		perror("malloc");
		exit(-1);
	}
	for(; k< i; k++){
		nextprog[k-n] = argv[k];
	}
	nextprog[k-n] = NULL; 
	*j = ++k;
	return nextprog;

}

void print_next_prog(char* prog, char** nextprog){
	int i;
	printf("%s ", prog);
	for(i = 0; nextprog[i] != NULL; i++){
		printf("%s ", nextprog[i]);
	}
	printf("\n");
}


