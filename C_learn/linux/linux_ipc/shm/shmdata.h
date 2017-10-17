#ifndef SHMDATA_H
#define SHMDATA_H

#define TEXT_SIZE 2048

#define SHM_KEY_1   123456

struct shared_use_st{
    int written;
    char text[TEXT_SIZE];
};




#endif // SHMDATA_H
