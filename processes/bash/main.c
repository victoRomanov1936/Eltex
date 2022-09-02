#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

#include <string.h>

#define BUFFER_WORDS 32
#define COMMAND_ARRAY_SIZE 2

int CdFunc(char **arguments){
  if(arguments[1] == NULL) {
    arguments[1] = "/";
  }
  if (chdir(arguments[1]) != 0) {
    perror("Bash");
  } 
 return 1;
}
int ExitFunc(char **arguments){
 return 0;
}

int (*command_fun[])(char**)= {
  &CdFunc,
  &ExitFunc
};

char *command[] = {
  "cd",
  "exit"
};


void BashLoop(void);


char *ReadUserString(void);
char **ParsingString(char *str);
int BashCommands(char **command_line);
int CheckCommand(char **command_line);

int main(void) {
  BashLoop();
  return 0;
}


void BashLoop() {
  char *user_string;
  char **user_words;
  int status = 1;
  
  while(status != 0) {
    printf(": ");
    user_string = ReadUserString();
    user_words = ParsingString(user_string);
    if(user_words[0] != NULL) {
      status = CheckCommand(user_words);
    } 
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
int CheckCommand(char **command_line) {
  for(int i = 0; i < COMMAND_ARRAY_SIZE; i++) {
    if (strcmp(command_line[0], command[i]) == 0) {
      return (*command_fun[i])(command_line);
    }
  }
  return BashCommands(command_line);
}






