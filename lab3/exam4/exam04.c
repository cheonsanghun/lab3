#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 512

char practice_1[BUFSIZE] = "안녕하세요. 지금부터 타자연습을 시작합니다.";
char practice_2[BUFSIZE] = "요새 많이 피곤해진거 같아요.";
char practice_3[BUFSIZE] = "마음 편히 잘 수 있으면 좋겠어요.";

void displayPractice(char practice[]) {
    fprintf(stderr, "%s\n", practice);
}

void result(int time1, int time2, char text1[], char text2[], char text3[], int total_error) {
    int time_tmp;
    float typing_speed;

    time_tmp = time2 - time1;
    printf("걸린 시간: %d sec(초)\n", time_tmp);

    // 타수 계산 = (전체 타수 - 오타 수) / 시간 * 60
    typing_speed = (((strlen(text1) + strlen(text2) + strlen(text3)) - total_error) / (float)time_tmp) * 60;
    printf("분당 타수: %.2f 타\n", typing_speed);
}

int main() {
    time_t start_time, end_time;
    char a[20];
    char buf1[BUFSIZE];
    char buf2[BUFSIZE];
    char buf3[BUFSIZE];
    int wrong_count = 0;
    int sum = 0;
    double total = 0;

    // 초기화
    printf("타자 연습 프로그램입니다. 시작(Enter)\n");
    printf("------Enter Key를 누르세요------");
    fgets(a, sizeof(a), stdin);
    start_time = time(NULL);

    // 주어진 문자 입력
    displayPractice(practice_1);
    fgets(buf1, sizeof(buf1), stdin);

    displayPractice(practice_2);
    fgets(buf2, sizeof(buf2), stdin);

    displayPractice(practice_3);
    fgets(buf3, sizeof(buf3), stdin);
    end_time = time(NULL);

    // 타이핑까지 걸린 시간
    total = difftime(end_time, start_time);

    // 입력한 타이핑 전체 글자수
    sum = strlen(buf1) + strlen(buf2) + strlen(buf3);

    // 잘못 입력한 거 체크
    for (int i = 0; i < strlen(practice_1); i++) {
        if (buf1[i] != practice_1[i])
            wrong_count++;
    }
    for (int i = 0; i < strlen(practice_2); i++) {
        if (buf2[i] != practice_2[i])
            wrong_count++;
    }
    for (int i = 0; i < strlen(practice_3); i++) {
        if (buf3[i] != practice_3[i])
            wrong_count++;
    }

    // 평균 분당 타자수 계산
    total /= 60;
    double average_speed = sum / total;

    printf("\n잘못 타이핑한 횟수: %d\n", wrong_count);

    // result 함수 호출
    result(start_time, end_time, buf1, buf2, buf3, wrong_count);

    return 0;
}

