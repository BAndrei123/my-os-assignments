#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "a2_helper.h"

#define MAX_RUNNING 4
pthread_mutex_t lock;

int th_running;
int count=0;
sem_t sem;
int ok=0;
sem_t sem_max_threads;
sem_t sem_t711;
sem_t *t_3_1;
sem_t *t_3_4;

pthread_mutex_t mutex;
pthread_mutex_t mutex2;
pthread_cond_t cond;

int num_running_threads = 0;
                 // count of threads currently running
int num_ended = 0;
void *thread_six(void* arg){
    int thread_num=*(int*)arg;
     if(thread_num==1){
        sem_wait(t_3_1);
    }
    info(BEGIN,6,thread_num);
   
    pthread_mutex_lock(&lock);
    if(thread_num==2){
        info(BEGIN,6,5);
        info(END,6,5);
    }
     pthread_mutex_unlock(&lock);
    info(END, 6, thread_num);
    pthread_mutex_lock(&mutex2);
    if(thread_num==1){
        sem_post(t_3_4);
    }
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

void *thread_seven(void *arg) {
    int thread_num = *(int*)arg;
    info(BEGIN, 7, thread_num);
    info(END, 7, thread_num);
    return NULL;
}

void *thread_three(void* arg){
    
    int thread_num = *(int*)arg;
    if(thread_num==4){
        sem_wait(t_3_4);
    }
    info(BEGIN, 3, thread_num);

    info(END, 3, thread_num);
    if(thread_num==1){
        sem_post(t_3_1);
    }
    
    return NULL;
}



    int main(){
        init();

        t_3_1 = sem_open("/proc3", O_CREAT, 0600, 0);
        t_3_4 = sem_open("/proc4", O_CREAT, 0600, 0); 
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

            int i, thread_nums[5] = { 1, 2, 3 ,4, 5};
            pthread_t threads[5];
            // t_3_1 = sem_open("/proc3", O_CREAT, 0600, 0);
            // t_3_4 = sem_open("/proc4", O_CREAT, 0600, 0);   
            
            for (i = 0; i < 5; i++) {
            pthread_create(&threads[i], NULL, thread_three, &thread_nums[i]);
            }
            for (i = 0; i < 5; i++) {
            pthread_join(threads[i], NULL);
            }
        
        
            pid4=fork();
            if(pid4==0){
                info(BEGIN,4,0);

                pid5=fork();

                if(pid5==0){
                    info(BEGIN,5,0);
                    pid7=fork();
                    if(pid7==0){

                        info(BEGIN,7,0);

                        pthread_t threads[47];
                        int thread_nums[47];
                        for(int i=0; i<47; i++){
                            
                            //printf("%d\n",a);
                            thread_nums[i]=i+1;
                            
                        }
                        sem_init(&sem_max_threads, 0, 4);
                        sem_init(&sem_t711, 0, 1);
                        pthread_mutex_init(&mutex, NULL);
                        pthread_cond_init(&cond, NULL);

                        for(int i=0; i<47; i++){
                        // int a=i+1;

                            pthread_create(&threads[i],NULL,thread_seven,&thread_nums[i]);
                            // if(a==11){
                            //     i+=3;
                            // }
                        }
                        for(int i=0; i<47; i++){
                        // int a=i+1;
                            pthread_join(threads[i], NULL);
                            // if(a==11){
                            //     i+=3;
                            // }
                        }
                        sem_destroy(&sem);
                        sem_destroy(&sem_t711);
                        pthread_cond_destroy(&cond);
                        pthread_mutex_destroy(&mutex);
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
            sem_close(t_3_1);
            sem_close(t_3_4);
            info(END,3,0);
            // sem_close(t_3_1);
            //  sem_close(t_3_4);
            return 0;
        }
        pid6=fork();
        if(pid6==0){
            info(BEGIN,6,0);
            int i, thread_nums[5] = { 1, 2, 3, 4};
            pthread_t threads[5];
        
        // t_3_4 = sem_open("/proc3", O_CREAT, 0600, 0);
            // t_3_1 = sem_open("/proc3", O_CREAT, 0600, 1);
            // t_3_4 = sem_open("/proc4", O_CREAT, 0600, 1); 
            for (i = 0; i < 4; i++) {
            pthread_create(&threads[i], NULL, thread_six, &thread_nums[i]);
            }
            for (i = 0; i < 4; i++) {
            pthread_join(threads[i], NULL);
            }
            
            sem_close(t_3_1);
             sem_close(t_3_4);
            info(END,6,0);
            return 0;
        }
        waitpid(pid2,NULL,0);
        waitpid(pid3,NULL,0);
        waitpid(pid6,NULL,0);
        sem_close(t_3_1);
        sem_close(t_3_4);
        info(END, 1, 0);
        return 0;
    }
