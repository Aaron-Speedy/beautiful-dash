#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "config.h"

#include <readline/readline.h>

typedef struct {
  char *str;
  int len;
} String;

int main() {
start:
  while(1) {
    char *input = readline("$ ");
    char *cmd_argv[ARG_BUF_SIZE] = {strtok(input, " ")}; // This should probably become a dynamic array
    int cmd_argc = 1;
    
    // Get arguments
    int arg_buf_size = ARG_BUF_SIZE;
    while((cmd_argv[cmd_argc] = strtok(NULL, " ")) != NULL) {
      cmd_argc++;
      if(cmd_argc >= arg_buf_size) {fprintf(stderr, "Error: More than 64 arguments\n"); goto start;}
    }
    
    if(!strcmp(cmd_argv[0], "cd")) {
      setenv("PWD", cmd_argv[1], 1);
      chdir(cmd_argv[1]);
    }
    else {
      pid_t pid = fork();
      if(pid == 0) execvp(cmd_argv[0], NULL);
      else if(pid < 0) perror("Fork");

      int status;
      waitpid(pid, &status, 0);
    }
    free(input);
  }
  return 0;
}
