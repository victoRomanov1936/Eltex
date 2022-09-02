#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(void) {
  int id = fork();

  if(id == 0) {
    printf("3My pid %d\tMy ppid %d\n", getpid(),getppid());  
    id = fork();
    if(id == 0) {
      printf("6My pid %d\tMy ppid %d\n", getpid(),getppid()); 
    } else {
        id = fork();
        if(id == 0) {
          printf("5My pid %d\tMy ppid %d\n", getpid(),getppid()); 
        } else {
          wait(NULL);
        }
    }
    wait(NULL);
  } else {
      printf("1My pid %d\n", getpid());
      id = fork();
    if(id == 0) {
      printf("2My pid %d\tMy ppid %d\n", getpid(),getppid()); 
      id = fork(); 
      if(id == 0) {
        printf("4My pid %d\tMy ppid %d\n", getpid(),getppid()); 
      } else {
        wait(NULL);
      }
    } else {
      wait(NULL);
    }
    wait(NULL);
  }
  return 0;
}
