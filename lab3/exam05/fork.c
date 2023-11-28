#include <stdio.h>
#include <unistd.h>

int main() {
  // 자식 프로세스를 생성합니다.
  pid_t pid = fork();

  // 자식 프로세스인 경우
  if (pid == 0) {
    printf("자식 프로세스입니다.\n");
  } else {
    // 부모 프로세스인 경우
    printf("부모 프로세스입니다.\n");

    // 자식 프로세스가 종료될 때까지 기다립니다.
    wait(NULL);

    printf("자식 프로세스가 종료되었습니다.\n");
  }

  return 0;
}
