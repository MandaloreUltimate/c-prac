#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

enum { MODE = 0666 };

int
main(int argc, char *argv[])
{
    int nproc;
    unsigned long long maxval;
    key_t key;

    sscanf(argv[1], "%d", &nproc);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%llu", &maxval);

    int semid = semget(key, nproc, IPC_CREAT | MODE);

    int shmid = shmget(key, 2 * sizeof(maxval), IPC_CREAT | MODE);


    int *shm_addr = shmat(shmid, NULL, 0);
    shm_addr[0] = shm_addr[1] = 0;

/*
    struct sembuf sem_up;
    sem_up.sem_num = 0;
    sem_up.sem_op = +1;
    sem_up.sem_flg = SEM_UNDO;
    semop(semid, &sem_up, 1); */

    semctl(semid, 0, SETVAL, 1);
    for (int i = 1; i < nproc; i++) {
        semctl(semid, i, SETVAL, 0);
    }

    for (int i = 0; i < nproc; i++) {
        pid_t pid = fork();
        if (!pid) {

            while (1) {
                struct sembuf sem_down;
                sem_down.sem_num = i;
                sem_down.sem_op = -1;
                sem_down.sem_flg = 0;
                if (semop(semid, &sem_down, 1) < 0 || maxval < shm_addr[0]) {
                    exit(0);
                }

                printf("%d %d %d\n", i + 1, shm_addr[0], shm_addr[1]);
                fflush(stdout);
                shm_addr[0]++;


                if (maxval < shm_addr[0]) {
                    for (int k = 0; k < nproc; k++) {
                        semctl(semid, k, SETVAL, 1);
                    }
                    exit(0);
                } else {
                    unsigned long long num = 1;
                    for (int i = 0; i < 4; i++) {
                        num *= shm_addr[0] % nproc;
                    }
                    num %= nproc;
             //       next_id = (unsigned long long) (val % nproc) * (val % nproc)
             //           * (val % nproc) * (val % nproc) % nproc;
   //                 unsigned short next_val = (unsigned long long) (val * val * val * val) % nproc;
         //       int nextid = (unsigned long long) (val % nproc) * (val % nproc) * (val % nproc) * (val % nproc) % nproc;
     //           struct sembuf vnext = {nextid, 1, SEM_UNDO};
                    shm_addr[1] = i + 1;
                    struct sembuf sem_up;
                    sem_up.sem_num = num;
                    sem_up.sem_op = +1;
                    sem_up.sem_flg = 0;
                    semop(semid, &sem_up, 1);
            //        if (semop(semid, &sem_up, 1) < 0) {
            //            exit(0);
            //        }

                }
            }
        }
    }
 //   wait(NULL);
 //   semctl(semid, 0, IPC_RMID, 0);
    while (wait(NULL) > 0);
    semctl(semid, 0, IPC_RMID, 0);
    shmdt(shm_addr);
    shmctl(shmid, IPC_RMID, 0);
    return 0;
}