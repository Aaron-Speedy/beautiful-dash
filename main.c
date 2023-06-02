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
  while(1) {
    char *input = readline("$ ");
    char *cmd_argv[1] = {strtok(input,  " ")};
    if(!strcmp(cmd_argv[0], "cd")) {
      setenv("PWD", "/", 1);
      chdir("/");
    }
    else {
      pid_t pid = fork();
      if(pid == 0) execvp(cmd_argv[], NULL);
      else if(pid < 0) perror("Fork");

      int status;
      waitpid(pid, &status, 0);
    }
    free(input);
  }
  return 0;
}
