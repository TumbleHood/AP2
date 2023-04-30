#include "codec.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUM 16
#define TASK_SIZE 1024

#define ENCRYPTION 0
#define DECRYPTION 1

#define EVER ;;

typedef struct task {
    char data[TASK_SIZE+1];
} task_t;

typedef struct task_queue {
    task_t val;
    struct task_queue *next;
} task_queue_t;

void enqueueTask(task_t task);
task_t* dequeueTask();
void executeTask(task_t *task);
void* startThread(void *args);

task_queue_t *taskQueue = NULL;
int taskCount = 0;
pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;
int key = 0;
int mode;