#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <conio.h>

char real_path[2000];
char gitche[2000] = "";
char branch[100] = "master";
char username[100] = "";
char email[100] = "";
struct stat attrib;
char last_change[100] = "";
char last_add[100];
char state[4];
int num_last_add;
char added_path[30][2000];
char file_name[100];
char parent_file[2000];
int file_commit[50];
char commit_time[100];
char massage[100];
char commit_branch[100];

void find_path(char* address);
void find_parent(char* address);
int found_gitche();
void found_branch();
void found_username();
void found_email();
int found_last_commit();
int date_compare(char* date1 , char* date2);
int run_config(int length , char* word[]);
int run_alias(int length , char* word[]);
int create_branch(char* name);
int run_init();
void last_modify(char* address);
int status_specifier(char* address , int* a , int* b);
int run_add_file(char* address);
int run_add_folder(char* address);
int run_add(char* address);
int run_reset_file(char* address);
int run_reset_folder(char* address);
int run_reset(char* address);
int run_status();
int run_commit(char* massage);
int find_commits(char* address , int* a);
int found_commit_info(int commit_number);

void find_path(char* address) {
    strcpy(real_path , "");
    if(address[1] == ':') strcpy(real_path , address);
    else {
        getcwd(real_path , sizeof(real_path));
        char* token;
        token = strtok(address , "/");
        strcat(real_path , "\\");
        strcat(real_path , token);
        while(1) {
            token = strtok(NULL , "/");
            if(token == NULL) break;
            strcat(real_path , "\\");
            strcat(real_path , token);
        }
    }
}

void find_parent(char* address) {
    char token[100][100];
    char* tok;
    strcpy(file_name , "");
    strcpy(parent_file , "");
    tok = strtok(address , "\\");
    strcpy(token[0] , tok);
    int i = 1;
    while(1) {
        tok = strtok(NULL , "\\");
        if(tok == NULL) break;
        strcpy(token[i] , tok);
        i++;
    }
    strcpy(file_name , token[i-1]);
    for(int j=0 ; j<i-2 ; j++) {
        strcat(parent_file , token[j]);
        strcat(parent_file , "\\");
    }
    strcat(parent_file , token[i-2]);
}

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

void found_branch() {
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    found_gitche();
    chdir(gitche);
    FILE* file = fopen("./config/branch.txt", "r");
    fscanf(file , "%s" , branch);
    fclose(file);
    chdir(work_dir);
}

void found_username() {
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    found_gitche();
    chdir(gitche);
    FILE* file = fopen("./config/username.txt", "r");
    fscanf(file , "%s" , username);
    fclose(file);
    chdir(work_dir);  
}

void found_email() {
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    found_gitche();
    chdir(gitche);
    FILE* file = fopen("./config/email.txt", "r");
    fscanf(file , "%s" , email);
    fclose(file);
    chdir(work_dir);  
}

int found_last_commit() {
    int last_commit_id = 0;
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    found_gitche();
    chdir(gitche);
    FILE* file = fopen("./config/last_commit_id.txt", "r");
    char help[10];
    fscanf(file , "%d" , &last_commit_id);
    fclose(file);
    chdir(work_dir);
    return last_commit_id;    
}

