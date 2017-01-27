#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<sys/sem.h>
#include<fcntl.h>

union semun
{
int val;
};
void sem_init();
void sem_p();
void sem_v();
void sem_destroy();

