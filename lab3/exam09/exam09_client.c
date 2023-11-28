#include <sys/types.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 32
#define QNAME "/my_queue"
#define PRIORITY 1

int main() {
    mqd_t qd;
    char send_data[BUFSIZE];

    struct mq_attr q_attr;
    q_attr.mq_maxmsg = 10; /* 큐에 들어갈 수 있는 최대 메시지 수 */
    q_attr.mq_msgsize = BUFSIZE; /* 최대 메시지 크기 */

    // 큐를 생성하고 여는 부분을 루프 바깥으로 이동
    if ((qd = mq_open(QNAME, O_CREAT | O_RDWR, 0600, &q_attr)) == -1) {
        perror("mq_open 실패");
        exit(1);
    }

    while (1) {
        memset(send_data, 0, BUFSIZE);
        printf("입력 > ");
        scanf("%s", send_data);

        if (mq_send(qd, send_data, strlen(send_data), PRIORITY) == -1) {
            perror("mq_send 실패");
            exit(1);
        }

        sleep(1); // 수신자가 메시지를 받을 동안 대기
    }
    return 0;
}

