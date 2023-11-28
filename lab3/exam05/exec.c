#include <stdio.h>
#include <unistd.h>

int main() {
  // 새로운 프로그램을 실행합니다.
  execlp("ls", "ls", "-l", NULL);

  // 실패한 경우
  printf("실패했습니다.\n");

  return 0;
}
