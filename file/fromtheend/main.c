#include <stdio.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#include <malloc.h>
#include <stdlib.h>

void InputString(char *buffer, int *i);
void ReadFileFromTheEnd(char *buffer_read,int size, int fd);

int main(void) {
  int i = 0;
  char *buffer = malloc(sizeof(char));
  InputString(buffer, &i);
  char *buffer_read = malloc(sizeof(char)*i);
  
  int fd = open("1.txt", O_CREAT |  O_RDWR , S_IREAD|S_IWRITE );
  if(fd == -1) {
    perror("Failed to create and open the file");
    exit (1);
  }  
  
  write(fd, buffer, i*sizeof(char)); 
  ReadFileFromTheEnd(buffer_read, i, fd);
 
  close(fd);
  free(buffer);
  free(buffer_read);
  return 0;
}

void InputString(char *buffer, int *i) {
  
  char symbol;
  symbol = getchar();
  while(symbol!='\n') {
    buffer[*i] = symbol;
    *i+=1;
    buffer = realloc(buffer,*i);
    symbol = getchar();
  }
}

void ReadFileFromTheEnd(char *buffer_read,int size, int fd) {
  char *symbol = malloc(sizeof(char));
  for(int i = 1; i <= size; i++) {
    int rek = lseek(fd, -i, SEEK_END);
    read(fd, symbol, 1*sizeof(char)); 
    buffer_read[i-1] = symbol[0]; 
  }
   printf("%s", buffer_read);
   free(symbol); 
}
