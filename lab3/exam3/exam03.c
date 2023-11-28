#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

//2번 문제보다 발전된 코딩

void listFilesRecursively(const char *basePath) {
    struct dirent *dp;
    struct stat st;

    // 디렉토리 열기
    DIR *dir = opendir(basePath);

    // 디렉토리 열기 실패 시 종료
    if (!dir) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    // 디렉토리 내용 읽기
    while ((dp = readdir(dir)) != NULL) {
        // 현재 디렉토리와 이전 디렉토리는 무시
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", basePath, dp->d_name);

        // 파일 정보 얻기
        if (stat(path, &st) == -1) {
            perror("Error getting file status");
            exit(EXIT_FAILURE);
        }

        // 디렉토리인 경우 재귀적으로 탐색
        if (S_ISDIR(st.st_mode)) {
            printf("%s  ", dp->d_name);
            listFilesRecursively(path);
        } else {
            // 파일인 경우 출력
            printf("%s  ", dp->d_name);
        }
    }
    printf("\n");
    // 디렉토리 닫기
    closedir(dir);
}

int main() {
    // 현재 디렉토리부터 시작
    char basePath[1024];
    getcwd(basePath, sizeof(basePath));

    printf(".:\n");
    listFilesRecursively(basePath);

    return 0;
}

