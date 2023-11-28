#include <sys/types.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 32
#define QNAME "/my_queue"
#define PRIORITY 1

char recv_data[BUFSIZE];

int main() {
    mqd_t qd;
    struct mq_attr q_attr, old_q_attr;
    int prio;

    q_attr.mq_maxmsg = 10; /* 큐에 들어갈 수 있는 최대 메시지 수 */
    q_attr.mq_msgsize = BUFSIZE; /* 최대 메시지 크기 */

    // 큐를 생성하고 여는 부분을 루프 바깥으로 이동
    if ((qd = mq_open(QNAME, O_RDWR | O_CREAT, 0600, &q_attr)) == -1) {
        perror("mq_open 실패");
        exit(1);
    }

    // O_NONBLOCK 플래그를 해제
    q_attr.mq_flags = 0;
    if (mq_setattr(qd, &q_attr, &old_q_attr)) {
        perror("mq_setattr 실패");
        exit(1);
    }

    while (1) {
        memset(recv_data, 0, BUFSIZE);

        if (mq_receive(qd, recv_data, BUFSIZE, &prio) == -1) {
            perror("mq_receive 실패");
            exit(1);
        }

        printf("사용자가 말했음: %s!\n", recv_data);
    }


    return 0;
}

