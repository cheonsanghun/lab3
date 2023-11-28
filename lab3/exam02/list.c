#include <stdio.h>
#include <dirent.h>
#include <string.h>

void list_dir(const char *dir) {
  DIR *dp;
  struct dirent *entry;

  dp = opendir(dir);
  if (dp == NULL) {
    printf("디렉터리 '%s'을(를) 열 수 없습니다.\n", dir);
    return;
  }

  while ((entry = readdir(dp)) != NULL) {
    if (entry->d_type == DT_DIR) {
      if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        printf("%s/\n", entry->d_name);
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
        list_dir(path);
      }
    } else {
      printf("%s\n", entry->d_name);
    }
  }

  closedir(dp);
}

int main() {
  list_dir("/home/user1/lab3");
  return 0;
}

