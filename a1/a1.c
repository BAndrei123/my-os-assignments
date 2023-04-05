#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>


int endsWith(char *name, char *end) {
    int name_len = strlen(name);
    int end_len = strlen(end);

    if (name_len >= end_len && strcmp(name + name_len - end_len, end) == 0) {
        return 1;
    } else {
        return 0;
    }
}
void list_non_recursive(char* dir_name, int haspermwrite, int find_name, char* name_ends_with){
    struct stat sb;
    struct stat sb2;
    struct dirent* d;
    char* path=(char*)malloc(1024*sizeof(char));
    
        if(stat(dir_name,&sb) || !S_ISDIR(sb.st_mode)){
            printf("ERROR\ninvalid directory path");
             exit(EXIT_FAILURE);
            }

    DIR* dir=opendir(dir_name);
    printf("SUCCESS\n");
    while((d=readdir(dir))!=NULL){
         if(strcmp(d->d_name,".")==0 || strcmp(d->d_name,"..")==0)
        continue;
        snprintf(path,1024,"%s/%s",dir_name,d->d_name);
        
        if(haspermwrite == 1 ){
            if(lstat(path,&sb2) == 0){
                if((sb2.st_mode & S_IWUSR))
            {
                printf("%s\n", path);
            }
            }
            
        }
        if(find_name == 1){
            if(endsWith(d->d_name,name_ends_with)==1){
                printf("%s\n", path);
            }
        }
        if(haspermwrite == 0 && find_name==0){
            printf("%s\n", path);
        
    }
    }
    
    //printf("SUCCESS");

    free(path);
    closedir(dir);

}
void list_recursive(char* dir_name,int haspermwrite, int find_name, char* name_ends_with){

    struct stat sb;
    struct stat sb2;
    struct dirent* d;
    
    char* path=(char*)malloc(1024*sizeof(char));
    
        if(stat(dir_name,&sb) || !S_ISDIR(sb.st_mode)){
            printf("ERROR\ninvalid directory path");
             exit(EXIT_FAILURE);
            }

    DIR* dir=opendir(dir_name);
    
    while((d=readdir(dir))!=NULL){
         if(strcmp(d->d_name,".")==0 || strcmp(d->d_name,"..")==0)
        continue;
        snprintf(path,1024,"%s/%s",dir_name,d->d_name);
        
        if(haspermwrite == 1 ){
            if(lstat(path,&sb2) == 0){
                if((sb2.st_mode & S_IWUSR))
            {
                printf("%s\n", path);
            }
            }
            
        }
        if(find_name == 1){
            if(endsWith(d->d_name,name_ends_with)==1){
                printf("%s\n", path);
            }
        }
        if(haspermwrite == 0 && find_name==0){
            printf("%s\n", path);
        }
        if(lstat(path,&sb2)==0){
            if(S_ISDIR(sb2.st_mode)){
                list_recursive(path,haspermwrite,find_name,name_ends_with);
            }
        }
    
    }
    
    //printf("SUCCESS");

    free(path);
    closedir(dir);
}



