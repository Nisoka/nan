#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT 0
#define OUTPUT 1

void main()
{
  int file_descriptors[2];
  pid_t pid;
  char buf[256];
  int returned_count;
  pipe(file_descriptors);
  
  if((pid = fork()) == -1){
    printf("Error in fork \n");
    exit(1);
  }
  
  if(pid == 0){
    printf("in the child process.... \n");
    close(file_descriptors[INPUT]);
    write(file_descriptors[OUTPUT], "test data ", strlen("test data"));
    exit(0);
  }else{
    printf("in the parent process... \n");
    close(file_descriptors[OUTPUT]);
    returned_count = read(file_descriptors[INPUT], buf, sizeof(buf));
    printf(" %d bytes read, read is : %s \n", returned_count, buf);
  }
}
  
