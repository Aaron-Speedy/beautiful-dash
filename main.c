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
  int path_env_len = strlen(path_env);
  if(path_env == NULL) {
    fprintf(stderr, "No PATH environment variable was found\n");
    exit(1);
  }

  // Store path dirs in array
  int num_elements = 0;
  int num_elements_allocated = BASE_PATH_DIRS_LENGTH;
  String *path_dirs = malloc(sizeof(String) * num_elements_allocated);
  int start = 0;
  for(int i = 0; i < path_env_len - 1; i++) {
    if(path_env[i] == ':') {
      num_elements++;

      if(num_elements > num_elements_allocated) {
        num_elements_allocated += 3;
        String *new_path_dirs = malloc(sizeof(String) * num_elements_allocated);
        memcpy(new_path_dirs, path_dirs, sizeof(String) * (num_elements - 1));
        free(path_dirs);
        path_dirs = new_path_dirs;
      }

      String new_path = {
        .str = path_env + start,
        .len = i - start
      };
    }
  }
  
  char *input = readline("$ ");
  return 0;
}
