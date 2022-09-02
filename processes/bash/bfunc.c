#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

#include <string.h>

#define BUFFER_WORDS 32
#define INIT_STRUCT 2
/*
int CdFunc(char **arguments){

}
int ExitFunc(char **arguments);	

char *command[] = {
  "cd",
  "exit"
};

int (*command_fun[])(char**)= {
  &CdFunc,
  &ExitFunc
};
*/

void BashLoop(void);

char *ReadUserString(void);
char **ParsingString(char *str);
int BashCommands(char **command_line);

int main(void) {
  BashLoop();
  return EXIT_SUCCESS;
}


void BashLoop() {
  /* 
  */
  char *user_string;
  char **user_words;
  int status = 1;
  
  while(status != 0) {
    printf(": ");
    user_string = ReadUserString();
    user_words = ParsingString(user_string);
    status = BashCommands(user_words);
  }
  free(user_string);
  free(user_words);

}

/*
   ReadUserString function use getline and returns an array 
   of character
*/
char *ReadUserString(void) {
  char *user_string;
  size_t len = 0;
  getline(&user_string, &len, stdin);
  return user_string;
}

/*
   ParsingString function use strtok and fills an array with its values
   returns array of string
*/
char **ParsingString(char *str) {
  char *word;
  int i = 0, buffer = BUFFER_WORDS;
  char **words = malloc(buffer * sizeof(char*));
  if(words == NULL) {
    perror("allocation error");
    exit(EXIT_FAILURE);
  }
  
  word = strtok(str, " \n\t");
  
  while(word != NULL) {
    words[i] = word;
    i++;
    if(i >= buffer) {
      buffer = buffer + BUFFER_WORDS;
      words = realloc(words, buffer * sizeof(char*));
      if(words == NULL) {
        perror("allocation error");
        exit(EXIT_FAILURE);
      }
    }
    word = strtok(NULL, " \n\t");
  }
  words[i] = NULL;	
  return words;
}


int BashCommands(char **command_line) {
  pid_t pid;
  
  if(command_line[0] == NULL) {
    return 1;
  }
  
  pid = fork();
  if(pid == 0) {
   if( execvp(command_line[0], command_line) == -1) {
      perror("bash");
    }
    exit(EXIT_FAILURE);
  } else if(pid < 0) {
    perror("fork error");
  } else {
    wait(NULL);
  }
  return 1;
    
}






