
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
int main() {
    const char* response_pipe = "RESP_PIPE_45704";
    const char* request_pipe = "REQ_PIPE_45704";

    
    if (mkfifo(response_pipe, 0666) == -1) {
        perror("ERROR\ncannot create the response pipe | cannot open the request pipe");
        exit(1);
    }

    
    int request_fd = open(request_pipe, O_RDONLY);
    if (request_fd == -1) {
        perror("ERROR\ncannot create the response pipe | cannot open the request pipe");
        unlink(response_pipe);  
        exit(1);
    }

    
    int response_fd = open(response_pipe, O_WRONLY);
    if (response_fd == -1) {
        perror("ERROR\ncannot create the response pipe | cannot open the request pipe");
        unlink(response_pipe);  
        close(request_fd);      
        exit(1);
    }

    const char* request_message = "BEGIN"; // acolesa - fara acel 0 pus de tine - el exista oricum

    // acolesa - ai omis sa trimiti LUNGIMEA sirului 
    char len = strlen(request_message);
    write(response_fd, &len, 1);
    
    if (write(response_fd, request_message, strlen(request_message)) == -1) { 
        perror("ERROR\ncannot create the response pipe | cannot open the request pipe");
        unlink(response_pipe); 
        close(request_fd);      
        close(response_fd);     
        exit(1);
    }

    char request_buffer[100];
    
    unsigned int number=0;
    
    
    
    
    read(request_fd,&number,1);
    read(request_fd,request_buffer,number);

    

    if(strcmp(request_buffer,"PING")==0){
        char* ping="PING";
        char ping_len=strlen(ping);
        char* pong="PONG";
        char pong_len=strlen(pong);
        unsigned int number2=45704;

        write(response_fd,&ping_len,1);
        write(response_fd,ping,strlen(ping));
        write(response_fd,&pong_len,1);
        write(response_fd,pong,strlen(pong));
        write(response_fd,&number2,sizeof(unsigned int));
    }
    int shm_fd;
    char* shared_char=NULL;
    if(strcmp(request_buffer,"CREATE_SHM")==0){
        unsigned int size=0;
        read(request_fd,&size,1);
        
        char* create="CREATE_SHM";
        char* succes="SUCCESS";
        char* error="ERROR";
        char len_create=strlen(create);
        char len_succes=strlen(succes);
        char len_error=strlen(error);

        shm_fd=shm_open("/fLm9wo",O_CREAT | O_RDWR,0644);
        if(shm_fd < 0){
            write(response_fd,&len_create,1);
            write(response_fd,create,strlen(create));
            write(response_fd,&len_error,1);
            write(response_fd,error,strlen(error));
        }
        ftruncate(shm_fd,size);
        
        shared_char=(char*) mmap(0,size,PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if(shared_char==MAP_FAILED){
            write(response_fd,&len_create,1);
            write(response_fd,create,strlen(create));
            write(response_fd,&len_error,1);
            write(response_fd,error,strlen(error));
        }
        
        write(response_fd,&len_create,1);
        write(response_fd,create,strlen(create));
        write(response_fd,&len_succes,1);
        write(response_fd,succes,strlen(succes));
    }
    if(strcmp(request_buffer,"WRITE_TO_SHM")==0){
        unsigned int offset=0;
        unsigned int value=0;
        read(request_fd,&offset,1);
        read(request_fd,&value,1);

        char* write_shm="WRITE_TO_SHM";
        char* succes="SUCCESS";
        char* error="ERROR";
        char len_create=strlen(write_shm);
        char len_succes=strlen(succes);
        char len_error=strlen(error);

        if(offset<0 || offset>2148240){
            write(response_fd,&len_create,1);
            write(response_fd,write_shm,strlen(write_shm));
            write(response_fd,&len_error,1);
            write(response_fd,error,strlen(error));
        }
        

        unsigned int size;
        size=lseek(shm_fd,0,SEEK_END);
        lseek(shm_fd,0,SEEK_SET);
        if(offset+sizeof(value)>size){
            write(response_fd,&len_create,1);
            write(response_fd,write_shm,strlen(write_shm));
            write(response_fd,&len_error,1);
            write(response_fd,error,strlen(error));
        }
        shared_char[offset]=size;

        write(response_fd,&len_create,1);
        write(response_fd,write_shm,strlen(write_shm));
        write(response_fd,&len_succes,1);
        write(response_fd,succes,strlen(succes));

    }
    // if(strcmp(request_buffer,"MAP_FILE")==0){
    //     unsigned size=0;
    //     read(request_fd,&size,1);

    //     char file[size];
    //     read(request_fd,file,size);

    //     char* map_file="MAP_FILE";
    //     char* succes="SUCCESS";
    //     char* error="ERROR";
    //     char len_create=strlen(map_file);
    //     char len_succes=strlen(succes);
    //     char len_error=strlen(error);

    //     int fd=open(file,O_RDONLY);
    //     if(fd==-1){
    //         write(response_fd,&len_create,1);
    //         write(response_fd,map_file,strlen(map_file));
    //         write(response_fd,&len_error,1);
    //         write(response_fd,error,strlen(error));
    //     }
    //     size=lseek(fd,0,SEEK_END);
    //     lseek(fd,0,SEEK_SET);
    //     char* data=NULL;
    //     data = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd,0);
    //     if(data==MAP_FAILED){
    //         write(response_fd,&len_create,1);
    //         write(response_fd,map_file,strlen(map_file));
    //         write(response_fd,&len_error,1);
    //         write(response_fd,error,strlen(error));
    //         close(fd);
    //     }
    //     write(response_fd,&len_create,1);
    //     write(response_fd,map_file,strlen(map_file));
    //     write(response_fd,&len_succes,1);
    //     write(response_fd,succes,strlen(succes));
    // }


    printf("SUCCESS");

    close(request_fd);    
    close(response_fd);   
    unlink(response_pipe);  

    return 0;
}