void parse(char* path){
    char* magic=(char*)malloc(2*sizeof(char));
    char* nr_of_sections=(char*)malloc(sizeof(char));
    unsigned char* version=(unsigned char*)malloc(4*sizeof(unsigned char));
    char* header_size=(char*)malloc(2*sizeof(char));
    int fd = open(path, O_RDONLY);

    if(fd==-1){
        printf("Failed to open file");
        free(magic);
        free(version);
        free(header_size);
        free(nr_of_sections);
        return;
    }
    lseek(fd, 0, SEEK_SET);
    read(fd,magic,2);

    if(strcmp(magic,"y5")!=0){
        printf("ERROR\nwrong magic");
        free(magic);
        free(version);
        free(header_size);
        free(nr_of_sections);
        return;
    }

    
    read(fd,header_size,2);
    //lseek(fd,4,SEEK_SET);
    read(fd,version,4);
    unsigned int value_version=version[0] | version[1] << 8 | version[2] << 16 | version[3] << 24;

    if(!(value_version>=107 && value_version <=221)){
        printf("ERROR\nwrong version");
        free(magic);
        free(version);
        free(header_size);
        free(nr_of_sections);
        return;
    }

    //lseek(fd,8,SEEK_SET);
    read(fd,nr_of_sections,1);
    unsigned int value=nr_of_sections[0] | nr_of_sections[1] << 8 | nr_of_sections[2] << 16 | nr_of_sections[3] << 24;
    if(!(value>=8 && value <=14)){
        printf("ERROR\nwrong sect_nr");
        free(magic);
        free(version);
        free(header_size);
        free(nr_of_sections);
        return;
    }
    int count=0;
    
    while(count!=value){
        lseek(fd,6,SEEK_CUR);

        unsigned char* type=(unsigned char*)malloc(4*sizeof(unsigned char));

        read(fd,type,4);

        unsigned int value_type=type[0] | type[1] << 8 | type[2] << 16 | type[3] << 24;
        

        if(value_type!=89 && value_type!=86 && value_type!=18 && value_type!=71 && value_type!=46){
            printf("ERROR\nwrong sect_types");
            free(magic);
            free(version);
            free(header_size);
            free(nr_of_sections);
            free(type);
            return;
        }
        lseek(fd,8,SEEK_CUR);
        free(type);
        count++;
    }
    lseek(fd,0,SEEK_SET);
    lseek(fd,9,SEEK_CUR);

    printf("SUCCESS\n");
    printf("version=%d\n",value_version);    //printf("nr_sections=%d\n",value_sections);
    printf("nr_sections=%d\n",value);
    
    count=0;
    while(count!=value){
        unsigned char* name=(unsigned char*)malloc(6*sizeof(char));
        unsigned char* s_type=(unsigned char*)malloc(4*sizeof(char));
        unsigned char* s_size=(unsigned char*)malloc(4*sizeof(char));
        printf("section%d: ", count+1);
        read(fd,name,6);
        printf("%s ", name);

        read(fd,s_type,4);
        unsigned int value_type=s_type[0] | s_type[1] << 8 | s_type[2] << 16 | s_type[3] << 24;
        printf("%d ", value_type);

        lseek(fd,4,SEEK_CUR);

        read(fd,s_size,4);
        unsigned int value_size=s_size[0] | s_size[1] << 8 | s_size[2] << 16 | s_size[3] << 24;
        printf("%d \n",value_size);
        
        free(name);
        free(s_type);
        
        free(s_size);

        count++;
    }
    
    free(magic);
    free(version);
    free(header_size);
    free(nr_of_sections);

    close(fd);

}

unsigned int  parse2(char* path, int sect_nr, int line_nr){
    char* magic=(char*)malloc(2*sizeof(char));
    char* nr_of_sections=(char*)malloc(sizeof(char));
    unsigned char* version=(unsigned char*)malloc(4*sizeof(unsigned char));
    char* header_size=(char*)malloc(2*sizeof(char));
    int fd = open(path, O_RDONLY);
    
    

    if(fd==-1){
        free(magic);
        free(version);
        free(header_size);
        free(nr_of_sections);
        return 1;
    }
    lseek(fd, 0, SEEK_SET);
    read(fd,magic,2);

    if(strcmp(magic,"y5")!=0){
        printf("ERROR\ninvalid file");
        free(magic);
        free(version);
        free(header_size);
        free(nr_of_sections);
        return 1;
    }

    
    read(fd,header_size,2);
    
    read(fd,version,4);
    unsigned int value_version=version[0] | version[1] << 8 | version[2] << 16 | version[3] << 24;

    if(!(value_version>=107 && value_version <=221)){
        printf("ERROR\ninvalid file");
        free(magic);
        free(version);
        free(header_size);
        free(nr_of_sections);
        return 1;
    }

    //lseek(fd,8,SEEK_SET);
    read(fd,nr_of_sections,1);
    unsigned int value=nr_of_sections[0] | nr_of_sections[1] << 8 | nr_of_sections[2] << 16 | nr_of_sections[3] << 24;
    if(!(value>=8 && value <=14)){
        printf("ERROR\ninvalid file");
        free(magic);
        free(version);
        free(header_size);
        free(nr_of_sections);
        return 1;
    }
    int count=0;

    if(sect_nr > value){
        printf("ERROR\nsection");
        return 1;
    }

    while(count!=value){
        lseek(fd,6,SEEK_CUR);

        unsigned char* type=(unsigned char*)malloc(4*sizeof(unsigned char));

        read(fd,type,4);

        unsigned int value_type=type[0] | type[1] << 8 | type[2] << 16 | type[3] << 24;
        

        if(value_type!=89 && value_type!=86 && value_type!=18 && value_type!=71 && value_type!=46){
            
            free(magic);
            free(version);
            free(header_size);
            free(nr_of_sections);
            free(type);
            return 1;
        }
        
            lseek(fd,4,SEEK_CUR);
        
        unsigned char* size=(unsigned char*)malloc(4*sizeof(unsigned char));
        read(fd,size,4);
        unsigned int value_size=size[0] | size[1] << 8 | size[2] << 16 | size[3] << 24;

        if(count+1 == sect_nr && line_nr > value_size ){
            printf("%d\n",value_size);
            printf("ERROR\nline");
            return 1;

        }
        free(size);
        free(type);
        count++;
    }

    
    free(magic);
    free(version);
    free(header_size);
    free(nr_of_sections);
    close(fd);
    lseek(fd,0,SEEK_SET);
    return value;
}

