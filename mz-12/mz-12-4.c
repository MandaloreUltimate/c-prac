#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

enum
{
    MODE = 0600
};

int
my_remainder(int a, int b)
{
    int rem = a % b;
    if (rem < 0) {
        rem += b;
    }
    return rem;
}

int
main(int argc, char *argv[]) {

    int count, num;
    sscanf(argv[1], "%d", &count);
    key_t key = ftok(argv[0], 0);
    if (key < 0) {
        perror("Failed to generate key: ");
        return 1;
    }
    int semid = semget(key, count, IPC_CREAT | MODE);
    if (key < 0) {
        perror("Failed to create shared semaphores array: ");
        return 1;
    }
    struct sembuf sem_ops = {0, 0, 0};
    setbuf(stdin, NULL);

    for (int i = 0; i < count; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Failed to fork: ");
            return 1;
        } else if (!pid) {
            sem_ops.sem_num = i;
            sem_ops.sem_op = -1;
            while (semop(semid, &sem_ops, 1) >= 0) {
                if (scanf("%d", &num) == EOF) {
                    semctl(semid, 0, IPC_RMID, 0);
                } else {
                    printf("%d %d\n", i, num);
                    fflush(stdout);
                    sem_ops.sem_op = +1;
                    sem_ops.sem_num = my_remainder(num, count);
                    semop(semid, &sem_ops, 1);
                }
                sem_ops.sem_num = i;
                sem_ops.sem_op = -1;
            }
            exit(0);
        }
    }
    struct sembuf sem_buf = {0, 1, 0};
    semop(semid, &sem_buf, 1);
    for (int i = 0; i < count; i++) {
        wait(NULL);
    }
    semctl(semid, 0, IPC_RMID, 0);
    exit(0);
}