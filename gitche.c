#include <stdio.h>
#include <string.h>
#include <dirent.h>

int main(int argc , char* argv[]) {
    if(argc == 1) {
        perror("plaese enter a valid command!");
        return 1;
    }
    if(strcmp(argv[1] , "config") == 0) {
        
    }
    if(strcmp(argv[1] , "init") == 0) {
        int exist = 0;
        DIR* cdir = opendir(".");
        if(cdir == NULL) {
            perror("Error opening current directory");
            return 1;
        }
        struct dirent* file;
        while((file = opendir(cdir)) != NULL) {
            if((file->d_type == DT_DIR) && (strcmp(file->d_name , ".gitche") == 0)) {
                perror("You have initialized before!");
                return 1;
            }
        }
        mkdir(".gitche");
        
    }
    if(strcmp(argv[1] , "add") == 0) {
        
    }
    if(strcmp(argv[1] , "reset") == 0) {
        
    }
    if(strcmp(argv[1] , "status") == 0) {
        
    }
    if(strcmp(argv[1] , "commit") == 0) {
        
    }
    if(strcmp(argv[1] , "log") == 0) {
        
    }
    if(strcmp(argv[1] , "branch") == 0) {
        
    }
    if(strcmp(argv[1] , "checkout") == 0) {
        
    }
    
}