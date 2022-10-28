#include <pthread.h>
#include <malloc.h>
#include <string.h>

const int MAX_SIZE = 28;

char *error_v[] = {
  "0.No such file or directory",
  "1.Arg list too long",
  "2.Exec format error",
  "3.Bad file number",
  "4.Not enough core",
  "5.Permission denied",
  "6.File exists",
  "7.Cross-device link",
  "8.Invalid argument",
  "9.Too many open files"
};

static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static void make_key() {
  (void) pthread_key_create(&key, NULL); 
}

char* func_error(int index) {

  void *ptr;
  
  (void) pthread_once(&key_once, make_key);
  
  if(pthread_getspecific(key) == NULL) {
  	ptr = malloc(MAX_SIZE * sizeof(char));
  	(void) pthread_setspecific(key, ptr);
  }
  
  char *str = (char*) ptr; 
  strcpy(str, error_v[index]);
 
  return str;
}
