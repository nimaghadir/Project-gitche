#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

char gitche[2000] = "";
struct tm* foo;
struct stat attrib;

int found_gitche() {
    char work_dir[2000] = "";
    char cor_dir[2000] = "";
    char cor_dir2[2000] = "";
    getcwd(work_dir , sizeof(work_dir));
    while(1) {
        getcwd(cor_dir , sizeof(cor_dir));
        if(strcmp(cor_dir , cor_dir2) == 0) {
            chdir(work_dir);
            strcpy(gitche , "");
            return 0;
        }
        getcwd(cor_dir2 , sizeof(cor_dir));
        DIR* cdir = opendir(".");
        struct dirent* file;
        while((file = readdir(cdir)) != NULL) {
            if((file->d_type == DT_DIR) && (strcmp(file->d_name , ".gitche") == 0)) {
                closedir(cdir);
                chdir(".gitche");
                getcwd(gitche , sizeof(gitche));
                chdir(work_dir);
                return 1;
            }
        }
        closedir(cdir);
        chdir("..");
    }
}

int run_config(int length , char* word[]) {
    if(strcmp(word[2] , "-global") == 0) {
        if(strcmp(word[3] , "user.name") == 0) {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            FILE* projects = fopen("C:\\global\\projects.txt", "r");
            while(1) {
                char loc[2000];
                fgets(loc , sizeof(loc) , projects);
                if(feof(projects)) break;
                loc[strlen(loc)-1] = '\0';
                chdir(loc);
                chdir("./config");
                FILE* file = fopen("username.txt", "w");
                fprintf(file , "%s" , word[4]);
                fclose(file);
            }
            fclose(projects);
            chdir(work_dir);
            return 1;
        }
        if(strcmp(word[3] , "user.email") == 0) {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            FILE* projects = fopen("C:\\global\\projects.txt", "r");
            while(1) {
                char loc[2000];
                fgets(loc , sizeof(loc) , projects);
                if(feof(projects)) break;
                loc[strlen(loc)-1] = '\0';
                chdir(loc);
                chdir("./config");
                FILE* file = fopen("email.txt", "w");
                fprintf(file , "%s" , word[4]);
                fclose(file);
            }
            fclose(projects);
            chdir(work_dir);
            return 1;
        }
    }
    int exist = found_gitche();
    if(exist == 0) {
        printf("You have to initializes first!");
        return 0;
    }
    if(exist == 1) {
        if(strcmp(word[2] , "user.name") == 0) {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            chdir(gitche);
            chdir("./config");
            FILE* file = fopen("username.txt", "w");
            fprintf(file , "%s" , word[3]);
            fclose(file);
            chdir(work_dir);
            return 1;
        }
        if(strcmp(word[2] , "user.email") == 0) {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            chdir(gitche);
            chdir("./config");
            FILE* file = fopen("email.txt", "w");
            fprintf(file , "%s" , word[3]);
            fclose(file);
            chdir(work_dir);
            return 1;
        }
    }
}

int run_alias(int length , char* word[]) {
    if(strcmp(word[2] , "global") == 0) {
        char work_dir[2000];
        getcwd(work_dir , sizeof(work_dir));
        FILE* projects = fopen("C:\\global\\projects.txt", "r");
        while(1) {
            char loc[2000];
            fgets(loc , sizeof(loc) , projects);
            if(feof(projects)) break;
            loc[strlen(loc)-1] = '\0';
            chdir(loc);
            chdir("./config");
            FILE* alias = fopen("aliases.txt", "a");
            for(int i=6 ; i<strlen(word[3]) ; i++) {
            fprintf(alias , "%c" , word[3][i]);
            }
            fprintf(alias , "\n");
            fprintf(alias , "%s\n" , word[4]);
            fprintf(alias , "--------------------\n");
            fclose(alias);
        }
        fclose(projects);
        chdir(work_dir);
        return 1;
    }
    int exist = found_gitche();
    if(exist == 0) {
        printf("You have to initialized first!");
        return 0;
    }
    if(exist == 1) {
        char work_dir[2000];
        getcwd(work_dir , sizeof(work_dir));
        chdir("./.gitche/config");
        FILE* alias = fopen("aliases.txt" , "a");
        for(int i=6 ; i<strlen(word[2]) ; i++) {
        fprintf(alias , "%c" , word[2][i]);                    
        }
        fprintf(alias , "\n");
        fprintf(alias , "%s\n", word[3]);
        fprintf(alias , "--------------------\n");
        fclose(alias);
        chdir(work_dir);
        return 1;
    }
}

