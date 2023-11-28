#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// 사용자 정의 system 함수
int my_system(const char *command) {
    pid_t pid, wpid;
    int status;

    pid = fork(); // 자식 프로세스 생성

    if (pid == 0) { // 자식 프로세스
        // 자식 프로세스에서 명령 실행
        if (execl("/bin/sh", "sh", "-c", command, (char *)NULL) == -1) {
            perror("execl");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else { // 부모 프로세스
        do {
            // 자식 프로세스의 종료를 기다림
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        return WEXITSTATUS(status); // 명령의 종료 상태 반환
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "사용법: %s <명령> <인자>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 명령과 인자를 연결하여 command 문자열 생성
    char command[256];
    snprintf(command, sizeof(command), "%s %s", argv[1], argv[2]);

    // 사용자 정의 system 함수 호출
    int result = my_system(command);

    printf("명령이 상태 %d로 종료되었습니다.\n", result);

    return 0;
}

