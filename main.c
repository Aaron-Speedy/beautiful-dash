#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "config.h"

#include <readline/readline.h>

typedef struct {
  char *str;
  int len;
} String;

int main() {
  // Get contents of path env variable
  char *path_env = getenv("PATH");
  if(path_env == NULL) {
    fprintf(stderr, "No PATH environment variable was found\n");
    exit(1);
  }
  int path_env_len = strlen(path_env);

  // Store path dirs in an array
  int num_path_dirs = 0;
  int num_path_dirs_allocated = BASE_PATH_DIRS_LENGTH;
  String *path_dirs = malloc(sizeof(String) * num_path_dirs_allocated);
  int start = 0;
  for(int i = 0; i <= path_env_len; i++) {
    if(path_env[i] == ':' || path_env[i] == 0) {
      num_path_dirs++;

      if(num_path_dirs > num_path_dirs_allocated) {
        num_path_dirs_allocated += 3;
        path_dirs = realloc(path_dirs, sizeof(String) * num_path_dirs_allocated);
      }

      path_dirs[num_path_dirs - 1].str = path_env + start;
      path_dirs[num_path_dirs - 1].len = i - start + 1;
      path_env[i] = '/';

      start = i + 1;
    }
  }

  char *input = readline("$ ");
  int input_len = strlen(input);

  char *program = malloc(path_env_len);
  for(int i = 0; i < num_path_dirs; i++) {
    memcpy(program, path_dirs[i].str, path_dirs[i].len);
    memcpy(program + path_dirs[i].len, input, input_len);
    memset(program + path_dirs[i].len + input_len, 0, path_env_len - path_dirs[i].len - input_len);
    if(!execl(program, NULL)) break;
  }
  return 0;
}
