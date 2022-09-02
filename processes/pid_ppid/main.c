#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(void) {
  pid_t id = fork();
  if(id == 0) {
   printf("Child: My pid %d\tMy ppid %d\n", getpid(),getppid());  
  } else {
    printf("Parents: My pid %d\tMy ppid %d\n", getpid(),getppid()); 
    wait(NULL);
  }
  return 0;
}
