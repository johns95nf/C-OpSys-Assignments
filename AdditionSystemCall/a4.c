/*
Name: Nathan Johnson
Class: CPS 470				Section: 22342759
Assignment: A4				Credit: 10
Started: February 25, 2018	Due: February 26, 2018

Problem Statement: Code segments is given below to implement a new system call 
that takes two integers values and returns their differences, products and sum.

	void mycall(int x, int y, int *xminusy, int *xtimesy, int *xplusy);

User process sends a message type m1 to MM for mycall (uses unused call
number 58 of /usr/src/mm/table.c) using sendrec(). MM (no interaction with
kernel is needed) returns the result of the operation or the error.
 
Problem: We want to give two integers to a system call and have it return
three values: the first subtracting integer one from integer two, the
second multiplying the two integers together, and the third adding the two
integers together. 

Solution: Create a driver program, a4.c, that calls another C program,
mycall.c. mycall.c takes five parameters: two integers and two integer
pointers. mycall.c completes the system call, do_mycall(), by passing the
memory manager the two given integers as well as assigning the pointers
given to the values output by the system call in m1_i1, m1_i2, and m1_i3. 

Data structures used: None

Acessing functions for the data structures: None

Errors handled: System call failure

Limitations: None

Acknowledgements: None

Functions/methods used:
	- int mycall(int a, int b, int *d, int *p, int *s);

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mycall.c"

int main(void)
{
	int aa, bb, dd, pp, ss;
	int mycall(int a, int b, int *d, int *p, int *s);
	
	aa = 32; bb = 23;
	
	if(mycall(aa, bb, &dd, &pp, &ss) < 0) {
		fprintf(stderr, "Mycall failed..\n"); exit(1);
	}
	printf("Difference: %d, product: %d, sum: %d\n", dd, pp, ss);
	
	exit(0);
}
