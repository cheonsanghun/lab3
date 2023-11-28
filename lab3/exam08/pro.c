#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

int main() {
  // 공유 메모리 세그먼트 생성
  key_t key = ftok(".", 'a');
  int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget");
    exit(1);
  }

  // 자식 프로세스 생성
  pid_t pid = fork();

  // 자식 프로세스
  if (pid == 0) {
    // 공유 메모리 세그먼트에 매핑
    int *shmaddr = shmat(shmid, NULL, 0);
    *shmaddr = 10;
    printf("공유 메모리 세그먼트에 10을 썼습니다.\n");
    shmdt(shmaddr);
    exit(0);
  }

  // 부모 프로세스
  else {
    // 공유 메모리 세그먼트에 매핑
    int *shmaddr = shmat(shmid, NULL, 0);
    printf("공유 메모리 세그먼트의 값: %d\n", *shmaddr);
    shmdt(shmaddr);
  }
}
