#include <stdio.h>
#include <dirent.h>

int main(void) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(".");
  if (dir == NULL) {
    printf("Could not open current directory.\n");
    return 1;
  }

  while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  closedir(dir);
  return 0;
}
