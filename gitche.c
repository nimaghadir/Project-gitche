#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <conio.h>

char gitche[2000] = "";
char branch[100] = "master";
struct stat attrib;
char last_change[100] = "";
char last_add[100];
char state[3];
int num_last_add;
char added_path[30][2000];

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
    if(strcmp(word[2] , "-global") == 0) {
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

int create_branch(char* name) {
    found_gitche();
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    chdir(gitche);
    mkdir(name);
    chdir(name);
    FILE* file = fopen("./status.txt", "w");
    fclose(file);
    chdir(work_dir);
    return 0;
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
        create_branch("master");
        char work_dir[2000];
        getcwd(work_dir , sizeof(work_dir));
        FILE* file;
        file = fopen("C:\\global\\projects.txt", "a");
        fprintf(file , "%s\n" , work_dir);
        fclose(file);
        file = fopen("./branch.txt", "w");
        fprintf(file , "master");
        fclose(file);
        chdir("..");
        return 1;
    }
}

void last_modify(char* address) {
    stat(address , &attrib);
    strftime(last_change , 100 , "%d/%m/%Y %H:%M:%S" , localtime(&attrib.st_mtime));
}

int status_specifier(char* address , int* a , int* b) {
    found_gitche();
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    chdir(gitche);
    chdir(branch);
    last_modify(address);
    FILE* file = fopen("./status.txt" , "r+");
    *a = 0;
    *b = 0;
    strcpy(last_add , "");
    strcpy(state , "");
    while(1) {
        char address_check[2000] = "";
        fgets(address_check , 2000 , file);
        address_check[strlen(address_check)-1] = '\0';
        if(feof(file)) break;
        if(strcmp(address , address_check) == 0) {
            *a = 1;
            *b = ftell(file);
            fgets(last_add , 100 , file);
            last_add[strlen(last_add)-1] = '\0';
            fgets(state , 3 , file);
            state[strlen(state)-1] = '\0';
            if((strcmp(last_add , last_change) == 0) && (strcmp(state , "s") == 0)) {
                chdir(work_dir);
                return 1;
            }
            break;
        }
    }
    fclose(file);
    chdir(work_dir);
    return 0;
}

int run_add_file(char* address) {
    int exist = found_gitche();
    if(exist == 0) {
        printf("You have to initialize first!");
        return 0;
    }
    int track;
    int loc;
    int x = status_specifier(address , &track , &loc);
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    chdir(gitche);
    chdir(branch);
    if(x == 1) {
        chdir(work_dir);
        return 0;
    }
    if(track == 1) {
        FILE* file = fopen("./status.txt" , "r+");
        fseek(file , loc , SEEK_SET);
        fprintf(file , "%s\n" , last_change);
        fprintf(file , "s\n");
        chdir(work_dir);
        fclose(file);
        strcpy(added_path[num_last_add] , address);
        num_last_add++;
        return 1;
    }
    FILE* file = fopen("./status.txt" , "a");
    fprintf(file , "%s\n" , address);
    fprintf(file , "%s\n" , last_change);
    fprintf(file , "s\n");
    fprintf(file , "--------------------\n");
    fclose(file);
    strcpy(added_path[num_last_add] , address);
    num_last_add++;
    chdir(work_dir);
    return 1;
}

int run_add_folder(char* address) {
    int exist = found_gitche();
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    chdir(address);
    DIR* dir = opendir(address);
    struct dirent* file;
    while((file = readdir(dir)) != NULL) {
        if(file->d_type != DT_DIR) {
            char help[2000];
            strcpy(help , address);
            strcat(help , "\\");
            strcat(help , file->d_name);
            run_add_file(help);
        }
    }
    chdir(work_dir);
    closedir(dir);
    return 1;
}

int run_reset_file(char* address) {
    int track;
    int loc;
    status_specifier(address , &track , &loc);
    if(loc > 0) {
        char work_dir[2000];
        getcwd(work_dir , sizeof(work_dir));
        chdir(gitche);
        chdir(branch);
        FILE* file = fopen("./status.txt", "r+");
        fseek(file , loc , SEEK_SET);
        fprintf(file , "%s\n" , last_add);
        fprintf(file , "u");
        fclose(file);
        chdir(work_dir);
    }
    return 1;
}

