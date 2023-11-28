#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// 시그널 핸들러 함수
void signalHandler(int signum) {
  printf("Received signal: %d\n", signum);
}

void signalEnd(int signum) {
  printf("Received signal: %d\n", signum);
  exit(1);
}
int main() {
  // SIGALRM 시그널에 대한 핸들러 등록
  signal(SIGALRM, signalHandler);

  printf("Setting an alarm for 5 seconds...\n");

  // 5초 후에 SIGALRM 시그널을 보내도록 알람 설정
  alarm(5);

  // 현재 프로세스의 시그널 마스크를 가져와서 이전 상태를 저장
  sigset_t oldMask, newMask;
  sigemptyset(&newMask);
  sigaddset(&newMask, SIGINT); // SIGINT 시그널을 차단
  sigprocmask(SIG_BLOCK, &newMask, &oldMask);

  printf("Blocking SIGINT signal for 10 seconds...\n");

  // 10초 동안 프로그램 실행 (SIGINT는 블록됨)
  while (1) {
    if (sigpending(&newMask) == 0) {
      // SIGINT 시그널이 대기 중인 경우
      break;
    }
    sleep(1);
  }

  // SIGINT 시그널에 대한 핸들러 호출
  signalHandler(SIGINT);

  printf("Unblocking SIGINT signal...\n");

  // 이전 상태로 시그널 마스크 복원
  sigprocmask(SIG_SETMASK, &oldMask, NULL);

  // SIGINT에 대한 핸들러 등록 (블록되지 않음)
  signal(SIGINT, signalEnd);

  printf("Press Ctrl+C to terminate the program.\n");

  // 무한 루프
  while (1) {
    if (getchar() == 'c') {
      break;
    }
    sleep(1);
  }

  return 0;
}
