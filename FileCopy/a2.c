/*
Name: Nathan Johnson		
Class: CPS 470				Section: 22342759			
Assignment: A2				Credit: 10
Started: January 28, 2018	Due: January 31, 2018

Problem Statement: Write a well documented, copmlete, and modular C-program
that will copy the given files to a specific destination directory using
file system call interface (open(), close(), read(), write(), stat(),
etc.). the program should copy only the regular files of specified
file-path(s). the program must preform the fallowing functions:

1. If given less than 2 args, print usage to stderr and terminate.

2. Last arg dose not exist or is not a dir, print usage to stder and
terminate.
calls to look at -> stat(), fpritnf(), exit()

3. For each argument, if the file to copy is a regular-file, copy it to
destination else print error message and return error.

4. When all copies are done print the count of files copied successfuly.

Problem: This program takes an input from the command line that will
take any number of files and copy them to a given dirctory. The command line
arguments are checked for sanity. Each individual file is checked to make sure
that it exists and is regular. The final destination is checked to make sure
that it exists and is a valid directory. Copy each file to the destination upon
success. Print the files copied.

Solution: The user will input any number of paths with a
destination path being the final path input. The program will check to see
if the corect amount of paths are input. In a case where there isn't enough
paths the program will print usage, and terminate. If there are enough
paths the program will then start moving though the list of paths and
start to copy the files to a destination. Files are first checked for sanity. 
Next the program create the destination file path in buildpath() by combining the 
source files name as well as the desired destination folder path.
In the makecp file we read in the file descriptors. If the data is valid, then the file
will be copied over to the new file. If the file is not regular or dosen't
exist the program will then skip over this entry. This process repeats
until there are no more files to copy.

Data-structure used: Arrays of char for the copying of files and keeping
track of what needs to be copied.

Accessing functions for the data stucture: standard c funcitons, as well
as sys/types, string, sys/stat, and fcntl.

Errors handled: Wrong amount of inputs given, non-existing file, existing
file destination, and non-regular file.

Limitations: Can only transfer files 1024 or smaller in size.

Acknowledgement: None

Functions used:
	- buildpath();
	- chkdst();
	- die();
	- isdir();
	- isregular();
	- makecp();
	- usage();
*/

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BFSIZE 1024

int main(int argc, char *argv[]){
	int chkdst(int, char**argv);
	int copyfile(char *src, char *dst);
	void usage(char *progname);
	int count, i;

	if(argc < 3){
		usage(argv[0]);
	}

	chkdst(argc, argv);
	
	for(i = 1; i < argc - 1; i++){
		if(copyfile(argv[i], argv[argc-1])){
			count++;
		}
	}
	printf("\n%d files successfully copied.\n", count);
	
	exit(0);
}

/* Problem: Users will sometimes enter the wrong input
   Solution: Print out the correct input format using the program name
*/
void usage(char *progname){
	fprintf(stderr,
	"Usage: ./%s <file-path 1> <file-path 2>...<file-path n> <dest>\n",
	progname);
	exit(1);
}

/* Problem: The program will crash or need to be terminated incorrectly on some occasions
   Solution: Print to stderr with the reason for the crash and terminate the program
*/
void die(char *reason){
	fprintf(stderr, "%s\n", reason);
	exit(1);
}

/* Problem: We need to verify the that the destination exists and is an actual directory
   Solution: Use isdir to check the path. If isdir returns a 0 the path is not an actual 
			 directory or doesn't exist. In this case end the program with the usage method. 
			 Otherwise the directory check is successful and a 1 is returned.
*/
int chkdst(int argc, char **argv){
	int isdir(char *path);
	void usage(char *progname);

	int retval;
	char *temp;
	
	temp = argv[argc-1];
	retval = isdir(temp);
	
	if(retval == 0){
		usage(argv[0]);
	}
	return retval;
}

/* Problem: We want to test if the path exists
   Solution: Use the system call S_ISDIR and stat to check if the given path is real.
			 Returns 1 on success and 0 on failure.
*/
int isdir(char *path){
	struct stat sb;
	if(stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)){
		return 1;
	}else{
		return 0;
	}
}

/* Problem: We need to check if the listed files are regular files
   Solution: For a given path, call the system call S_ISREG to check if the file is a
	         regular file. Returns the value from S_ISREG.
*/
int isregular(char *path){
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

/* Problem: We want a primary method to make all of the necessary calls and checks
		    for a file copy.
   Solution: For a given destination and source file this meathod will allocate the
			 memory for the destination path pointer. It will then enter a loop if
			 the file that is being passed is a regular file. Next it will take the
			 path, source code,  destination, and the dest_path and it will create the
			 target of the copy using buildpath(). The method will then open the input
			 file and the destination file and copy from the input to the destination.
			 Returns 1 on success and 0 on failure.
*/
int copyfile(char *src, char*dst){
	void buildpath(char *src, char *dst, char *dest_path);
	int isregular(char *path);
	int makecp(int filedes_in, int filedes_out);

	char buffer[BFSIZE], *dest_path;
	int fdi, fdo, i;

	dest_path = buffer;
	
	if(isregular(src)){
		buildpath(src, dst, dest_path);

		fdi = open(src, O_RDONLY);
		if(fdi == -1){
			perror(src);
		}
	
		fdo = open(dest_path, O_CREAT | O_TRUNC | O_WRONLY |
O_EXCL, 0644);
		if(fdo == -1){
			perror(src);
		}

		if(!(makecp(fdi, fdo)) || fdi == -1 || fdo == -1){
			close(fdi);
			close(fdo);
			return 0;
		}else{
			close(fdi);
			close(fdo);
			return 1;
		}
	}else{
		printf("%s: Not a regular file\n", src);
	}
}

/* Problem: We want to copy a file
   Solution: After allocating buffer space in memory, the method will open the
		     input file in read(). If the file opens correctly the method will then
			 write to the destination file. Returns 1 on success, 0 on failure.
*/
int makecp(int filedes_in, int filedes_out){
	char buffer[BFSIZE];
	size_t bytesRead, bytesWritten;

	if((bytesRead = read(filedes_in, &buffer, BFSIZE)) > 0){
		bytesWritten = write(filedes_out, &buffer, (size_t) bytesRead);
		return 1;
	}else{
		return 0;
	}
}

/* Problem: We need to build a path for each destination file based upon the given 
			filename/filepath as well as the given destination path.
   Solution: The source file is stripped of its folders and backslashes in the pathname.
			 The new path is allocated memory and then the stripped source file is added to
			 the new path. The destination path is then concatenated to the new path creating
			 the final and complete destination file path. 
*/
void buildpath(char *src, char *dst, char *dest_path){
	size_t dest_path_len;
	int i;
	char *newPath;

	dest_path_len = strlen(dst) + 1;
	newPath = (char*) malloc(dest_path_len);
	strncpy(newPath, dst, dest_path_len);
	src = strrchr(src, '/');

	dest_path_len += 1 + strlen(src);
	newPath = (char*) realloc(newPath, dest_path_len);
	strncat(newPath, src, dest_path_len);

	strncpy(dest_path, newPath, dest_path_len);
	free(newPath);
}


