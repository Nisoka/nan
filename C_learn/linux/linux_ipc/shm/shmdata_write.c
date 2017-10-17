#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#if 1
#include <sys/shm.h>
#else
#include "win_linux_declare.h"
#endif
#include "shmdata.h"


int main(){
    int running = 1;
    void *shm = NULL;
    struct shared_use_st *shared;
    char buffer[TEXT_SIZE+1];

    int shmid;

    shmid = shmget((key_t)SHM_KEY_1, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if(shmid == -1){
        printf("shmget failed!\n");
        return 0;
    }

    shm = shmat(shmid, 0, 0);
    if(shm == (void *)-1){
        printf("shmat failed!\n");
        return 0;
    }

    shared = (struct shared_use_st*)shm;
    while(running){
        while(shared->written == 1){
            printf("waitting read...\n");
            sleep(1);
        }
        printf("Enter message to send:\n");
        fgets(buffer, TEXT_SIZE, stdin);
        strncpy(shared->text, buffer, TEXT_SIZE);

        shared->written = 1;

        if(strncmp(shared->text, "end", 3) == 0){
            running = 0;
        }
    }

    if(shmdt(shm) == -1){
        printf("shmdt failed!\n");
        return 0;
    }

//    if(shmctl(shmid, IPC_RMID, 0) == -1){
//        printf("shmctl failed!\n");
//        return 0;
//    }

    return 1;
}


