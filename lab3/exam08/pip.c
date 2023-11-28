#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int pipefd[2];
  char buf[1025];

  // 파이프 생성
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(1);
  }

  // 자식 프로세스 생성
  pid_t pid = fork();

  // 자식 프로세스
  if (pid == 0) {
    // 파이프에서 데이터를 읽는다.
    int n = read(pipefd[0], buf, sizeof(buf));
    printf("받은 데이터: %s\n", buf);
    exit(0);
  }

  // 부모 프로세스
  else {
    // 파이프에 데이터를 쓴다.
    sprintf(buf, "안녕하세요.");
    write(pipefd[1], buf, strlen(buf));
    wait(NULL);
  }

  return 0;
}
