#include <stdio.h>
#include <dirent.h>
#include <readline/readline.h>
#include <unistd.h>

void get_execs(char *path);

void get_execs(char *path) {
  DIR *d;
  struct dirent *dir;
  d = opendir(path);

  if(d) {
    while((dir = readdir(d)) != NULL) {
      char file_path[1024];
      snprintf(file_path, sizeof(file_path), "%s/%s", path, dir->d_name);
      if(dir->d_type == DT_DIR) {
        if(dir->d_name[0] == '.' && (dir->d_name[1] == '\0' || (dir->d_name[1] == '.' && dir->d_name[2] == '\0'))) continue;
        get_execs(file_path);
      }
  
      else if(access(file_path, X_OK) == 0) printf("%s\n", file_path);
    }
    
    closedir(d);
  }
}

int main() {
  get_execs(".");

  return 0;
}