int date_compare(char* date1 , char* date2) {
    int day1 , day2;
    int month1 , month2;
    int year1 , year2;
    int hour1 , hour2;
    int minute1 , minute2;
    int second1 , second2;
    sscanf(date1 , "%d/%d/%d %d:%d:%d" , &day1 , &month1 , &year1 , &hour1 , &minute1 , &second1);
    sscanf(date2 , "%d/%d/%d %d:%d:%d" , &day2 , &month2 , &year2 , &hour2 , &minute2 , &second2);
    if(year1 > year2) return 1;
    else if(year1 < year2) return -1;
    else {
        if(month1 > month2) return 1;
        else if(month1 < month2) return -1;
        else {
            if(day1 > day2) return 1;
            else if(day1 < day1) return -1;
            else {
                if(hour1 > hour2) return 1;
                else if(hour1 < hour2) return -1;
                else {
                    if(minute1 > minute2) return 1;
                    else if(minute1 < minute2) return -1;
                    else {
                        if(second1 > second2) return 1;
                        else if(second1 < second2) return -1;
                        else return 0;
                    }
                }
            }
        }
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
    file = fopen("./head.txt", "w");
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
        chdir("./config");
        FILE* file = fopen("./last_commit_id.txt", "w");
        fprintf(file , "%d" , 0);
        fclose(file);
        file = fopen("./shortcuts.txt", "w");
        fclose(file);
        create_branch("master");
        file = fopen("./branch.txt", "w");
        fprintf(file , "master");
        fclose(file);
        chdir("..");
        char work_dir[2000];
        getcwd(work_dir , sizeof(work_dir));
        file = fopen("C:\\global\\projects.txt", "a");
        fprintf(file , "%s\n" , work_dir);
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
    FILE* check = fopen(address , "r");
    last_modify(address);
    fclose(check);
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
            fgets(state , 4 , file);
            state[strlen(state)-1] = '\0';
            if(strcmp(state , "dd") == 0) {
                fclose(file);
                chdir(work_dir);
                return -1;
            }
            check = fopen(address , "r");
            if(check != NULL) {
                if((strcmp(last_add , last_change) == 0) && (strcmp(state , "se") == 0)) {
                    fclose(file);
                    fclose(check);
                    chdir(work_dir);
                    return 1;
                }
                else {
                    fclose(file);
                    fclose(check);
                    chdir(work_dir);
                    return 0;
                }
            }
            if(check == NULL) {
                if((strcmp(state , "sd") == 0)) {
                    fclose(file);
                    fclose(check);
                    chdir(work_dir);
                    return 1;
                }
                else {
                    fclose(file);
                    fclose(check);
                    chdir(work_dir);
                    return 0;
                }                
            }
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
    if(x == -1) return -1;
    else {
        char work_dir[2000];
        getcwd(work_dir , sizeof(work_dir));
        chdir(gitche);
        chdir(branch);
        if(x == 1) {
            chdir(work_dir);
            return 0;
        }
        else if(x == 0) {
            if(track == 1) {
                FILE* file = fopen("./status.txt" , "r+");
                fseek(file , loc , SEEK_SET);
                FILE* check = fopen(address , "r");
                if(check != NULL) {
                    fprintf(file , "%s\n" , last_change);
                    fprintf(file , "se\n");
                    chdir(work_dir);
                    fclose(check);
                    fclose(file);
                    strcpy(added_path[num_last_add] , address);
                    num_last_add++;
                    return 1;
                }
                else {
                    fprintf(file , "%s\n" , last_change);
                    fprintf(file , "sd\n");
                    chdir(work_dir);
                    fclose(check);
                    fclose(file);
                    strcpy(added_path[num_last_add] , address);
                    num_last_add++;
                    return 1;
                }
            }
            else {
                FILE* check = fopen(address , "r");
                if(check == NULL) {
                    fclose(check);
                    chdir(work_dir);
                    return -1;
                }
                else {
                    FILE* file = fopen("./status.txt" , "a");
                    fprintf(file , "%s\n" , address);
                    fprintf(file , "%s\n" , last_change);
                    fprintf(file , "se\n");
                    fprintf(file , "--------------------\n");
                    fclose(file);
                    strcpy(added_path[num_last_add] , address);
                    num_last_add++;
                    chdir(work_dir);
                    return 1;
                }
            }
        } 
    }
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
            find_path(file->d_name);
            run_add_file(real_path);
        }
    }
    chdir(work_dir);
    closedir(dir);
    return 1;
}

int run_add(char* address) {
    char help[2000];
    strcpy(help , address);
    find_path(help);
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    if(chdir(real_path) == 0) {
        chdir(work_dir);
        run_add_folder(real_path);
        return 1;
    }
    else {
        int x = run_add_file(real_path);
        if(x == -1) {
            printf("\033[33m%s \033[0mdoes not exist!\n", address);
            return 0;
        }
        return 1;
    }
}

int run_reset_file(char* address) {
    int track;
    int loc;
    int x = status_specifier(address , &track , &loc);
    if(x == -1) {
        return -1;
    }
    else {
        FILE* check = fopen(real_path , "r");
        if((track == 0) && (check == NULL)) {
            fclose(check);
            return -1;
        }
        else if(loc > 0) {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            chdir(gitche);
            chdir(branch);
            FILE* file = fopen("./status.txt", "r+");
            fseek(file , loc , SEEK_SET);
            fprintf(file , "%s\n" , last_change);
            fprintf(file , "u");
            fclose(file);
            chdir(work_dir);
        }
        return 1;
    }
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
            find_path(file->d_name);
            run_reset_file(real_path);
        }
    }
    chdir(work_dir);
    closedir(dir);
    return 1;
}

