#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>

enum
{
    MODE = 0666,
    LIMIT = 10
};

void
operation(int *data, int ind1, int ind2, int value);

int
my_rand(int max)
{
    return (rand() / (RAND_MAX + 1.0) * max);
}

int
main(int argc, char *argv[])
{
    int count, nproc, iter_count, ind1, ind2, value;
    key_t key;
    unsigned seed;

    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &nproc);
    sscanf(argv[4], "%d", &iter_count);

    int semid = semget(key, count, IPC_CREAT | MODE);
    if (semid < 0) {
        perror("Failed to create shared semaphores array: ");
        return 1;
    }
    int shmid = shmget(key, count * sizeof(count), IPC_CREAT | MODE);
    if (shmid < 0) {
        perror("Failed to allocate shared memory space: ");
        return 1;
    }
    int *shmaddr = shmat(shmid, NULL, 0);
    for (int i = 0; i < count; i++) {
        scanf("%d", &shmaddr[i]);
        semctl(semid, i, SETVAL, 1);
    }

    for (int i = 5; i < nproc + 5; i++) {
        if (!fork()) {
            sscanf(argv[i], "%u", &seed);
            srand(seed);
            for (int j = 0; j < iter_count; j++) {
                ind1 = my_rand(count);
                ind2 = my_rand(count);
                value = my_rand(LIMIT);

                if (ind1 != ind2) {
                    struct sembuf sem_down[2], sem_up[2];
                    sem_down[0].sem_num = ind1;
                    sem_down[0].sem_op = -1;
                    sem_down[0].sem_flg = SEM_UNDO;

                    sem_down[1].sem_num = ind2;
                    sem_down[1].sem_op = -1;
                    sem_down[1].sem_flg = SEM_UNDO;

                    sem_up[0].sem_num = ind1;
                    sem_up[0].sem_op = 1;
                    sem_up[0].sem_flg = SEM_UNDO;

                    sem_up[1].sem_num = ind2;
                    sem_up[1].sem_op = 1;
                    sem_up[1].sem_flg = SEM_UNDO;

                    semop(semid, sem_down, 2);
                    operation(shmaddr, ind1, ind2, value);
                    semop(semid, sem_up, 2);
                }
            }
            exit(0);
        }
    }

    for (int i = 0; i < nproc; i++) {
        wait(NULL);
    }
    for (int i = 0; i < count; i++) {
        printf("%d ", shmaddr[i]);
        fflush(stdout);
    }
    putchar('\n');
    semctl(semid, 0, IPC_RMID, 0);
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, 0);
    return 0;
}
