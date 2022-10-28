#include "fun_er.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *treads() {
	char * exit_string = func_error(rand()%10);
 	printf("first fun call:%ld:   %s\n", pthread_self(), exit_string);

	sleep(1);
	
	exit_string = func_error(rand()%10);
 	printf("second fun call:%ld:  %s\n", pthread_self(), exit_string);
 	free(exit_string);
}

int main() {
	srand(time(NULL));
	pthread_t thread[10];
	int *s;
  
	for(int i = 0; i < 10; i++) {
    pthread_create(&thread[i], NULL, treads, NULL);
  }

  for(int i = 0; i < 10; i++) {
    pthread_join(thread[i], (void **) &s);
  }
  
  return 0;
}