int run_reset(char* address) {
    find_path(address);
    if(chdir(real_path) == 0) {
        run_reset_folder(real_path);
        return 0;
    }
    else {
        int x = run_reset_file(real_path);
        if(x == -1) {
            printf("\033[33m%s \033[0mdoes not exist!\n", address);
            return 0;
        }
        return 1;
    }
}

int run_status() {
    found_gitche();
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    int last_commit = found_last_commit();
    DIR* dir = opendir(".");
    struct dirent* file;
    char file_path[2000];
    while((file = readdir(dir)) != NULL) {
        if(file->d_type != DT_DIR) {
            sprintf(file_path , "%s\\%s" , work_dir , file->d_name);
            int track;
            int loc;
            int x = status_specifier(file_path , &track , &loc);
            if(last_commit == 0) {
                if(x == 0) printf("\033[36m%-15.15s\033[0m: \033[31m-A\n", file->d_name);
                if(x == 1) printf("\033[36m%-15.15s\033[0m: \033[32m+A\n", file->d_name);
            }
            else {
                int num_commit;
                int y = find_commits(file_path , &num_commit);
                if(y == 0) {
                    if(x == 0) printf("\033[36m%-15.15s\033[0m: \033[31m-A\n", file->d_name);
                    if(x == 1) printf("\033[36m%-15.15s\033[0m: \033[32m+A\n", file->d_name);
                }
                else {
                    char commit_name[10];
                    sprintf(commit_name , "%d" , file_commit[num_commit-1]);
                    chdir(gitche);
                    chdir(commit_name);
                    last_modify(file_path);
                    FILE* help = fopen("./information/status.txt", "r");
                    char address_check[2000];
                    while(1) {
                        fgets(address_check , 2000 , help);
                        address_check[strlen(address_check)-1] = '\0';
                        if(feof(help)) break;
                        if(strcmp(address_check , file_path) == 0) {
                            fgets(last_add , 100 , help);
                            last_add[strlen(last_add)-1] = '\0';
                            break;
                        }
                    }
                    fclose(help);
                    if(strcmp(last_add , last_change) == 0) {
                        if(x == 0) printf("\033[36m%-15.15s\033[0m: \033[31m-NM\n", file->d_name);
                        if(x == 1) printf("\033[36m%-15.15s\033[0m: \033[32m+NM\n", file->d_name);
                    }
                    else {
                        if(x == 0) printf("\033[36m%-15.15s\033[0m: \033[31m-M\n", file->d_name);
                        if(x == 1) printf("\033[36m%-15.15s\033[0m: \033[32m+M\n", file->d_name);
                    }
                }
            }
        }
    }
    closedir(dir);
    chdir(gitche);
    chdir(branch);
    FILE* for_d = fopen("./status.txt", "r");
    char address_check[2000];
    char line[30];
    while(1) {
        fgets(address_check , 2000 , for_d);
        fgets(last_add , 100 , for_d);
        fgets(state , 5 , for_d);
        fgets(line , 30 , for_d);
        if(feof(for_d)) break;
        address_check[strlen(address_check)-1] = '\0';
        state[strlen(state)-1] = '\0';
        char help[2000];
        strcpy(help , address_check);
        find_parent(help);
        if(strcmp(parent_file , work_dir) == 0) {
            FILE* help = fopen(address_check , "r");
            if(help == NULL) {
                if(strcmp(state , "sd") == 0) printf("\033[36m%-15.15s\033[0m: \033[32m+D\n", file_name);
                else if(strcmp(state , "dd") != 0) printf("\033[36m%-15.15s\033[0m: \033[31m-D\n", file_name);
            }
            fclose(help);
        }
    }
    fclose(for_d);
    chdir(work_dir);
    return 1;
}