int run_init() {
    int exist = found_gitche();
    if(exist == 1) {
        printf("You have already initialized!");
        return 0;
    }
    if(exist == 0) {
        mkdir(".gitche");
        chdir("./.gitche");
        mkdir("./config");
        char work_dir[2000];
        getcwd(work_dir , sizeof(work_dir));
        FILE* file;
        file = fopen("C:\\global\\projects.txt", "a");
        fprintf(file , "%s\n" , work_dir);
        fclose(file);
        chdir("..");
        return 1;
    }
}

int run_add_file(char* address) {
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    stat(address , &attrib);
    FILE* file = fopen(address , "r");
    
}

int run_add_folder(char* address) {

}

int main(int argc , char* argv[]) {
    if(strcmp(argv[1] , "config") == 0) {
        if((strncmp(argv[2] , "alias." , 6) == 0) || (strncmp(argv[3] , "alias." , 6) == 0)) {
            int n = 3;
            if(strcmp(argv[2] , "-global") == 0) n = 4;
            int ok = 0;
            if(strncmp(argv[n] , "gitche config -global user.name" , 31) == 0) ok = 1;
            if(strncmp(argv[n] , "gitche config -global user.email" , 32) == 0) ok = 1;
            if(strncmp(argv[n] , "gitche config user.name" , 23) == 0) ok = 1;
            if(strncmp(argv[n] , "gitche config user.email" , 24) == 0) ok = 1;
            if(strncmp(argv[n] , "gitche config -global alias." , 28) == 0) ok = 1;
            if(strncmp(argv[n] , "gitche config -global alias." , 28) == 0) ok = 1;
            if(strncmp(argv[n] , "gitche init" , 11) == 0) ok = 1;
            if(ok == 0) {
                printf("the command is not valid!");
                return 1;
            }
            run_alias(argc , argv);
            return 0;
        }
        run_config(argc , argv);
        return 0;
    }
    if(strcmp(argv[1] , "init") == 0) {
        run_init();
        return 0;
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
    if(argc != 2) {
        printf("plaese enter a valid command!");
        return 1;
    }
    int exist = found_gitche();
    if(exist == 0) {
        printf("You have to initialized first!");
        return 1;
    }
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    chdir(gitche);
    FILE* alias = fopen("./config/aliases.txt", "r");
    char real_command[2000];
    int ok = 0;
    while(1) {
        char command[2000];
        fgets(command , sizeof(command) , alias);
        if(feof(alias)) break;
        command[strlen(command)-1] = '\0';
        if(strcmp(command , argv[1]) == 0) {
            fgets(real_command , sizeof(real_command) , alias);
            ok = 1;
            break;
        }
    }
    fclose(alias);
    chdir(work_dir);
    if(ok == 0) {
        printf("please enter a valid command!");
        return 1;
    }
    FILE* help = fopen("C:\\global\\help.txt", "w+");
    fprintf(help , "%s" , real_command);
    rewind(help);
    int number = 0;
    char* word[100];
    for(int i=0 ; i<100 ; i++) {
        word[i] = (char*) malloc(100 * sizeof(char));
    }
    while(1) {
        fscanf(help , "%s" , word[number]);
        number++;
        if(feof(help)) break;
    }
    fclose(help);
    remove("C:\\global\\help.txt");
    return main(number , word);
}