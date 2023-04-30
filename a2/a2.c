#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "a2_helper.h"

pthread_mutex_t lock;
pthread_cond_t cond;
pthread_barrier_t barrier; 
int count=0;

void *thread_six(void* arg){
    int thread_num=*(int*)arg;
    info(BEGIN,6,thread_num);

    if(thread_num==2){
        pthread_mutex_lock(&lock);
        while(count<4){
            pthread_cond_wait(&cond,&lock);
        }
        pthread_mutex_unlock(&lock);
        // pthread_barrier_wait(&barrier);
    }
    info(END, 6, thread_num);
    pthread_mutex_lock(&lock);
    count++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* thread_func(void* arg) {
    int thread_num = *(int*)arg;
    info(BEGIN, 6, thread_num);

    if (thread_num == 2) {
        // wait for thread T6.5 to start
        pthread_mutex_lock(&lock);
        while (count < 4) {
            pthread_cond_wait(&cond, &lock);
        }
        pthread_mutex_unlock(&lock);
    }

    // simulate some work
    // int i;
    // for (i = 0; i < 100000000; i++) {}

    // wait at the barrier before terminating
    pthread_barrier_wait(&barrier);

    info(END, 6, thread_num);

    // signal that this thread has ended
    pthread_mutex_lock(&lock);
    count++;

        // if all threads have ended, signal the main thread
        pthread_cond_signal(&cond);
    
    pthread_mutex_unlock(&lock);

    return NULL;
}



int main(){
    init();

    pid_t pid2,pid3,pid4,pid5,pid6,pid7;
    info(BEGIN, 1, 0);
    
    pid2=fork();

    if(pid2==0){
        info(BEGIN,2,0);
        info(END,2,0);
        return 0;
    }
    pid3=fork();
    if(pid3==0){
        info(BEGIN,3,0);
        pid4=fork();
        if(pid4==0){
            info(BEGIN,4,0);

            pid5=fork();

            if(pid5==0){
                info(BEGIN,5,0);
                pid7=fork();
                if(pid7==0){
                    info(BEGIN,7,0);
                    info(END,7,0);
                    return 0;
                }
                waitpid(pid7,NULL,0);
                info(END,5,0);
                return 0;
            }

            waitpid(pid5,NULL,0);
            info(END,4,0);
            return 0;
            
        }
        waitpid(pid4,NULL,0);
        info(END,3,0);
        return 0;
    }
    pid6=fork();
    if(pid6==0){
        info(BEGIN,6,0);
         int i, thread_nums[5] = { 1, 2, 3, 4,5};
        pthread_t threads[5];
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&cond, NULL);
       pthread_barrier_init(&barrier, NULL, 5);

        for (i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_six, &thread_nums[i]);
         }
        for (i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
        }
        pthread_barrier_destroy(&barrier);
         pthread_cond_destroy(&cond);
         pthread_mutex_destroy(&lock);
        info(END,6,0);
        return 0;
    }
    waitpid(pid2,NULL,0);
    waitpid(pid3,NULL,0);
    waitpid(pid6,NULL,0);
    info(END, 1, 0);
    return 0;
}