int run_reset_folder(char* address) {
    int exist = found_gitche();
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    chdir(address);
    DIR* dir = opendir(address);
    struct dirent* file;
    while((file = readdir(dir)) != NULL) {
        if(file->d_type != DT_DIR) {
            char help[2000];
            strcpy(help , address);
            strcat(help , "\\");
            strcat(help , file->d_name);
            run_reset_file(help);
        }
    }
    chdir(work_dir);
    closedir(dir);
    return 1;
}

int main(int argc , char* argv[]) {
    if(argc == 1) {
        printf("please enter a valid command!");
        return 1;
    }
    else if(strcmp(argv[1] , "config") == 0) {
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
    else if(strcmp(argv[1] , "init") == 0) {
        run_init();
        return 0;
    }
    else if(strcmp(argv[1] , "add") == 0) {
        if(argc < 3) {
            printf("please wnter a valid command!");
            return 1;
        }
        if((argc == 3)) {
            if((strcmp(argv[2] , "-f") == 0) || (strcmp(argv[2] , "-n") == 0)) {
                printf("please enter a valid command!");
                return 1;
            }
            for(int i=0 ; i<strlen(argv[2]) ; i++) {
                if(argv[2][i] == '*') {
                    printf("there is no word with this condition!");
                    return 1;
                }
            }
            num_last_add = 0;
            if(chdir(argv[2]) == 0) {
                run_add_folder(argv[2]);
                if(num_last_add != 0) {
                    found_gitche();
                    char work_dir[2000];
                    getcwd(work_dir , sizeof(work_dir));
                    chdir(gitche);
                    chdir(branch);
                    FILE* l_add = fopen("./last-add.txt", "w");
                    for(int i=0 ; i<num_last_add ; i++) {
                        fprintf(l_add , "%s\n" , added_path[i]);
                    }
                    fclose(l_add);
                    chdir(work_dir);
                }
                return 0;
            }
            FILE* help = fopen(argv[2] , "r");
            if(help != NULL) {
                fclose(help);
                run_add_file(argv[2]);
                if(num_last_add != 0) {
                    found_gitche();
                    char work_dir[2000];
                    getcwd(work_dir , sizeof(work_dir));
                    chdir(gitche);
                    chdir(branch);
                    FILE* l_add = fopen("./last-add.txt", "w");
                    for(int i=0 ; i<num_last_add ; i++) {
                        fprintf(l_add , "%s\n" , added_path[i]);
                    }
                    fclose(l_add);
                    chdir(work_dir);
                }
                return 0;
            }
            printf("%s does not exist!", argv[2]);
            return 0;
        }
        if(strcmp(argv[2] , "-f") == 0) {
            for(int i=3 ; i<argc ; i++) {
                FILE* help = fopen(argv[i] , "r");
                if(help != NULL) {
                    fclose(help);
                    run_add_file(argv[i]);
                }
                else if(chdir(argv[i]) == 0) {
                    run_add_folder(argv[i]);
                }
                else printf("%s does not exist!\n", argv[i]);
            }
            if(num_last_add != 0) {
                found_gitche();
                char work_dir[2000];
                getcwd(work_dir , sizeof(work_dir));
                chdir(gitche);
                chdir(branch);
                FILE* l_add = fopen("./last-add.txt", "w");
                for(int i=0 ; i<num_last_add ; i++) {
                    fprintf(l_add , "%s\n" , added_path[i]);
                }
                fclose(l_add);
                chdir(work_dir);
            }
            return 0;
        }
        if(strcmp(argv[2] , "-n") == 0) {
            if(strcmp(argv[3] , "1") != 0) {
                printf("this command is not supported!");
                return 1;
            }
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            DIR* cdir = opendir(".");
            struct dirent* file1;
            while((file1 = readdir(cdir)) != NULL) {
                if(file1->d_type != DT_DIR) {
                    char help[2000];
                    strcpy(help , work_dir);
                    strcat(help , "\\");
                    strcat(help , file1->d_name);
                    int track;
                    int loc;
                    int x = status_specifier(help , &track , &loc);
                    if(x == 1) printf("\033[0m%-15.15s: \033[32mstaged\n", file1->d_name);
                    else printf("\033[0m%-15.15s: \033[31munstaged\n", file1->d_name);
                    chdir(work_dir);
                }
                if(file1->d_type == DT_DIR) {
                    if((strcmp(file1->d_name , ".gitche") == 0) || (strcmp(file1->d_name , ".") == 0) || (strcmp(file1->d_name , "..") == 0)) continue;
                    int is_stage = 1;
                    DIR* dir = opendir(file1->d_name);
                    struct dirent* file2;
                    while((file2 = readdir(dir)) != NULL) {
                        if(file2->d_type != DT_DIR) {
                            char help[2000];
                            strcpy(help , work_dir);
                            strcat(help , "\\");
                            strcat(help , file1->d_name);
                            strcat(help , "\\");
                            strcat(help , file2->d_name);
                            int track;
                            int loc;
                            int x = status_specifier(help , &track , &loc);
                            if(x == 0) {
                                is_stage = 0;
                                break;
                            }
                        }
                    }
                    if(is_stage == 1) printf("\033[0m%-15.15s: \033[32mstaged\n", file1->d_name);
                    else printf("\033[0m%-15.15s: \033[31munstaged\n", file1->d_name);
                    chdir(work_dir);
                }
            }
            closedir(cdir);
            return 0;
        }
        for(int i=2 ; i<argc ; i++) {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            FILE* help = fopen(argv[i] , "r");
            if(help != NULL) {
                fclose(help);
                char help[2000];
                getcwd(help , sizeof(help));
                strcat(help , "\\");
                strcat(help , argv[i]);
                run_add_file(help);
            }
            else if((chdir(argv[i]) == 0) && (strcmp(argv[i] , ".gitche") != 0)) {
                chdir(work_dir);
                char help[2000];
                getcwd(help , sizeof(help));
                strcat(help , "\\");
                run_add_folder(help);
            }
            else printf("%s does not exist!\n", argv[i]);
        }
        if(num_last_add != 0) {
            found_gitche();
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            chdir(gitche);
            chdir(branch);
            FILE* l_add = fopen("./last-add.txt", "w");
            for(int i=0 ; i<num_last_add ; i++) {
                fprintf(l_add , "%s\n" , added_path[i]);
            }
            fclose(l_add);
            chdir(work_dir);
        }
        return 0;
    }
    else if(strcmp(argv[1] , "reset") == 0) {
        if(argc != 3) {
            printf("please wnter a valid command!");
            return 1;
        }
        if((argc == 3)) {
            if((strcmp(argv[2] , "-f") == 0) || (strcmp(argv[2] , "-n") == 0)) {
                printf("this command is not supported!");
                return 1;
            }
            if(strcmp(argv[2] , "-undo") == 0) {
                int exist = found_gitche();
                if(exist == 0) {
                    printf("You have to initialized first!");
                    return 1;
                }
                char work_dir[2000];
                getcwd(work_dir , sizeof(work_dir));
                chdir(gitche);
                chdir(branch);
                char address[2000];
                FILE* file = fopen("./last-add.txt", "r");
                while(1) {
                    fgets(address , sizeof(address) , file);
                    if(feof(file)) break;
                    address[strlen(address)-1] = '\0';
                    if(chdir(address) == 0) {
                        run_reset_folder(address);
                    }
                    FILE* help = fopen(address , "r");
                    if(help != NULL) {
                        fclose(help);
                        run_reset_file(address);
                    }
                }
                fclose(file);
                chdir(work_dir);
                return 0;
            }
            if(chdir(argv[2]) == 0) {
                run_reset_folder(argv[2]);
                return 0;
            }
            FILE* help = fopen(argv[2] , "r");
            if(help != NULL) {
                fclose(help);
                run_reset_file(argv[2]);
                return 0;
            }
            printf("%s does not exist!", argv[2]);
            return 0;
        }
    }
    else if(strcmp(argv[1] , "status") == 0) {
        return 0;
    }
    else if(strcmp(argv[1] , "commit") == 0) {
        return 0;
    }
    if(strcmp(argv[1] , "log") == 0) {
        return 0;
    }
    else if(strcmp(argv[1] , "branch") == 0) {
        return 0;
    }
    else if(strcmp(argv[1] , "checkout") == 0) {
        return 0;
    }
    else {
        if(argc > 2) {
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
}