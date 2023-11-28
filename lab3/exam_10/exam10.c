#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h> 

#define BUFFER_SIZE 1024

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// 세마포어 초기화
int init_semaphore(int semid, int semnum, int value) {
    union semun arg;
    arg.val = value;
    return semctl(semid, semnum, SETVAL, arg);
}

// 세마포어 감소 (P 연산)
void semaphore_wait(int semid, int semnum) {
    struct sembuf buf;
    buf.sem_num = semnum;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;
    semop(semid, &buf, 1);
}

// 세마포어 증가 (V 연산)
void semaphore_signal(int semid, int semnum) {
    struct sembuf buf;
    buf.sem_num = semnum;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;
    semop(semid, &buf, 1);
}

int main(int argc, char *argv[]) {
    char source_filename[256];
    char destination_filename[256];

    printf("Enter source file name: ");
    scanf("%s", source_filename);

    printf("Enter destination file name: ");
    scanf("%s", destination_filename);

    key_t key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // 공유 메모리 생성
    int shmid = shmget(key, BUFFER_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // 세마포어 생성
    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // 세마포어 초기화 (0으로 설정)
    if (init_semaphore(semid, 0, 0) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    // 파일 열기
    int source_fd = open(source_filename, O_RDONLY);
    if (source_fd == -1) {
        perror("open source file");
        exit(EXIT_FAILURE);
    }

    // 공유 메모리 연결
    char *shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (char *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // 부모 프로세스 (읽는 프로세스)
    if (fork() > 0) {
        // 세마포어를 이용하여 자식 프로세스가 공유 메모리에 쓰기를 시작할 때까지 기다림
        semaphore_wait(semid, 0);

        // 공유 메모리의 내용을 출력 파일에 쓰기
        FILE *destination_file = fopen(destination_filename, "w");
        if (destination_file == NULL) {
            perror("fopen destination file");
            exit(EXIT_FAILURE);
        }

        fprintf(destination_file, "%s", shared_memory);

        fclose(destination_file);

        // 세마포어를 이용하여 공유 메모리에 쓰기가 끝났음을 알림
        semaphore_signal(semid, 0);

        // 부모 프로세스 종료 전에 자식 프로세스가 종료되기를 기다림
        wait(NULL);
    }
    // 자식 프로세스 (쓰는 프로세스)
    else {
        // 파일의 내용을 공유 메모리에 쓰기
        ssize_t bytes_read;
        while ((bytes_read = read(source_fd, shared_memory, BUFFER_SIZE)) > 0) {
            // 세마포어를 이용하여 부모 프로세스가 공유 메모리에 읽기를 끝냈음을 알림
            semaphore_signal(semid, 0);

            // 세마포어를 이용하여 부모 프로세스가 다시 공유 메모리에 읽기를 시작할 때까지 기다림
            semaphore_wait(semid, 0);
        }

        // 세마포어를 이용하여 부모 프로세스가 공유 메모리에 읽기를 끝냈음을 알림
        semaphore_signal(semid, 0);

        // 자식 프로세스 종료
        exit(EXIT_SUCCESS);
    }

    // 공유 메모리 해제
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // 공유 메모리 삭제
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    // 세마포어 삭제
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    close(source_fd);

    return 0;
}
