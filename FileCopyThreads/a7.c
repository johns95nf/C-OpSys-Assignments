/*
Name: Nathan Johnson, Alex Kaumeheiwa
Class: CPS 470		      	Section: 22342759
Assignment: A7		      	Credit: 10
Started: April 12, 2018		Due: April 12, 2018

Problem Statement:
  The statement and constraints are same A2 and:
- each file will be copied by a separate thread
  (wrapper function for [int copyfile(char *src, char *dst);
  is needed as thread executes a function with one argument
  only!)
- the count (of successfully copied files) is a shared/global
  variable among all threads and must be accessed using mutex and
  condition constructs
- use the Makefile given.

Problem: We want to copy files given by the user using threads. Update a global
variable counter safely using a mutex.

Solution: Create a C file a7.c that takes any number of files to copy and a file
destination. Check the destination for sanity.For every file to copy, create a
thread and call a copyfile function which verifies sanity and calls the function
that makes the actual copy. If the copy is successful, lock the mutex and
increment the global counter.

Data-structures used: Struct

Accessing functions for the data structures: None

Errors handled: Wrong amount of inputs given, non-existing file, existing
file destination, and non-regular file.

Limitations: None

Acknowledgements: Worked with partner

Functions used:
  - buildpath();
  - chkdst();
  - copyWrapper();
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
#include <pthread.h>
#include <errno.h>

#define BFSIZE 10240

typedef struct thdata{
  char *src;
  char *dst;
}thdata;

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]){
  int chkdst(int, char**argv);
  void usage(char *progname);
  void copyWrapper(void *ptr);
  int pthread_yield(void);
  int i;

  if(argc < 3){
    usage(argv[0]);
  }

  chkdst(argc, argv);

  pthread_t threads[argc-1];
  thdata data[argc-1];

  for(i = 1; i < argc - 1; i++){
    data[i].src = argv[i];
    data[i].dst = argv[argc-1];
  }

  for(i = 1; i < argc - 1; i++){
    pthread_create(&threads[i], NULL, (void *) &copyWrapper, (void *) &data[i]);
  }

  sleep(1);

  printf("%d copies copied successfully.\n", counter);
  exit(0);
}

/* Problem: We cannot send two arguments with a thread
   Solution: Send the thread with a struct and use a wrapper function to access
              the two parameters.
*/
void copyWrapper(void *ptr){
  int copyfile(char *src, char *dst);
  thdata *data;
  data = (thdata *) ptr;
  copyfile(data->src, data->dst);
  pthread_exit(0);
}

/* Problem: Users will sometimes enter incorrect command line arguments
   Solution: Print example usage to stderr
*/
void usage(char *progname){
  fprintf(stderr, "Usage: ./%s <file-path 1> <file-path 2> ... <file-path n> <dest>\n", progname);
  exit(1);
}

/* Problem: We want to print the issue when the program fails
   Solution: Create an easily callable function to print the termination reason
              and exit.
*/
void die(char *reason){
  fprintf(stderr, "%-15s\n", reason);
  exit(1);
}

/* Problem: A given destination could be irregular or not exist.
   Solution: Create a function that verifies sanity with isdir. Prints usage if
              not sane.
*/
int chkdst(int argc, char **argv){
  int isdir(char *path);
  void usage(char *progname);

  int retval;
  char* temp;

  temp = argv[argc-1];
  retval = isdir(temp);

  if(retval == 0){
    usage(argv[0]);
  }
  return retval;
}

/* Problem: We want to verify if a given path is a directory
   Solution: Use the system call S_ISDIR and return 1 on success 0 on failure
*/
int isdir(char *path){
  struct stat sb;
  if(stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)){
    return 1;
  }else{
    return 0;
  }
}

/* Problem: Files may be irregular
   Solution: Use the system call S_ISREG to verify that a file is regular
*/
int isregular(char *path){
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

/* Problem: We need a method to call all of the sanity checks as well as make
            the actual file copy.
   Solution: Use one method to verify a files sanity and then call the makecp
              method to make the copy based on the path specified in buildpath.
*/
int copyfile(char *src, char *dst){
  void buildpath(char *src, char *dst, char *dest_path);
  void die(char *reason);
  int isregular(char *path);
  int makecp(int filedes_in, int filedes_out);

  char buffer[BFSIZE], *dest_path;
  int fdi, fdo;

  dest_path = buffer;

  if(isregular(src)){

    buildpath(src, dst, dest_path);

    fdi = open(src, O_RDONLY);
    if(fdi == -1){
      fprintf(stderr, "%-15s: %s\n", src, strerror(errno));
    }


    fdo = open (dest_path, O_CREAT | O_TRUNC | O_WRONLY | O_EXCL, 0644);
    if(fdo == -1){
      fprintf(stderr, "%-15s: %s\n", src, strerror(errno));
    }

    if(!(makecp(fdi, fdo)) || fdi == -1 || fdo == -1){
      close(fdi);
      close(fdo);
      return 0;
    }else{
      close(fdi);
      close(fdo);
      printf("%-15s: Successfully copied by thread %lu\n", src, pthread_self());
      pthread_mutex_lock(&mutex);
      counter++;
      pthread_mutex_unlock(&mutex);
      return 1;
    }
  }else{
    printf("%-15s: Not a regular file\n", src);
  }

  return 0;
}

/* Problem: We need to copy a file
   Solution: Use read and write to copy a given file-descriptor to the destination
              file descriptor. Returns 1 on success, 0 otherwise.
*/
int makecp(int filedes_in, int filedes_out){
  char buffer[BFSIZE];
  size_t bytesRead;

  if((bytesRead = read(filedes_in, &buffer, BFSIZE)) > 0){
    write (filedes_out, &buffer, (size_t) bytesRead);
    return 1;
  }else{
    return 0;
  }

}

/* Problem: We need to build a destination path based on the input and destination
   Solution: Strip the source string of its folder and concatenate it to the end
              of the destination.
*/
void buildpath(char *src, char *dst, char *dest_path){
  size_t dest_path_len;
  char * newPath;
  char *i;

  dest_path_len = strlen(dst) + 1;
  newPath = (char*) malloc(dest_path_len);
  strncpy(newPath, dst, dest_path_len);

  i = strrchr(src, '/');
  if(i != NULL){
    src = i;
  }else{
    newPath = strncat(newPath, "/", strlen(src) + 1);
  }

  dest_path_len += 1 + strlen(src);
  newPath = (char*) realloc(newPath, dest_path_len);
  strncat(newPath, src, dest_path_len);
  strncpy(dest_path, newPath, dest_path_len);

  free(newPath);
}
