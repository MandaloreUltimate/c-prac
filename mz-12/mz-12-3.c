#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

enum { MODE = 0660 };

struct Message
{
    long mtype;
    long long val1;
    long long val2;
};

int
main(int argc, char *argv[])
{
    struct Message msg;
    key_t key;
    int n;
    long long val1, val2, maxval;
    sscanf(argv[1], "%d", &key);
    sscanf(argv[2], "%d", &n);
    sscanf(argv[3], "%lld", &val1);
    sscanf(argv[4], "%lld", &val2);
    sscanf(argv[5], "%lld", &maxval);

    int msgid = msgget(key, IPC_CREAT | MODE);
    if (msgid < 0) {
        perror("Failed to create message queue: ");
        return 1;
    }

    int msg_size = sizeof(msg) - sizeof(msg.mtype);

    for (int i = 0; i < n; i++) {
        pid_t pid;
        if ((pid = fork()) < 0) {
            msgctl(msgid, IPC_RMID, NULL);
            while (wait(NULL) > 0);
            return 1;
        } else if (!pid) {
            for (;;) {
                int rcvd = msgrcv(msgid, &msg, msg_size, i + 1, 0);
                if (rcvd < 0) {
                    exit(0);
                }

                printf("%d %lld\n", i, msg.val1 + msg.val2);
                fflush(stdout);

                if (maxval < msg.val1 + msg.val2) {
                    exit(0);
                }

                long long sum = msg.val1 + msg.val2;
                msg.val1 = msg.val2;
                msg.val2 = sum;
                msg.mtype = sum % n + 1;

                int sent = msgsnd(msgid, &msg, msg_size, IPC_NOWAIT);
                if (sent < 0) {
                    exit(0);
                }
            }
        }
    }

    msg.val1 = val1;
    msg.val2 = val2;
    msg.mtype = 1;
    msgsnd(msgid, &msg, msg_size, IPC_NOWAIT);

    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);

    while (wait(NULL) > 0);
    return 0;
}