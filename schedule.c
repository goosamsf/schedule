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
	int status;
  char *end;
	char *prog = NULL;
	char **nextargs = NULL;
	int i,j=2;
	int numofprog = 0;
	long quantum = strtol(argv[1], &end, 10);
	if(*end){
		fprintf(stderr, "Invalid Quantum\n");
		exit(1);
	}

 /*Install the handler*/	

	struct sigaction sa;
	sigset_t empty;
	sigemptyset(&empty);
	//signal
	sa.sa_handler = tick_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(-1 == sigaction(SIGALRM, &sa, NULL)){
		perror("sigaction");
		exit(-1);
	}

	//Parse command line and put all of them into queue
	for(i=2; i < argc; i++) {
		if(!strcmp(argv[i], ":")){
			prog = argv[j];
			nextargs= extract_next_prog(argv, &j, i , i-j+1);
			node = createNode(prog,nextargs, 0);
			enqueue(&head,&tail, node);
			numofprog++;
			continue;
		}
		if(i == argc-1){
			prog = argv[j];
			nextargs = extract_next_prog(argv, &j, i+1, i-j+2);
			node = createNode(prog,nextargs, 0);
			enqueue(&head,&tail, node);
			numofprog++;
		}
	}
	i = 0;	
	pid_t wpid;
	//node_printer(&head,&tail);
	while(head){
		for(; i < numofprog; i++){
			pnode* curr = head;
			pid_t pid = fork();
			if(pid==0){
				raise(SIGSTOP);
				execvp(curr->prog, curr->args);
			}
			node = createNode(NULL, NULL, pid);
			enqueue(&head, &tail,node);
			dequeue(&head);
		}
		
		//set timer
		//node_printer(&head, &tail);
		//sleep(100);
		start_timer(quantum);
		kill(head->pid,SIGCONT);
		while((wpid = waitpid(head->pid,&status, WUNTRACED)) == -1){
			perror("waitpid");
			exit(-1);
		}

		stop_timer();
		if(WIFEXITED(status)){
			//Terminated;
			dequeue(&head);
		}

		if(WIFSTOPPED(status)){
			//Stopped
			push_back(&head, &tail, head->pid);
		}
		//continue process in head.
		//get waitpid
		//if terminated?
		//	dequeue
		//if stoppe1d?
		//	push_back

		
	}
	//sleep(10);
		
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


