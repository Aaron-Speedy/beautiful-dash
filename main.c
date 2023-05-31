#include <stdio.h>
#include <stdlib.h>
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

  printf("%s\n", path_env);

  // Store path dirs in an array
  int num_elements = 0;
  int num_elements_allocated = BASE_PATH_DIRS_LENGTH;
  String *path_dirs = malloc(sizeof(String) * num_elements_allocated);
  int start = 0;
  for(int i = 0; i <= path_env_len; i++) {
    if(path_env[i] == ':' || path_env[i] == 0) {
      num_elements++;

      if(num_elements > num_elements_allocated) {
        num_elements_allocated += 3;
        path_dirs = realloc(path_dirs, sizeof(String) * num_elements_allocated);
      }

      path_dirs[num_elements - 1].str = path_env + start;
      path_dirs[num_elements - 1].len = i - start;

      start = i + 1;
    }
  }

  for(int i = 0; i < num_elements; i++) {
    printf("TEST: %.*s\n", path_dirs[i].len, path_dirs[i].str);
  }
  
  char *input = readline("$ ");
  return 0;
}
