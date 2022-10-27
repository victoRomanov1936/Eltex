#include <stdio.h>
#include <pthread.h>
long a = 0;

void *treads() {
  long tmp;
  for(int i = 0; i < 100000; i++) {
    tmp = a;
    tmp++;
    a = tmp;    
  }
}

int main() {
  int index[1000];
  pthread_t thread[1000];
  int *s;

  for(int i = 0; i < 1000; i++) {
    index[i] = i;
    pthread_create(&thread[i], NULL, treads, NULL);
  }

  for(int i = 0; i < 1000; i++) {
    pthread_join(thread[i], (void **) &s);
  }
  
  printf("%ld\n",a);
  return 0;
}
