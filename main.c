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
  char *home;
  if((home = getenv("HOME")) == NULL) {fprintf(stderr, "Error: Home environment variable not found\n"); exit(0);}
  
  while(1) {
    // Get input
    char *input = readline("$ ");
    char *cmd_argv[ARG_BUF_SIZE] = {strtok(input, " ")}; // This should probably become a dynamic array
    if(cmd_argv[0] == NULL) goto end;
    int cmd_argc = 1;
    
    // Get arguments
    int arg_buf_size = ARG_BUF_SIZE;
    while((cmd_argv[cmd_argc] = strtok(NULL, " ")) != NULL) {
      cmd_argc++;
      if(cmd_argc >= arg_buf_size) {fprintf(stderr, "Error: More than 64 arguments\n"); goto end;}
    }

    // Built-ins
    if(!strcmp(cmd_argv[0], "cd")) {
      if(cmd_argc == 1) cmd_argv[1] = home;
      
      if(setenv("PWD", cmd_argv[1], 1)) {perror("Error"); goto end;};
      if(chdir(cmd_argv[1])) {perror("Chdir"); goto end;}
    }
    // Regular commands
    else {
      pid_t pid = fork();
      if(pid == 0) {
        if(execvp(cmd_argv[0], cmd_argv)) {perror("Error"); exit(1);};
      }
      else if(pid < 0) perror("Fork");

      int status;
      waitpid(pid, &status, 0); // Deal with errors
    }

end:
    free(input);
  }
  return 0;
}
