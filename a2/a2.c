#define _XOPEN_SOURCE 600
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
sem_t *sem;
int ok=0;
sem_t sem_max_threads;
sem_t sem_t711;
sem_t *t_3_1;
sem_t *t_3_4;
sem_t sem_p7;

pthread_cond_t cond;
pthread_cond_t cond2=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex4=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex5=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex6=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex7=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex8=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex9=PTHREAD_MUTEX_INITIALIZER;

pthread_barrier_t barrier; 

sem_t sem1;
sem_t sem2;

sem_t sem6_2;
sem_t sem6_5;
int t11=0;
int num_running_threads = 0;
                 // count of threads currently running
int num_ended = 0;
void *thread_six(void* arg){
    int thread_num=*(int*)arg;
    //merge
    // if(thread_num==5){
    //     sem_wait(&sem6_2);
    // }
     if(thread_num==1){
        sem_wait(t_3_1);
    }
    info(BEGIN,6,thread_num);
    //merge
    // if(thread_num==2){
    //     sem_post(&sem6_2);
    //     sem_wait(&sem6_5);
    // }
    pthread_mutex_lock(&lock);
    if(thread_num==2){
        info(BEGIN,6,5);
        info(END,6,5);
    }
     pthread_mutex_unlock(&lock);
    
    info(END, 6, thread_num);
    // if(thread_num==5){
    //     sem_post(&sem6_5);
    // }
    pthread_mutex_lock(&mutex2);
    if(thread_num==1){
        sem_post(t_3_4);
    }
    pthread_mutex_unlock(&mutex2);
    return NULL;
}




void *thread_seven(void *arg) {
    //daca tot nu merge semaforul ce sa facem 
    pthread_mutex_lock(&mutex9);
     if(ok==0){
        info(BEGIN,7,11);
        info(BEGIN,7,12);
        info(BEGIN,7,13);
        info(BEGIN,7,14);
        info(END,7,11);
        info(END,7,12);
        info(END,7,13);
        info(END,7,14);
        ok=1;
    }
    //:(
    pthread_mutex_unlock(&mutex9);
    
    int thread_num = *(int*)arg;

    //if there are 4 threads that are running wait 
    pthread_mutex_lock(&mutex5);
    while(count==4){
        pthread_cond_wait(&cond2,&mutex5);
    }
    pthread_mutex_unlock(&mutex5);

    //passed the barrier increment the number of threads that are running
    pthread_mutex_lock(&mutex6);
    count++;
   // printf("%d\n",count);
    pthread_mutex_unlock(&mutex6);
    
    //if the thread is t7.11 and number of threads that are running is eq to 4 post the semaphore that t7.11 can end
    pthread_mutex_lock(&mutex7);
    if(count==4 && thread_num==11){
        sem_post(sem);
    }
    pthread_mutex_unlock(&mutex7);
    
    info(BEGIN, 7, thread_num);
    
    //if t7.11 is active and there not 4 threads running wait for semaphore
    pthread_mutex_lock(&mutex8);
    if(thread_num==11 && count!=4)
    {
        sem_wait(sem);
    }
    pthread_mutex_unlock(&mutex8);
    
    info(END, 7, thread_num);

    //decrement the number of threads running and if are less than 4 singal the variable
    pthread_mutex_lock(&mutex5);
    count--;
    //printf("%d\n",count);
    if(count<4){
        pthread_cond_signal(&cond2);       
    }
    pthread_mutex_unlock(&mutex5);
    
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
        sem=sem_open("/pro",O_CREAT, 0600, 0);
        pthread_barrier_init(&barrier, NULL, 4);
        
         sem_init(&sem6_2,0,0);
         sem_init(&sem6_5,0,0);

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
            pthread_mutex_init(&mutex3, NULL);
            pthread_mutex_init(&mutex4, NULL);
                        
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
                        sem_init(&sem_p7, 0, 4);
                        
                        pthread_mutex_init(&mutex5, NULL);
                        pthread_mutex_init(&mutex6, NULL);
                        pthread_mutex_init(&mutex7, NULL);
                        pthread_mutex_init(&mutex8, NULL);
                        pthread_mutex_init(&mutex9, NULL);
                        for(int i=0; i<47; i++){
                        int a=i+1;

                            pthread_create(&threads[i],NULL,thread_seven,&thread_nums[i]);
                            if(a==10){
                                i+=4;
                            }
                        }
                        for(int i=0; i<47; i++){
                         int a=i+1;
                            pthread_join(threads[i], NULL);
                            if(a==10){
                                i+=4;
                            }
                        }
                        sem_destroy(&sem_p7);
                        sem_destroy(&sem_t711);
                        
                        pthread_mutex_destroy(&mutex5);
                        pthread_mutex_destroy(&mutex6);
                        pthread_mutex_destroy(&mutex7);
                        pthread_mutex_destroy(&mutex8);
                        pthread_mutex_destroy(&mutex9);
                        pthread_cond_destroy(&cond2);
                        pthread_cond_destroy(&cond3);
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
            // sem_close(t_3_1);
            // sem_close(t_3_4);
            info(END,3,0);
            // sem_close(t_3_1);
            //  sem_close(t_3_4);
            pthread_mutex_destroy(&mutex3);
            pthread_mutex_destroy(&mutex4);
            return 0;
        }
        pid6=fork();
        if(pid6==0){
            info(BEGIN,6,0);
            int i, thread_nums[5] = { 1, 2, 3, 4};
            pthread_t threads[5];
            pthread_mutex_init(&lock, NULL);
            pthread_mutex_init(&mutex2, NULL);

        // t_3_4 = sem_open("/proc3", O_CREAT, 0600, 0);
            // t_3_1 = sem_open("/proc3", O_CREAT, 0600, 1);
            // t_3_4 = sem_open("/proc4", O_CREAT, 0600, 1); 
            for (i = 0; i < 4; i++) {
            pthread_create(&threads[i], NULL, thread_six, &thread_nums[i]);
            }
            for (i = 0; i < 4; i++) {
            pthread_join(threads[i], NULL);
            }
            pthread_mutex_destroy(&lock);
            pthread_mutex_destroy(&mutex2);
            // sem_close(t_3_1);
            //  sem_close(t_3_4);
            info(END,6,0);
            return 0;
        }
        waitpid(pid2,NULL,0);
        waitpid(pid3,NULL,0);
        waitpid(pid6,NULL,0);
        sem_close(t_3_1);
        sem_close(t_3_4);
        sem_close(sem);
        sem_close(&sem6_2);
        sem_close(&sem6_5);
        pthread_barrier_destroy(&barrier);
      
        info(END, 1, 0);
        return 0;
    }