int run_commit(char* massage) {
    found_gitche();
    found_branch();
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    chdir(gitche);
    chdir(branch);
    char address[2000];
    char last_add[100];
    char status[4];
    char line[30];
    int num_stage = 0;
    char ads[40][2000];
    FILE* file = fopen("./status.txt", "r");
    while(1) {
        chdir(gitche);
        chdir(branch);
        fgets(address , 2000 , file);
        address[strlen(address)-1] = '\0';
        fgets(last_add , 100 , file);
        last_add[strlen(last_add)-1] = '\0';
        fgets(status , 4 , file);
        status[strlen(status)-1] = '\0';
        fgets(line , 30 , file);
        line[strlen(line)-1] = '\0';
        if(feof(file)) break;
        int track = 0;
        int loc = 0;
        int x = status_specifier(address , &track , &loc);
        if(x == 1) {
            strcpy(ads[num_stage] , address);
            num_stage++;
            FILE* check = fopen(address , "r");
            if(check != NULL) {
                fclose(check);
                run_reset(address);
                chdir(work_dir);
            }
            else {
                fclose(check);
                FILE* help = fopen("./status.txt", "r+");
                fseek(help , loc , SEEK_SET);
                fprintf(help , "%s\n" , last_change);
                fprintf(help , "dd");
                fclose(help);
                chdir(work_dir);
            }
        }
    }
    fclose(file);
    if(num_stage == 0) {
        printf("\033[33mthere is nothing to commit!");
        chdir(work_dir);
        return 0;
    }
    else {
        chdir(gitche);
        chdir(branch);
        file = fopen("./status.txt", "r+");
        chdir("../config");
        int last_commit_id = found_last_commit();
        last_commit_id++;
        file = fopen("./last_commit_id.txt", "w");
        fprintf(file , "%d" , last_commit_id);
        fclose(file);
        chdir(gitche);
        char name[10];
        sprintf(name , "%d" , last_commit_id);
        mkdir(name);
        chdir(name);
        mkdir("information");
        mkdir("files");
        chdir("information");
        found_username();
        found_email();
        file = fopen("./branch.txt", "w");
        fprintf(file , "%s" , branch);
        fclose(file);
        file = fopen("./username.txt", "w");
        fprintf(file , "%s" , username);
        fclose(file);
        file = fopen("./email.txt", "w");
        fprintf(file , "%s" , email);
        fclose(file);
        file = fopen("./massage.txt", "w");
        fprintf(file , "%s" , massage);
        fclose(file);
        file = fopen("./number-of-stage.txt", "w");
        fprintf(file , "%d" , num_stage);
        fclose(file);
        file = fopen("./time.txt", "w");
        stat("./time.txt" , &attrib);
        char help[100];
        strftime(help , 100 , "%d/%m/%Y %H:%M:%S" , localtime(&attrib.st_ctime));
        fprintf(file , "%s" , help);
        fclose(file);
        chdir(gitche);
        chdir("./config");
        for(int i=0 ; i<num_stage ; i++) {
            FILE* hi = fopen("commited.txt", "a");
            fprintf(hi , "%s\n" , ads[i]);
            fprintf(hi , "%d\n" , last_commit_id);
            fprintf(hi , "--------------------\n");
        }
        char destination[2000];
        char command[2000];
        for(int i=0 ; i<num_stage ; i++) {
            FILE* check = fopen(ads[i] , "r");
            if(check != NULL) {
                fclose(check);
                char help[2000];
                strcpy(help , ads[i]);
                find_parent(help);
                sprintf(destination , "%s\\%s\\files\\%s" , gitche , name , file_name);
                sprintf(command , "copy %s %s" , ads[i] , destination);
                system(command);
            }
            if(check == NULL) fclose(check);
        }
        char help1[2000];
        char help2[2000];
        sprintf(help1 , "%s\\%s\\status.txt" , gitche , branch);
        sprintf(help2 , "%s\\%s\\information\\status.txt" , gitche , name);
        sprintf(command , "copy %s %s" , help1 , help2);
        system(command);
        printf("\033[36m      id: \033[35m%d\n", last_commit_id);
        printf("\033[36m    time: \033[35m%s\n", help);
        printf("\033[36m massage: \033[35m%s\n", massage);
        chdir(work_dir);
        return 1;
    }
}

int find_commits(char* address , int* a) {
    found_gitche();
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    chdir(gitche);
    FILE* file = fopen("./config/commited.txt", "r");
    char address_check[2000];
    int commit_id;
    *a = 0;
    while(1) {
        fgets(address_check , 2000 , file);
        address_check[strlen(address_check)-1] = '\0';
        if(feof(file)) break;
        fscanf(file , "%d\n" , &commit_id);
        fscanf(file , "--------------------\n");
        if(strcmp(address_check , address) == 0) {
            file_commit[*a] = commit_id;
            (*a)++;
        }
    }
    if((*a) == 0) return 0;
    if((*a) != 0) return 1;
}

