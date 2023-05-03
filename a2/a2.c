#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include "a2_helper.h"
#define MAX_RUNNING 4
pthread_mutex_t lock;
pthread_cond_t cond;
int th_running;
int count=0;
sem_t sem;
                 // count of threads currently running
int num_ended = 0;
void *thread_six(void* arg){
    int thread_num=*(int*)arg;
    info(BEGIN,6,thread_num);
    
    if(thread_num==2){
        
        info(BEGIN,6,5);
        info(END,6,5);
       
    }
    info(END, 6, thread_num);
    
    return NULL;
}

void *thread_seven(void* arg){
    int thread_num=*(int*)arg;
    info(BEGIN,7,thread_num);
     
    

    if(thread_num==11){
       
        info(BEGIN,7,12);
        info(BEGIN,7,13);
        info(BEGIN,7,14);
        info(END,7,11);
        info(END,7,12);
        info(END,7,13);
        info(END,7,14);
        
        
    }
    
    else
    {   
        info(END, 7, thread_num);
    }

   
    return NULL;
}
void* thread_func(void* arg) {
    int thread_num = *(int*)arg;
    info(BEGIN, 7, thread_num);

    sem_wait(&sem);           // wait for semaphore to be available

    // simulate some work
    int i;
    for (i = 0; i < 100000000; i++) {}

    pthread_mutex_lock(&lock);
    count++;                  // increment count of running threads
    pthread_mutex_unlock(&lock);

    // check if Thread T7.11 can end
    if (thread_num == 11) {
        pthread_mutex_lock(&lock);
        while (count != MAX_RUNNING) {
            pthread_cond_wait(&cond, &lock);
        }
        pthread_mutex_unlock(&lock);
    }

    // simulate some more work
    

    pthread_mutex_lock(&lock);
    count--;                  // decrement count of running threads
    num_ended++;              // increment count of ended threads

    // signal T7.11 if there are 4 threads running
    if (thread_num != 11 && count == MAX_RUNNING) {
        pthread_cond_signal(&cond);
    }

    pthread_mutex_unlock(&lock);

    sem_post(&sem);           // release semaphore

    info(END, 7, thread_num);
    return NULL;
}

void *thread_three(void* arg){

    int thread_num=*(int*)arg;
    info(BEGIN,3,thread_num);
   
    info(END, 3, thread_num);
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

         int i, thread_nums[5] = { 1, 2, 3, 4, 5};
        pthread_t threads[5];
       
       sem_init(&sem, 0, 4); 

        for (i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_three, &thread_nums[i]);
         }
        for (i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
        }
       sem_destroy(&sem);

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
                        int a=i+1;
                        //printf("%d\n",a);
                        thread_nums[i]=i+1;
                        if(a==11){
                            i+=3;
                        }
                    }
                    pthread_mutex_init(&lock, NULL);
                    pthread_cond_init(&cond, NULL);
                    sem_init(&sem, 0, 3);
                    for(int i=0; i<47; i++){
                        int a=i+1;

                        pthread_create(&threads[i],NULL,thread_seven,&thread_nums[i]);
                        if(a==11){
                            i+=3;
                        }
                    }
                    for(int i=0; i<47; i++){
                        int a=i+1;
                        pthread_join(threads[i], NULL);
                        if(a==11){
                            i+=3;
                        }
                    }
                    sem_destroy(&sem);
                    pthread_cond_destroy(&cond);
                    pthread_mutex_destroy(&lock);
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
         int i, thread_nums[5] = { 1, 2, 3, 4};
        pthread_t threads[5];
       
       

        for (i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_six, &thread_nums[i]);
         }
        for (i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
        }
       
       
        info(END,6,0);
        return 0;
    }
    waitpid(pid2,NULL,0);
    waitpid(pid3,NULL,0);
    waitpid(pid6,NULL,0);
    info(END, 1, 0);
    return 0;
}
