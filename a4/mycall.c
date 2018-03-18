/*
Names: Nathan Johnson
*/

#include <lib.h>

/*
Problem: We want to create a system call that is usable through
	other various C programs on this operating system.

Solution: Write a function that takes two inputs, a and b, and three
	pointers to output values: *d, *p, and *s. Pass a and b to the memory
	managements m1_i1 and m1_i2 and store the returned values in the pointer
	upon system call completion. Returns r on completion, which contains a
	value of 1 on success, and a value of -1 on failure. 
*/
int mycall(int a, int b, int *d, int *p, int *s)
{
	message m;
	int r;

	m.m1_i1 = a;
	m.m1_i2 = b;

	r = _syscall(MM, 58, &m);
	if(r < 0) return -1;
	*d = m.m1_i1;
	*p = m.m1_i2;
	*s = m.m1_i3;
	
	return r;
}
