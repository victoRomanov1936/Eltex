#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>  

typedef struct thread_arg{
  pthread_mutex_t mutex;
  int score[5];
} arg;

void *ThreadClient(void *args) {
  int *score;
  int score_num;
  arg* mutex = (arg*) args;
  int products = 0;
  int need_prod = 4500 + rand()%1000;  
  while(products < need_prod){
    pthread_mutex_lock(&(mutex->mutex));  
      score_num = rand()%5;
      score = (&(mutex->score[score_num]));
      products += *score;

      if (*score != 0) {
        printf("%ld:\tget %d in %d shop\t%d/%d\n",pthread_self(),*score, score_num+1, products, need_prod);
      }
      *score = 0;
    pthread_mutex_unlock(&(mutex->mutex));  
    sleep(2);
  }
}

void *ThreadLoader(void *args) {
  arg* mutex = (arg*) args;
  int *score;
  int score_num;
  for(;;) {
    pthread_mutex_lock(&(mutex->mutex));  
      score_num = rand()%5;
      score = (&(mutex->score[score_num]));
      *score += 500;
      printf("loader: \t\tgive 500 in %d shop\tproducts: %d\n", score_num+1, *score);
    pthread_mutex_unlock(&(mutex->mutex));
    sleep(1);
  }  
}

int main() {
  arg param;
  pthread_mutex_init(&(param.mutex), NULL);

  pthread_t threads_client[3], thread_loader;

  srand(time(NULL));

  int *s;
  int score[] = {          
     950 + rand()%100,      
     950 + rand()%100, 
     950 + rand()%100,
     950 + rand()%100,
     950 + rand()%100,
  };


  for(int i = 0;i < 5;i++) {
    param.score[i] = score[i];
    printf("shop%d: %d\n",i+1, param.score[i]);
  } 
  printf("\n");

  for(int i = 0; i < 3; i++) {
    pthread_create(&threads_client[i], NULL, ThreadClient, &param);
  }

  pthread_create(&thread_loader, NULL, ThreadLoader, &param);
  
  for(int i = 0; i < 3; i++) {
    pthread_join(threads_client[i], (void **) &s);
  }

  pthread_mutex_destroy(&(param.mutex));
  return 0;
}