void extract(char* path, int sect_nr, int line_nr){
    int fd=open(path, O_RDONLY);

    unsigned nr_of_sections=parse2(path,sect_nr,line_nr);
    if(nr_of_sections==1)
    {
        return;
    }
    printf("SUCCESS\n");
    lseek(fd,0,SEEK_SET);
    lseek(fd,9,SEEK_CUR);
    unsigned int value_offset;
    unsigned int value_size;
    int count=0;
    while(count!= nr_of_sections){

    if(count+1==sect_nr)
    {
    lseek(fd,10,SEEK_CUR);
    unsigned char* offset=(unsigned char*)malloc(4*sizeof(unsigned char));
    read(fd,offset,4);
    value_offset=offset[0] | offset[1] << 8 | offset[2] << 16 | offset[3] << 24;
    free(offset);
    
    unsigned char* size=(unsigned char*)malloc(4*sizeof(unsigned char));
    read(fd,size,4);
    value_size=size[0] | size[1] << 8 | size[2] << 16 | size[3] << 24;
    free(size);
    break;
    }

        lseek(fd,18,SEEK_CUR);
    
    count++;
    }
    
    lseek(fd,0,SEEK_SET);
    lseek(fd,value_offset,SEEK_CUR);
    
    
    int current_line=1;
     char* buffer=(char*)malloc(value_size+1 *sizeof(char));
    read(fd,buffer,value_size);
     char* token;
    token = strtok(buffer,"\n");
    while(token != NULL){
        if(current_line == line_nr){
            printf("%s\n",token);
            break;
        }
        current_line++;
        token = strtok(NULL, "\n");
    }
    
    close(fd);
    free(buffer);
}

int main(int argc, char **argv){
    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("45704\n");
        }
        if(strcmp(argv[1], "list") == 0){
                if(argc==5){
                    if((strcmp(argv[2],"has_perm_write" )==0 && strcmp(argv[3],"recursive" )==0) || (strcmp(argv[3],"has_perm_write" )==0 && strcmp(argv[2],"recursive" )==0)){
                        char* path=strchr(argv[4],'=');
                        memmove(path, path + 1, strlen(path));
                        printf("SUCCESS\n");
                        list_recursive(path,1,0, "0");
                    }
                    else{
                        if(strcmp(argv[2],"recursive")==0){
                            char* name=strchr(argv[3],'=');
                            memmove(name,name+1, strlen(name));
                            char* path=strchr(argv[4],'=');
                            memmove(path, path + 1, strlen(path));
                            printf("SUCCESS\n");
                            list_recursive(path,0,1,name);
                        }
                        if(strcmp(argv[3],"recursive")==0){
                            char* name=strchr(argv[2],'=');
                            memmove(name,name+1, strlen(name));
                            char* path=strchr(argv[4],'=');
                            memmove(path, path + 1, strlen(path));
                            printf("SUCCESS\n");
                            list_recursive(path,0,1,name);
                        }
                    }
                }
            
                if(argc==4){
                   if(strcmp(argv[2],"has_perm_write")==0)
                   { 
                     char* path=strchr(argv[3],'=');
                    memmove(path, path + 1, strlen(path));
                    list_non_recursive(path,1,0, "0");
                    }
                    else if(strcmp(argv[2],"has_perm_write" )!=0 && strcmp(argv[2],"recursive" )!=0){
                        char* name=strchr(argv[2],'=');
                        memmove(name,name+1, strlen(name));
                        char* path=strchr(argv[3],'=');
                        memmove(path, path + 1, strlen(path));
                        list_non_recursive(path,0,1,name);
                    }
                    else if(strcmp(argv[2],"has_perm_write" )!=0 && strcmp(argv[2],"recursive" )==0){
                        char* path=strchr(argv[3],'=');
                        memmove(path, path + 1, strlen(path));
                        printf("SUCCESS\n");
                        list_recursive(path,0,0, "0");
                    }
                }
                if(argc==3){
                    
                    char* path=strchr(argv[2],'=');
                   memmove(path, path + 1, strlen(path));
                   list_non_recursive(path,0,0, "0");
                }

        }
        if(strcmp(argv[1], "parse") == 0){
            char* path=strchr(argv[2],'=');
            memmove(path, path + 1, strlen(path));
            parse(path);
        }

        if(strcmp(argv[1],"extract") ==0){
            char* path=strchr(argv[2],'=');
            memmove(path, path + 1, strlen(path));
            
            char* section=strchr(argv[3],'=');
            memmove(section, section + 1, strlen(section));
            int sect_nr=atoi(section);

            char* line=strchr(argv[4],'=');
            memmove(line, line + 1, strlen(line));
            int line_nr=atoi(line);
            extract(path,sect_nr,line_nr);

        }



    
    }
    
    
    return 0;
}