#include "main.h"

void enqueueTask(task_t task){
    pthread_mutex_lock(&mutexQueue);

    task_queue_t *newTask = malloc(sizeof(task_queue_t));
    newTask->val = task;
    newTask->next = NULL;

    if (taskQueue == NULL){
        taskQueue = newTask;
    } else {
        task_queue_t *lastTask = taskQueue;
        while (lastTask->next != NULL){
            lastTask = lastTask->next;
        }
        lastTask->next = newTask;
    }

    taskCount++;

    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

task_t dequeueTask(){
    if (taskQueue == NULL){
        task_t emptyTask;
        emptyTask.empty = 1;
        return emptyTask;
    }

    task_t task = taskQueue->val;

    task_queue_t *toDelete = taskQueue;
    taskQueue = taskQueue->next;

    free(toDelete);

    taskCount--;

    return task;
}

void executeTask(task_t task){
    if (mode == ENCRYPTION){
        encrypt(task.data, key);
    } else if (mode == DECRYPTION){
        decrypt(task.data, key);
    }
    printf("%s", task.data);
    fflush(stdin);
}

void* startThread(void *args){
    for (EVER){
        task_t task;

        pthread_mutex_lock(&mutexQueue);

        while (taskCount == 0){
            pthread_cond_wait(&condQueue, &mutexQueue);
        }

        task = dequeueTask();
        if (task.empty){
            return NULL;
        }

        printf("Task count %i. Executing task %s.\n", taskCount, task.data);

        pthread_mutex_unlock(&mutexQueue);

        executeTask(task);
    }
}

int main(int argc, char *argv[]){

    if (argc != 3){
	    perror("usage: \"key [-e/-d]\"\n");
	    return 1;
	}
    if (!strcmp(argv[2], "-e")){
        mode = ENCRYPTION;
    } else if (!strcmp(argv[2], "-d")){
        mode = DECRYPTION;
    } else {
        perror("usage: \"key [-e/-d]\"\n");
	    return 1;
    }

    key = atoi(argv[1]);

    char c;
    int counter = 0;
    char data[TASK_SIZE];
    task_t task;
    task.empty = 0;

    while ((c = getchar()) != EOF){
	    data[counter] = c;
	    counter++;
	    if (counter == TASK_SIZE){
            strcpy(task.data, data);
            task.data[TASK_SIZE] = '\0';
            enqueueTask(task);
		    counter = 0;
	    }
	}
	if (counter > 0){
		char lastData[counter];
		lastData[0] = '\0';
		strncat(lastData, data, counter);
        strcpy(task.data, lastData);
        enqueueTask(task);
	}

    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);

    int i;
    for (i = 0; i < THREAD_NUM; i++){
        if (pthread_create(&th[i], NULL, &startThread, NULL) != 0){
            perror("Failed to create the thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++){
        if (pthread_join(th[i], NULL) != 0){
            perror("Failed to join the thread");
        }
    }

    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);

    return 0;
}