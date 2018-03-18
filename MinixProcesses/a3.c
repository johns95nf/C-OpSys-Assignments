/*
Name: Nathan Johnson
Class: CPS 470				Section: 22342759
Assignment: A3				Credit: 10
Started: February 11, 2018	Due: February 12, 2018

Problem Statement: Write a complete C-program to determine the number of
simultaneous processes Minix can support for a single user (both for super
user root and ordinary user ast - account already exists in Minix). Be
aware that a users' login shell is itself a process. Once this has been
determined, any processes created will have to be terminate terminated.
Created processes wil be kept track of, in a singly linked list with node
structure at runtime.

Problem: Find the maximum number of processes Minix can run simultaneously
per user. After testing, print the number of processes found. Then
terminate and print each process individually.

Solution: Create a process using fork() until fork fails, keeping track of
the number of processes found and adding each process ID into a singly
linked list. Once fork() fails, terminate the process using the process ID
and SIGKILL. Print each process ID after the termination of the respective
process. Finally, free the node used from memory.

Data structures used: Linked list

Accessing functions for the data structure: None

Errors handled: None

Limitations: None

Acknowledgements: None

Functions/methods used:
	- freenode();
	- getnode();
	- printpcount();
	- printpkill();
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
	int pid;
	struct lnode *next;
} lnode;

int main(void){
	lnode *getnode(void);
	void freenode(lnode *node);
	int kill(int pid, int sig);
	void printpcount(int pcount);
	void printpkill(int currentp);
	
	int count, currentp;
	lnode *current, *node;

	current = NULL;
	node = NULL;
	count = 0;
	currentp = 1;
	
	while(currentp > 0){
		current = (void*)getnode();
		currentp = fork();
		count = count + 1;
		
		if(currentp < 0) break;
		else if(currentp == 0) pause();
		else if(currentp > 0){
			current->pid = currentp;
			current->next = (void*)node;
			node = current;
		}
	}

	printpcount(count);
	current = node;
	
	while(current){
		currentp = current->pid;
	
		kill(currentp, SIGKILL);
		printpkill(currentp);
		
		node = current;
		current = (void*)current->next;

		freenode(node);
	}
	
	exit(0);
}

/*
Problem: We need to dynamically allocate memory for each node.
Solution: Create method getnode to allocate memory for each node at
	  runtime.
*/
lnode *getnode(void){
	return malloc(sizeof(lnode));
}

/*
Problem: We need to free the memory for each node at program termination.
Solution: Create method freenode which uses the free system-call to free
	  up the memory used by the node.
*/
void freenode(lnode *node){
	free(node);
}

/*
Problem: We need to print the number of processes found by the program.
Solution: Use fprintf to print the process found to sdout.
*/
void printpcount(int pcount){
	fprintf(stdout, "For this user: %d\n", pcount);
}

/*
Problem: We need to print each process as it is terminated.
Solution: Use fprintf to print the process before termination stdout.
*/
void printpkill(int currentp){
	fprintf(stdout, "process %d killed\n", currentp);
}
