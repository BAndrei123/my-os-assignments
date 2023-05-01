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
        
        info(BEGIN,6,5);
        info(END,6,5);
       
    }
    info(END, 6, thread_num);
    return NULL;
}

void *thread_seven(void* arg){
    int thread_num=*(int*)arg;
    info(BEGIN,7,thread_num);
   
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
                        thread_nums[i]=i+1;
                    }
                    for(int i=0; i<47; i++){
                        pthread_create(&threads[i],NULL,thread_seven,&thread_nums[i]);
                    }
                    for(int i=0; i<47; i++){
                        pthread_join(threads[i], NULL);
                    }

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