int found_commit_info(int commit_number) {
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    found_gitche();
    chdir(gitche);
    char commit_name[10];
    sprintf(commit_name , "%d" , commit_number);
    chdir(commit_name);
    FILE* file = fopen("./information/time.txt", "r");
    fgets(commit_time , 100 , file);
    fclose(file);
    file = fopen("./information/massage.txt", "r");
    fgets(massage , 100 , file);
    fclose(file);
    file = fopen("./information/username.txt", "r");
    fgets(username , 100 , file);
    fclose(file);
    file = fopen("./information/branch.txt", "r");
    fgets(commit_branch , 100 , file);
    fclose(file);
    int n;
    file = fopen("./information/number-of-stage.txt", "r");
    fscanf(file , "%d" , &n);
    fclose(file);
    chdir(work_dir);
    return n;
}

int main(int argc , char* argv[]) {
    if(argc == 1) {
        printf("\033[33mplease enter a valid command!\033[0m");
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
            if(strncmp(argv[n] , "gitche add" , 10) == 0) ok = 1;
            if(ok == 0) {
                printf("\033[33mthe command is not valid!\033[0m");
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
        int x = found_gitche();
        if(x == 0) {
            printf("You have to initializes first!");
            return 1;
        }
        if(argc < 3) {
            printf("please wnter a valid command!");
            return 1;
        }
        if((argc == 3)) {
            if((strcmp(argv[2] , "-f") == 0) || (strcmp(argv[2] , "-n") == 0)) {
                printf("please enter a valid command!");
                return 1;
            }
            else {
                for(int i=0 ; i<strlen(argv[2]) ; i++) {
                    if(argv[2][i] == '*') {
                        printf("there is no word with this condition!");
                        return 1;
                    }
                }
                num_last_add = 0;
                run_add(argv[2]);
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
        }
        else if(strcmp(argv[2] , "-f") == 0) {
            for(int i=3 ; i<argc ; i++) {
                run_add(argv[i]);
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
        else if(strcmp(argv[2] , "-n") == 0) {
            if(strcmp(argv[3] , "1") != 0) {
                printf("this command is not supported!");
                return 1;
            }
            else {
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
                        if(x == 1) printf("\033[36m%-15.15s\033[0m: \033[32mstaged\n", file1->d_name);
                        else printf("\033[36m%-15.15s\033[0m: \033[31munstaged\n", file1->d_name);
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
                        if(is_stage == 1) printf("\033[36m%-15.15s\033[0m: \033[32mstaged\n", file1->d_name);
                        else printf("\033[36m%-15.15s\033[0m: \033[31munstaged\n", file1->d_name);
                        chdir(work_dir);
                    }
                }
                closedir(cdir);
                return 0;
            }
        }
        else {
            for(int i=2 ; i<argc ; i++) {
                run_add(argv[i]);
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
                    run_reset(address);
                }
                fclose(file);
                chdir(work_dir);
                return 0;
            }
            else {
                int exist = found_gitche();
                if(exist == 0) {
                    printf("You have to initialized first!");
                    return 1;
                }
                run_reset(argv[2]);
                return 0;
            }
        }
    }
    else if(strcmp(argv[1] , "status") == 0) {
        run_status();
        return 0;
    }
    else if(strcmp(argv[1] , "commit") == 0) {
        if(argc != 4) printf("\033[33mthe command is not valid!\033[0m");
        else if(strcmp(argv[2] , "-m") == 0) {
            if(strlen(argv[3]) > 72) printf("\033[33mthe massage is too long!\033[0m");
            else {
                run_commit(argv[3]);
            }
            return 0;
        }
        else if(strcmp(argv[2] , "-s") == 0) {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            found_gitche();
            chdir(gitche);
            FILE* file = fopen("./config/shortcuts.txt", "r");
            char shortcut[100];
            char sh_mass[100];
            char line[30];
            int ok = 0;
            while(1) {
                fgets(shortcut , 100 , file);
                fgets(sh_mass , 100 , file);
                fgets(line , 30 , file);
                if(feof(file)) break;
                shortcut[strlen(shortcut)-1] = '\0';
                sh_mass[strlen(sh_mass)-1] = '\0';
                if(strcmp(argv[3] , shortcut) == 0) {
                    ok = 1;
                    break;
                }
            }
            fclose(file);
            if(ok == 0) printf("\033[33mthe shortcut name does not exist!\033[0m");
            else run_commit(sh_mass);
            chdir(work_dir);
        }
        else {
            printf("\033[33mthe comand is not valid!\033[0m");
            return 1;
        }
    }
    else if(strcmp(argv[1] , "set") == 0) {
        if(argc != 6) printf("\033[33mplease enter a valid command!\033[0m");
        else if((strcmp(argv[2] , "-m") != 0) || (strcmp(argv[4] , "-s") != 0)) printf("\033[33mplease enter a valid command!\033[0m");
        else {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            found_gitche();
            chdir(gitche);
            FILE* file = fopen("./config/shortcuts.txt", "r");
            char shortcut[100];
            char sh_mass[100];
            char line[30];
            while(1) {
                fgets(shortcut , 100 , file);
                fgets(sh_mass , 100 , file);
                fgets(line , 30 , file);
                if(feof(file)) break;
                shortcut[strlen(shortcut)-1] = '\0';
                if(strcmp(argv[5] , shortcut) == 0) {
                    printf("\033[33mthe shortcut name is already exist!\033[0m");
                    fclose(file);
                    return 1;
                }
            }
            fclose(file);
            file = fopen("./config/shortcuts.txt", "a");
            fprintf(file , "%s\n" , argv[5]);
            fprintf(file , "%s\n" , argv[3]);
            fprintf(file , "--------------------\n");
            chdir(work_dir);
        }
        return 0;
    }
    else if(strcmp(argv[1] , "replace") == 0) {
        if(argc != 6) printf("\033[33mplease enter a valid command!\033[0m");
        else if((strcmp(argv[2] , "-m") != 0) || (strcmp(argv[4] , "-s") != 0)) printf("\033[33mplease enter a valid command!\033[0m");
        else {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            found_gitche();
            chdir(gitche);
            FILE* file = fopen("./config/shortcuts.txt", "r");
            char shortcut[100];
            char sh_mass[100];
            char line[30];
            int ok = 0;
            while(1) {
                fgets(shortcut , 100 , file);
                fgets(sh_mass , 100 , file);
                fgets(line , 30 , file);
                if(feof(file)) break;
                shortcut[strlen(shortcut)-1] = '\0';
                if(strcmp(argv[5] , shortcut) == 0) {
                    ok = 1;
                    break;
                }
            }
            fclose(file);
            if(ok == 0) printf("\033[33mthe shortcut name does not exist!\033[0m");
            else {
                file = fopen("./config/shortcuts.txt", "r");
                FILE* help = fopen("./config/tempsh.txt", "w");
                while(1) {
                    fgets(shortcut , 100 , file);
                    fputs(shortcut , help);
                    fgets(sh_mass , 100 , file);
                    fgets(line , 30 , file);
                    if(feof(file)) break;
                    shortcut[strlen(shortcut)-1] = '\0';
                    if(strcmp(argv[5] , shortcut) == 0) {
                        fprintf(help , "%s\n" , argv[3]);
                    }
                    else fputs(sh_mass , help);
                    fputs(line , help);
                }
                fclose(file);
                fclose(help);
                file = fopen("./config/shortcuts.txt", "w");
                help = fopen("./config/tempsh.txt", "r");
                while(1) {
                    fgets(shortcut , 100 , help);
                    fgets(sh_mass , 100 , help);
                    fgets(line , 30 , help);
                    if(feof(help)) break;
                    fputs(shortcut , file);
                    fputs(sh_mass , file);
                    fputs(line , file);
                }
                fclose(file);
                fclose(help);
                remove("./config/tempsh.txt");
            }
            chdir(work_dir);
        }
        return 0;
    }
    else if(strcmp(argv[1] , "remove") == 0) {
        if(argc != 4) printf("\033[33mplease enter a valid command!\033[0m");
        else if((strcmp(argv[2] , "-s") != 0)) printf("\033[33mplease enter a valid command!\033[0m");
        else {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            found_gitche();
            chdir(gitche);
            FILE* file = fopen("./config/shortcuts.txt", "r");
            char shortcut[100];
            char sh_mass[100];
            char line[30];
            int ok = 0;
            while(1) {
                fgets(shortcut , 100 , file);
                fgets(sh_mass , 100 , file);
                fgets(line , 30 , file);
                if(feof(file)) break;
                shortcut[strlen(shortcut)-1] = '\0';
                if(strcmp(argv[3] , shortcut) == 0) {
                    ok = 1;
                    break;
                }
            }
            fclose(file);
            if(ok == 0) printf("\033[33mthe shortcut name does not exist!\033[0m");
            else {
                file = fopen("./config/shortcuts.txt", "r");
                FILE* help = fopen("./config/tempsh.txt", "w");
                while(1) {
                    fgets(shortcut , 100 , file);
                    fgets(sh_mass , 100 , file);
                    fgets(line , 30 , file);
                    if(feof(file)) break;
                    shortcut[strlen(shortcut)-1] = '\0';
                    if(strcmp(argv[3] , shortcut) != 0) {
                        fprintf(help , "%s\n" , shortcut);
                        fprintf(help , "%s" , sh_mass);
                        fprintf(help , "%s" , line);
                    }
                }
                fclose(file);
                fclose(help);
                file = fopen("./config/shortcuts.txt", "w");
                help = fopen("./config/tempsh.txt", "r");
                while(1) {
                    fgets(shortcut , 100 , help);
                    fgets(sh_mass , 100 , help);
                    fgets(line , 30 , help);
                    if(feof(help)) break;
                    fprintf(file , "%s" , shortcut);
                    fprintf(file , "%s" , sh_mass);
                    fprintf(file , "%s" , line);
                }
                fclose(file);
                fclose(help);
                remove("./config/tempsh.txt");
            }
            chdir(work_dir);
        }
        return 0;
    }
    else if(strcmp(argv[1] , "log") == 0) {
        int last_commit = found_last_commit();
        if(argc == 2) {
            for(int i=last_commit ; i>0 ; i--) {
                int x = found_commit_info(i);
                printf("\033[35mcommit id:\033[36m%03d  ----  \033[0mon branch \033[36m%s\n", i , commit_branch);
                printf("\033[36m%02d \033[0mfile(s) commited  \033[35muser: \033[36m%s\n", x , username);
                printf("\033[35mmassage:  \033[36m%s\n", massage);
                printf("\033[35mtime:     \033[36m%s\n", commit_time);
                printf("\033[0m--------------------------------------------------\n");
            }
        }
        else if(argc == 4) {
            if(strcmp(argv[2] , "-n") == 0) {
                int num;
                sscanf(argv[3] , "%d" , &num);
                int max = 0;
                if(num < last_commit) max = last_commit - num;
                for(int i=last_commit ; i>max ; i--) {
                    int x = found_commit_info(i);
                    printf("\033[35mcommit id:\033[36m%03d  ----  \033[0mon branch \033[36m%s\n", i , commit_branch);
                    printf("\033[36m%02d \033[0mfile(s) commited  \033[35muser: \033[36m%s\n", x , username);
                    printf("\033[35mmassage:  \033[36m%s\n", massage);
                    printf("\033[35mtime:     \033[36m%s\n", commit_time);
                    printf("\033[0m--------------------------------------------------\n");
                }
            }
            else if(strcmp(argv[2] , "-branch") == 0) {
                int ok = 0;
                for(int i=last_commit ; i>0 ; i--) {
                    int x = found_commit_info(i);
                    if(strcmp(commit_branch , argv[3]) != 0) continue;
                    ok = 1;
                    printf("\033[35mcommit id:\033[36m%03d  ----  \033[0mon branch \033[36m%s\n", i , commit_branch);
                    printf("\033[36m%02d \033[0mfile(s) commited  \033[35muser: \033[36m%s\n", x , username);
                    printf("\033[35mmassage:  \033[36m%s\n", massage);
                    printf("\033[35mtime:     \033[36m%s\n", commit_time);
                    printf("\033[0m--------------------------------------------------\n");
                }
                if(ok == 0) printf("\033[33mthere is no commit with the given condition!");
            }
            else if(strcmp(argv[2] , "-author") == 0) {
                int ok = 0;
                for(int i=last_commit ; i>0 ; i--) {
                    int x = found_commit_info(i);
                    if(strcmp(username , argv[3]) != 0) continue;
                    ok = 1;
                    printf("\033[35mcommit id:\033[36m%03d  ----  \033[0mon branch \033[36m%s\n", i , commit_branch);
                    printf("\033[36m%02d \033[0mfile(s) commited  \033[35muser: \033[36m%s\n", x , username);
                    printf("\033[35mmassage:  \033[36m%s\n", massage);
                    printf("\033[35mtime:     \033[36m%s\n", commit_time);
                    printf("\033[0m--------------------------------------------------\n");
                }
                if(ok == 0) printf("\033[33mthere is no commit with the given condition!");
            }
            else if(strcmp(argv[2] , "-since") == 0) {
                int ok = 0;
                for(int i=last_commit ; i>0 ; i--) {
                    int x = found_commit_info(i);
                    if(date_compare(commit_time , argv[3]) == (-1)) continue;
                    ok = 1;
                    printf("\033[35mcommit id:\033[36m%03d  ----  \033[0mon branch \033[36m%s\n", i , commit_branch);
                    printf("\033[36m%02d \033[0mfile(s) commited  \033[35muser: \033[36m%s\n", x , username);
                    printf("\033[35mmassage:  \033[36m%s\n", massage);
                    printf("\033[35mtime:     \033[36m%s\n", commit_time);
                    printf("\033[0m--------------------------------------------------\n");
                }
                if(ok == 0) printf("\033[33mthere is no commit with the given condition!");
            }
            else if(strcmp(argv[2] , "-before") == 0) {
                int ok = 0;
                for(int i=last_commit ; i>0 ; i--) {
                    int x = found_commit_info(i);
                    if(date_compare(commit_time , argv[3]) == 1) continue;
                    ok = 1;
                    printf("\033[35mcommit id:\033[36m%03d  ----  \033[0mon branch \033[36m%s\n", i , commit_branch);
                    printf("\033[36m%02d \033[0mfile(s) commited  \033[35muser: \033[36m%s\n", x , username);
                    printf("\033[35mmassage:  \033[36m%s\n", massage);
                    printf("\033[35mtime:     \033[36m%s\n", commit_time);
                    printf("\033[0m--------------------------------------------------\n");
                }
                if(ok == 0) printf("\033[33mthere is no commit with the given condition!");
            }
            else if(strcmp(argv[2] , "-search") == 0) {
                int ok = 0;
                for(int i=last_commit ; i>0 ; i--) {
                    int x = found_commit_info(i);
                    if(strstr(massage , argv[3]) == NULL) continue;
                    ok = 1;
                    printf("\033[35mcommit id:\033[36m%03d  ----  \033[0mon branch \033[36m%s\n", i , commit_branch);
                    printf("\033[36m%02d \033[0mfile(s) commited  \033[35muser: \033[36m%s\n", x , username);
                    printf("\033[35mmassage:  \033[36m%s\n", massage);
                    printf("\033[35mtime:     \033[36m%s\n", commit_time);
                    printf("\033[0m--------------------------------------------------\n");
                }
                if(ok == 0) printf("\033[33mthere is no commit with the given condition!");
            }
        }
        else if(strcmp(argv[2] , "-search") == 0) {
            int ok = 0;
            for(int i=last_commit ; i>0 ; i--) {
                int x = found_commit_info(i);
                int show = 0;
                for(int j=3 ; j<argc ; j++) {
                    if(strstr(massage , argv[j]) != NULL) {
                        show = 1;
                        break;
                    }
                }
                if(show == 0) continue;
                ok = 1;
                printf("\033[35mcommit id:\033[36m%03d  ----  \033[0mon branch \033[36m%s\n", i , commit_branch);
                printf("\033[36m%02d \033[0mfile(s) commited  \033[35muser: \033[36m%s\n", x , username);
                printf("\033[35mmassage:  \033[36m%s\n", massage);
                printf("\033[35mtime:     \033[36m%s\n", commit_time);
                printf("\033[0m--------------------------------------------------\n");
            }
            if(ok == 0) printf("\033[33mthere is no commit with the given condition!");
        }
        else printf("\033[33mplease enter a valid command!\033[0m");
        return 0;
    }
    else if(strcmp(argv[1] , "branch") == 0) {

    }
    else if(strcmp(argv[1] , "checkout") == 0) {

    }
    else {
        if(argc > 2) {
            printf("\033[33mplaese enter a valid command!\033[0m");
            return 1;
        }
        else {
            int exist = found_gitche();
            if(exist == 0) {
                printf("\033[33mYou have to initialized first!\033[0m");
                return 1;
            }
            else {
                char work_dir[2000];
                getcwd(work_dir , sizeof(work_dir));
                chdir(gitche);
                FILE* alias = fopen("./config/aliases.txt", "r");
                if(alias == NULL) {
                    printf("\033[33mplaese enter a valid command!\033[0m");
                    return 1;
                }
                else {
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
                        printf("\033[33mplease enter a valid command!\033[0m");
                        return 1;
                    }
                    else {
                        FILE* help = fopen("C:\\global\\help.txt", "w+");
                        fprintf(help , "%s" , real_command);
                        rewind(help);
                        int number = 0;
                        char* word[100];
                        for(int i=0 ; i<100 ; i++) {
                            word[i] = (char*) malloc(100 * sizeof(char));
                        }
                        char god[100];
                        while(1) {
                            fscanf(help , "%s" , god);
                            if(feof(help)) break;
                            strcpy(word[number] , god);
                            number++;
                        }
                        fclose(help);
                        remove("C:\\global\\help.txt");
                        return main(number , word);
                    }
                }
            }
        }
    }
}