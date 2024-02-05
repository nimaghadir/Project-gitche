#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <conio.h>

char address_copy[2000];
char real_path[2000];
char gitche[2000] = "";
char branch[100] = "";
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
char tag_name[100];
char tag_username[100];
char tag_date[100];
char tag_mass[100];

void find_path(char* address);
void find_parent(char* address);
int found_gitche();
void found_branch();
void found_username();
void found_email();
int found_last_commit();
int find_branch_head(char* name);
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
int find_in_commit(char* address , int comm_num);
int found_commit_info(int commit_number);
int working_tree_clean_check(int* clean);
int run_checkout(char* comm_name);
int find_full_path(char* fi_name);
int run_tag(char* tag_name , char* tag_mass , int commit_id , int writable);
int find_tag_info(char* tag , int* a , int* b);
void left_shift(char* a , int first , int size , int bit);
void sort(char* a);
int file_line(char* address);
int file_compare(char* address1 , char* adddress2 , int start1 , int finish1 , int start2 , int finish2);
int run_grep(char* file_n , char* word , char* comm_name , int line_show);


void find_path(char* address) {
    strcpy(address_copy , address);
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
    strcpy(address , address_copy);
}

void find_parent(char* address) {
    char token[100][100];
    char* tok;
    char address_copy[2000];
    strcpy(address_copy , address);
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
    strcpy(address , address_copy);
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

int find_branch_head(char* name) {
    char work_dir[2000];
    getcwd(work_dir , 2000);
    chdir(gitche);
    chdir(name);
    int head;
    FILE* file = fopen("./head.txt", "r");
    fscanf(file , "%d" , &head);
    fclose(file);
    chdir(work_dir);
    return head;
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
    int n = find_branch_head(branch);
    fprintf(file , "%d" , n);
    fclose(file);
    file = fopen("./last-add.txt", "w");
    fclose(file);
    chdir(gitche);
    file = fopen("./config/branch-names.txt", "a");
    fprintf(file , "%s\n" , name);
    fclose(file);
    chdir(work_dir);
    return 0;
}

int run_init() {
    int exist = found_gitche();
    if(exist == 1) {
        printf("\033[32mYou have already initialized!\033[0m");
        return 0;
    }
    if(exist == 0) {
        mkdir(".gitche");
        chdir("./.gitche");
        mkdir("./staging-area");
        mkdir("./config");
        chdir("./config");
        FILE* file = fopen("./last_commit_id.txt", "w");
        fprintf(file , "%d" , 0);
        fclose(file);
        file = fopen("./current-commit-id.txt", "w");
        fprintf(file , "%d" , 0);
        fclose(file);
        file = fopen("./shortcuts.txt", "w");
        fclose(file);
        file = fopen("./tags.txt", "w");
        fclose(file);
        file = fopen("./username.txt", "w");
        fclose(file);
        file = fopen("./email.txt", "w");
        fclose(file);
        file = fopen("./aliases.txt", "w");
        fclose(file);
        create_branch("master");
        chdir(gitche);
        chdir("master");
        file = fopen("./head.txt", "w");
        fprintf(file , "%d" , 0);
        fclose(file);
        chdir("../config");
        file = fopen("./branch.txt", "w");
        fprintf(file , "master");
        fclose(file);
        file = fopen("./branch-names.txt", "w");
        fprintf(file , "master\n");
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
                    find_parent(address);
                    char destination[2000];
                    sprintf(destination , "%s\\staging-area\\%s" , gitche , file_name);
                    char command[4000];
                    sprintf(command , "copy %s %s" , address , destination);
                    system(command);
                    return 1;
                }
                else {
                    fprintf(file , "%s\n" , last_change);
                    fprintf(file , "sd\n");
                    fclose(check);
                    fclose(file);
                    strcpy(added_path[num_last_add] , address);
                    num_last_add++;
                    chdir(gitche);
                    chdir("staging-area");
                    find_parent(address);
                    remove(file_name);
                    chdir(work_dir);
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
                    find_parent(address);
                    char destination[2000];
                    sprintf(destination , "%s\\staging-area\\%s" , gitche , file_name);
                    char command[4000];
                    sprintf(command , "copy %s %s" , address , destination);
                    system(command);
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
        else if(track == 1) {
            char work_dir[2000];
            getcwd(work_dir , sizeof(work_dir));
            chdir(gitche);
            chdir(branch);
            FILE* file = fopen("./status.txt", "r+");
            fseek(file , loc , SEEK_SET);
            fprintf(file , "%s\n" , last_change);
            fprintf(file , "u");
            fclose(file);
            find_parent(address);
            chdir(gitche);
            chdir("staging-area");
            remove(file_name);
            chdir(work_dir);
            return 1;
        }
        else if(track == 0) {
            printf("\033[33mthe file is untracked!\033[0m");
            return 1;
        }
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
    int last_commit = find_branch_head(branch);
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
                int y = find_in_commit(file_path , last_commit);
                if(y == 0) {
                    if(x == 0) printf("\033[36m%-15.15s\033[0m: \033[31m-A\n", file->d_name);
                    if(x == 1) printf("\033[36m%-15.15s\033[0m: \033[32m+A\n", file->d_name);
                }
                else {
                    char commit_name[10];
                    sprintf(commit_name , "%d" , last_commit);
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
                    if(strcmp(last_add , last_change) != 0) {
                        if(x == 0) printf("\033[36m%-15.15s\033[0m: \033[31m-M\n", file->d_name);
                        if(x == 1) printf("\033[36m%-15.15s\033[0m: \033[32m+M\n", file->d_name);
                    }
                }
            }
            chdir(work_dir);
        }
        else if(file->d_type == DT_DIR) {
            if((strcmp(file->d_name , ".gitche") != 0) && (strcmp(file->d_name , ".") != 0) && (strcmp(file->d_name , "..") != 0)) {
                chdir(file->d_name);
                run_status();
                chdir("..");
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
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    int head = find_branch_head(branch);
    char head_s[10];
    sprintf(head_s , "%d" , head);
    chdir(gitche);
    chdir("./staging-area");
    DIR* dir = opendir(".");
    struct dirent* file;
    int num_stage = 0;
    while((file = readdir(dir)) != NULL) {
        if(file->d_type != DT_DIR) num_stage++;
    }
    closedir(dir);
    chdir(gitche);
    chdir(branch);
    FILE* file1 = fopen("./status.txt" , "r");
    char addr[2000];
    char last_a[100];
    char status[5];
    char line[30];
    while(1) {
        fgets(addr , 2000 , file1);
        fgets(last_a , 100 , file1);
        fgets(status , 5 , file1);
        fgets(line , 30 , file1);
        if(feof(file1)) break;
        status[strlen(status)-1] = '\0';
        if(strcmp(status , "sd") == 0) num_stage++;
    }
    fclose(file1);
    if(num_stage == 0) {
        printf("\033[33mthere is nothing to commit!");
        chdir(work_dir);
        return 0;
    }
    else {
        chdir(gitche);
        int last_commit = found_last_commit();
        last_commit++;
        file1 = fopen("./config/last_commit_id.txt", "w");
        fprintf(file1 , "%d" , last_commit);
        fclose(file1);
        chdir(gitche);
        char name[10];
        sprintf(name , "%d" , last_commit);
        mkdir(name);
        chdir(name);
        mkdir("files");
        mkdir("information");
        chdir("information");
        found_username();
        found_email();
        file1 = fopen("./branch.txt", "w");
        fprintf(file1 , "%s" , branch);
        fclose(file1);
        file1 = fopen("./username.txt", "w");
        fprintf(file1 , "%s" , username);
        fclose(file1);
        file1 = fopen("./email.txt", "w");
        fprintf(file1 , "%s" , email);
        fclose(file1);
        file1 = fopen("./massage.txt", "w");
        fprintf(file1 , "%s" , massage);
        fclose(file1);
        file1 = fopen("./number-of-stage.txt", "w");
        fprintf(file1 , "%d" , num_stage);
        fclose(file1);
        file1 = fopen("./time.txt", "w");
        stat("./time.txt" , &attrib);
        char help[100];
        strftime(help , 100 , "%d/%m/%Y %H:%M:%S" , localtime(&attrib.st_ctime));
        fprintf(file1 , "%s" , help);
        fclose(file1);
        chdir(gitche);
        chdir(head_s);
        chdir("files");
        DIR* dir2 = opendir(".");
        while((file = readdir(dir2)) != NULL) {
            if(file->d_type != DT_DIR) {
                char source[2000];
                char destination[2000];
                sprintf(source , "%s\\%s\\files\\%s" , gitche , head_s , file->d_name);
                sprintf(destination , "%s\\%s\\files\\%s" , gitche , name , file->d_name);
                char command[4000];
                sprintf(command , "copy %s %s" , source , destination);
                system(command);
            }
        }
        closedir(dir2);
        chdir(gitche);
        chdir("./staging-area");
        dir = opendir(".");
        struct dirent* file;
        while((file = readdir(dir)) != NULL) {
            if(file->d_type != DT_DIR) {
                char source[2000];
                char destination[2000];
                sprintf(source , "%s\\staging-area\\%s" , gitche , file->d_name);
                sprintf(destination , "%s\\%s\\files\\%s" , gitche , name , file->d_name);
                char command[4000];
                sprintf(command , "move %s %s" , source , destination);
                system(command);
            }
        }
        closedir(dir);
        chdir(gitche);
        chdir(branch);
        FILE* file1 = fopen("./status.txt" , "r");
        while(1) {
            fgets(addr , 2000 , file1);
            addr[strlen(addr)-1] = '\0';
            fgets(last_add , 100 , file1);
            fgets(status , 4 , file1);
            status[strlen(status)-1] = '\0';
            fgets(line , 30 , file1);
            if(feof(file1)) break;
            int track = 0;
            int loc = 0;
            int x = status_specifier(addr , &track , &loc);
            if(x == 1) {
                if(strcmp(status , "se") == 0) {
                    run_reset(addr);
                }
                else {
                    FILE* help = fopen("./status.txt", "r+");
                    fseek(help , loc , SEEK_SET);
                    fprintf(help , "%s\n" , last_change);
                    fprintf(help , "dd");
                    fclose(help);
                    chdir(gitche);
                    chdir(name);
                    chdir("files");
                    find_parent(addr);
                    remove(file_name);
                    chdir(gitche);
                }
            }
        }
        chdir(gitche);
        fclose(file1);
        file1 = fopen("./config/current-commit-id.txt", "w");
        fprintf(file1 , "%d" , last_commit);
        fclose(file1);
        chdir(branch);
        file1 = fopen("./head.txt", "w");
        fprintf(file1 , "%d" , last_commit);
        fclose(file1);
        char help1[2000];
        char help2[2000];
        char command[4000];
        sprintf(help1 , "%s\\%s\\status.txt" , gitche , branch);
        sprintf(help2 , "%s\\%s\\information\\status.txt" , gitche , name);
        sprintf(command , "copy %s %s" , help1 , help2);
        system(command);
        printf("\033[36m      id: \033[35m%d\n", last_commit);
        printf("\033[36m    time: \033[35m%s\n", help);
        printf("\033[36m massage: \033[35m%s\n", massage);
        chdir(work_dir);
        return 1;
    }
}

int find_in_commit(char* address , int comm_num) {
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    chdir(gitche);
    char comm_name[10];
    sprintf(comm_name , "%d" , comm_num);
    chdir(comm_name);
    chdir("files");
    find_parent(address);
    DIR* dir = opendir(".");
    struct dirent* file;
    while((file = readdir(dir)) != NULL) {
        if(file->d_type != DT_DIR) {
            if(strcmp(file->d_name , file_name) == 0) return 1;
        }
    }
    return 0;
}

int find_full_path(char* fi_name) {
    char work_dir[2000];
    getcwd(work_dir , 2000);
    chdir(gitche);
    chdir(branch);
    FILE* file = fopen("./status.txt", "r");
    char addr[2000];
    char la[100];
    char status[5];
    char line[30];
    while(1) {
        fgets(addr , 2000 ,  file);
        fgets(la , 100 ,  file);
        fgets(status , 5 , file);
        fgets(line , 30 , file);
        if(feof(file)) break;
        addr[strlen(addr)-1] = '\0';
        find_parent(addr);
        if(strcmp(fi_name , file_name) == 0) {
            strcpy(real_path , addr);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    chdir(work_dir);
    return 0;
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

int working_tree_clean_check(int* clean) {
    found_gitche();
    char work_dir[2000];
    getcwd(work_dir , sizeof(work_dir));
    int last_commit = find_branch_head(branch);
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
                *clean = 0;
            }
            else {
                int y = find_in_commit(file_path , last_commit);
                if(y == 0) {
                    *clean = 0;
                }
                else {
                    char commit_name[10];
                    sprintf(commit_name , "%d" , last_commit);
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
                    if(strcmp(last_add , last_change) != 0) {
                        return 0;
                    }
                }
            }
            chdir(work_dir);
        }
        else if(file->d_type == DT_DIR) {
            if((strcmp(file->d_name , ".gitche") != 0) && (strcmp(file->d_name , ".") != 0) && (strcmp(file->d_name , "..") != 0)) {
                chdir(file->d_name);
                working_tree_clean_check(clean);
                chdir("..");
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
                *clean = 0;
            }
            fclose(help);
        }
    }
    fclose(for_d);
    chdir(work_dir);
    return 1;
}

int run_checkout(char* comm_name) {
    int clean;
    working_tree_clean_check(&clean);
    if(clean == 0) {
        printf("\033[33mworking tree is not clean!\033[0m");
        return 0;
    }
    else {
        chdir(gitche);
        chdir("..");
        DIR* dir = opendir(".");
        struct dirent* file;
        while((file = readdir(dir)) != NULL) {
            if(file->d_type == DT_DIR) {
                if((strcmp(file->d_name , ".") != 0) && (strcmp(file->d_name , "..") != 0) && (strcmp(file->d_name , ".gitche") != 0)) {
                    char command[2000];
                    sprintf(command , "rmdir /Q /S %s" , file->d_name);
                    system(command);
                }
            }
            else {
                remove(file->d_name);
            }
        }
        closedir(dir);
        chdir(gitche);
        chdir(comm_name);
        chdir("files");
        dir = opendir(".");
        while((file = readdir(dir)) != NULL) {
            if(file->d_type != DT_DIR) {
                FILE* help = fopen("../information/status.txt" , "r");
                char addr[2000];
                char la[100];
                char status[5];
                char line[30];
                while(1) {
                    fgets(addr , 2000 ,  help);
                    fgets(la , 100 ,  help);
                    fgets(status , 5 , help);
                    fgets(line , 30 , help);
                    if(feof(help)) break;
                    addr[strlen(addr)-1] = '\0';
                    find_parent(addr);
                    if(strcmp(file->d_name , file_name) == 0) {
                        strcpy(real_path , addr);
                        break;
                    }
                }
                fclose(help);
                char helpi[2000];
                strcpy(helpi , real_path);
                char* token;
                char folder[100][100];
                int num = 0;
                token = strtok(helpi , "\\");
                strcpy(folder[num] , token);
                num++;
                while(1) {
                    token = strtok(NULL , "\\");
                    if(token == NULL) break;
                    strcpy(folder[num] , token);
                    num++;
                }
                chdir(folder[0]);
                for(int i=1 ; i<num-1 ; i++) {
                    mkdir(folder[i]);
                    chdir(folder[i]);
                }
                char source[2000];
                sprintf(source , "%s\\%s\\files\\%s" , gitche , comm_name , file->d_name);
                char command[2000];
                sprintf(command , "copy %S %s" , source , real_path);
                system(command);
            }
        }
        closedir(dir);
        chdir(gitche);
        FILE* help = fopen("./config/current-commit-id", "w");
        fprintf(help , "%s" , comm_name);
    }
}

int run_tag(char* tag_n , char* tag_massage , int commit_id , int writable) {
    char work_dir[2000];
    getcwd(work_dir , 2000);
    int check_id;
    int check_write;
    int write;
    int x = find_tag_info(tag_n , &check_id , &check_write);
    if(x == 1) {
        if(check_write == 0) printf("\033[33mtag name already exist!\033[0m");
        else {
            chdir(gitche);
            char line[30];
            FILE* file = fopen("./config/tags.txt", "r");
            FILE* help = fopen("./config/temp_tags.txt", "w");
            while(1) {
                fgets(tag_name , 100 , file);
                fscanf(file , "%d\n" , &check_id);
                fgets(tag_username , 100 , file);
                fgets(tag_date , 100 , file);
                fgets(tag_mass , 100 , file);
                fscanf(file , "%d\n" , &write);
                fgets(line , 30 , file);
                if(feof(file)) break;
                fputs(tag_name , help);
                tag_name[strlen(tag_name)-1] = '\0';
                if(strcmp(tag_name , tag_n) == 0) {
                    fprintf(help , "%d\n" , commit_id);
                    found_username();
                    fprintf(help , "%s\n" , username);
                    stat("./config/temp_tags.txt" , &attrib);
                    char helpi[100];
                    strftime(helpi , 100 , "%d/%m/%Y %H:%M:%S" , localtime(&attrib.st_ctime));
                    fprintf(help , "%s\n" , helpi);
                    fprintf(help , "%s\n" , tag_massage);
                    fprintf(help , "%d\n" , writable);
                    fprintf(help , "--------------------\n");
                }
                else {
                    fprintf(help , "%d\n" , check_id);
                    fputs(tag_username , help);
                    fputs(tag_date , help);
                    fputs(tag_mass , help);
                    fprintf(help , "%d\n" , write);
                    fprintf(help , "--------------------\n");
                }
            }
            fclose(file);
            fclose(help);
            file = fopen("./config/tags.txt", "w");
            help = fopen("./config/temp_tags.txt", "r");
            while(1) {
                fgets(tag_name , 100 , help);
                fscanf(help , "%d\n" , &commit_id);
                fgets(tag_username , 100 , help);
                fgets(tag_date , 100 , help);
                fgets(tag_mass , 100 , help);
                fscanf(help , "%d\n" , &write);
                fgets(line , 30 , help);
                if(feof(help)) break;
                fputs(tag_name , file);
                fprintf(file , "%d\n" , commit_id);
                fputs(tag_username , file);
                fputs(tag_date , file);
                fputs(tag_mass , file);
                fprintf(file , "%d\n" , writable);
                fprintf(file , "--------------------\n");
            }
            fclose(file);
            fclose(help);
            remove("./config/temp_tags.txt");
            chdir(work_dir);
        }
    }
    if(x == 0) {
        chdir(gitche);
        FILE* file = fopen("./config/tag-names.txt", "a");
        fprintf(file , "%s\n" , tag_n);
        fclose(file);
        file = fopen("./config/tags.txt", "a");
        fprintf(file , "%s\n" , tag_n);
        fprintf(file , "%d\n" , commit_id);
        found_username();
        fprintf(file , "%s\n" , username);
        FILE* help = fopen("./config/for-time.txt", "w");
        stat("./config/for-time.txt" , &attrib);
        char helpi[100];
        strftime(helpi , 100 , "%d/%m/%Y %H:%M:%S" , localtime(&attrib.st_ctime));
        fprintf(file , "%s\n" , helpi);
        fclose(help);
        remove("./config/for-time.txt");
        fprintf(file , "%s\n" , tag_massage);
        fprintf(file , "%d\n" , writable);
        fprintf(file , "--------------------\n");
        fclose(file);
        chdir(work_dir);
    }
    return 0;
}

int find_tag_info(char* tag , int* a , int* b) {
    char work_dir[2000];
    getcwd(work_dir , 2000);
    chdir(gitche);
    FILE* file = fopen("./config/tags.txt", "r");
    int commit_id;
    int writable;
    char line[30];
    int ok = 0;
    while(1) {
        fgets(tag_name , 100 , file);
        fscanf(file , "%d\n" , &commit_id);
        fgets(tag_username , 100 , file);
        fgets(tag_date , 100 , file);
        fgets(tag_mass , 100 , file);
        fscanf(file , "%d\n" , &writable);
        fgets(line , 30 , file);
        if(feof(file)) break;
        tag_name[strlen(tag_name)-1] = '\0';
        tag_username[strlen(tag_username)-1] = '\0';
        tag_date[strlen(tag_date)-1] = '\0';
        tag_mass[strlen(tag_mass)-1] = '\0';
        if(strcmp(tag_name , tag) == 0) {
            ok = 1;
            break;
        }
    }
    fclose(file);
    chdir(work_dir);
    if(ok == 0) return 0;
    if(ok == 1) {
        *a = commit_id;
        *b = writable;
        return 1;
    }
}

void left_shift(char* a , int first , int size , int bit) {
    for(int i=first ; i<=size ; i++) {
        a[i - bit] = a[i];
    }
}

void sort(char* a) {
        for(int i = 0 ; i<strlen(a) ; i++) {
            if(a[i] == '\t') a[i] = ' ';
            if(a[i] == '\n') a[i] = ' ';
        }
        int pointer = 0;
        while(pointer < strlen(a)) {
            if((a[pointer] == ' ') && (pointer == 0)) {
                left_shift(a , pointer+1 , strlen(a) , 1);
            }
            else if((a[pointer] == ' ') && (a[pointer + 1] == ' ')) {
                left_shift(a , pointer+1 , strlen(a) , 1);
            }
            else if((a[pointer] == ' ') && (pointer == strlen(a)-1)) {
                left_shift(a , strlen(a) , strlen(a) , 1);
            }
            else pointer ++;
        }
}

int file_line(char* address) {
    find_path(address);
    int r = 0;
    char line[1000];
    FILE* file = fopen(real_path , "r");
    while(feof(file) == 0) {
        fgets(line , 1000 , file);
        r++;
    }
    return r;
}

int file_compare(char* r_address1 , char* r_address2 , int start1 , int finish1 , int start2 , int finish2) {
    find_path(r_address1);
    char address1[2000];
    strcpy(address1 , real_path);
    find_path(r_address2);
    char address2[2000];
    strcpy(address2 , real_path);
    FILE* file1 = fopen(address1 , "r");
    FILE* help1 = fopen("./help1.txt" , "w");
    int line = 1;
    char line_c[1000];
    while(feof(file1) == 0) {
        fgets(line_c , 1000 , file1);
        if((line >= start1) && (line <= finish1)) fprintf(help1 , "%s" , line_c);
        line++;
        strcpy(line_c , "");
    }
    fclose(file1);
    fclose(help1);
    FILE* file2 = fopen(address2 , "r");
    FILE* help2 = fopen("./help2.txt" , "w");
    line = 1;
    while(feof(file2) == 0) {
        fgets(line_c , 1000 , file2);
        if((line >= start2) && (line <= finish2)) fprintf(help2 , "%s" , line_c);
        line++;
        strcpy(line_c , "");
    }
    fclose(file2);
    fclose(help2);
    help1 = fopen("./help1.txt" , "r");
    help2 = fopen("./help2.txt" , "r");
    char line1[1000] = "";
    char line2[1000] = "";
    char save1[1000] = "";
    char save2[1000] = "";
    int l1 = 0 , l2 = 0;
    while((feof(help1) == 0) && (feof(help2) == 0)) {
        fgets(line1 , 1000 , help1);
        if(feof(help1) && (line1[strlen(line1)-1] == '\n')) strcpy(line1 , "");
        strcpy(save1 , line1);
        sort(line1);
        if(strcmp(line1 , "") == 0) {
            l1++;
            continue;
        }
        else {
            while(feof(help2) == 0) {
                fgets(line2 , 1000 , help2);
                if(feof(help2) && (line2[strlen(line2)-1] == '\n')) strcpy(line2 , "");
                strcpy(save2 , line2);
                sort(line2);
                if(strcmp(line2 , "") != 0) {
                    if(strcmp(line1 , line2) != 0) {
                        if(save1[strlen(save1)-1] == '\n') save1[strlen(save1)-1] = '\0';
                        if(save2[strlen(save2)-1] == '\n') save2[strlen(save2)-1] = '\0';
                        printf("-------------------------\n");
                        printf("\033[34m%s\033[0m - \033[34m%d\n\033[0m", r_address1 , l1 + start1);
                        printf("\033[34m%s\033[0m\n", save1);
                        printf("\n");
                        printf("\033[31m%s\033[0m - \033[31m%d\n\033[0m", r_address2 , l2 + start2);
                        printf("\033[31m%s\033[0m\n", save2);
                        printf("-------------------------\n");
                    }
                    l2++;
                    break;
                }
                else l2++;
            }
            l1++;
        }
    }
    fclose(help1);
    fclose(help2);
    remove("./help1.txt");
    remove("./help2.txt");
    return 1;
}

int run_grep(char* file_p , char* word , char* comm_name , int line_show) {
    char work_dir[2000];
    getcwd(work_dir , 2000);
    chdir(gitche);
    chdir(comm_name);
    chdir("files");
    find_path(file_p);
    find_parent(real_path);
    char line[1000] = "";
    int l = 1;
    FILE* file = fopen(file_name , "r");
    while(feof(file) == 0) {
        fgets(line , 1000 , file);
        if(feof(file) && (line[strlen(line)-1] == '\n')) strcpy(line , "");
        if(strstr(line , word) != NULL) {
            if(line_show == 0) {
                printf("\033[36m%s\n\033[0m", line);
                printf("--------------------\n");
            }
            else if(line_show == 1) {
                printf("line: \033[35m%d\033[0m\n", l);
                printf("\033[36m%s\n\033[0m", line);
                printf("--------------------\n");
            }
        }
        l++;
    }
    fclose(file);
    chdir(work_dir);
}


int main(int argc , char* argv[]) {
    if(argc == 1) {
        printf("\033[33mplease enter a valid command!\033[0m");
        return 1;
    }
    else if(strcmp(argv[1] , "init") == 0) {
        run_init();
        return 0;
    }
    int x = found_gitche();
    if(x == 0) {
        printf("\033[31mYou have to initialize first!\033[0m");
        return 1;
    }
    found_branch();
    if(strcmp(argv[1] , "config") == 0) {
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        else if((strncmp(argv[2] , "alias." , 6) == 0) || (strncmp(argv[3] , "alias." , 6) == 0)) {
            int n = 3;
            if(strcmp(argv[2] , "-global") == 0) n = 4;
            int ok = 0;
            if(strncmp(argv[n] , "gitche config -global user.name" , 31) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche config -global user.email" , 32) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche config user.name" , 23) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche config user.email" , 24) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche config -global alias." , 28) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche config -global alias." , 28) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche init" , 11) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche add" , 10) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche reset" , 12) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche status" , 13) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche commit" , 13) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche set" , 10) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche replace" , 14) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche remove" , 13) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche log" , 10) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche branch" , 13) == 0) ok = 1;
            else if(strncmp(argv[n] , "gitche checkout" , 15) == 0) ok = 1;
            else if(ok == 0) {
                printf("\033[33mthe command is not valid!\033[0m");
                return 1;
            }
            run_alias(argc , argv);
            return 0;
        }
        run_config(argc , argv);
        return 0;
    }
    else if(strcmp(argv[1] , "add") == 0) {
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        else if(argc < 3) {
            printf("\033[33mplease enter a valid command!\033[0m");
            return 1;
        }
        else if((argc == 3)) {
            if((strcmp(argv[2] , "-f") == 0) || (strcmp(argv[2] , "-n") == 0)) {
                printf("\033[33mplease enter a valid command!\033[0m");
                return 1;
            }
            else {
                for(int i=0 ; i<strlen(argv[2]) ; i++) {
                    if(argv[2][i] == '*') {
                        printf("\033[33mthere is no word with this condition!\033[0m");
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
                printf("\033[31mthis command is not supported!\033[0m");
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
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        else if(argc < 3) {
            printf("\033[33mplease wnter a valid command!\033[0m");
            return 1;
        }
        else if((argc == 3)) {
            if((strcmp(argv[2] , "-f") == 0)) {
                printf("\033[33mplease enter a valid command!\033[0m");
                return 1;
            }
            else if(strcmp(argv[2] , "-undo") == 0) {
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
                for(int i=0 ; i<strlen(argv[2]) ; i++) {
                    if(argv[2][i] == '*') {
                        printf("\033[33mthere is no word with this condition!\033[0m");
                        return 1;
                    }
                }
                run_reset(argv[2]);
                return 0;
            }
        }
        else if(strcmp(argv[2] , "-f") == 0) {
            for(int i=3 ; i<argc ; i++) {
                run_reset(argv[i]);
            }
            return 0;
        }
        else {
            for(int i=2 ; i<argc ; i++) {
                run_reset(argv[i]);
            }
            return 0;
        }
    }
    else if(strcmp(argv[1] , "status") == 0) {
        char work_dir[2000];
        getcwd(work_dir , 2000);
        chdir(gitche);
        chdir("..");
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
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        found_branch();
        char work_dir[2000];
        getcwd(work_dir , 2000);
        chdir(gitche);
        if(argc == 3) {
            FILE* file = fopen("./config/branch-names.txt", "r");
            char name[100];
            int exist = 0;
            while(1) {
                fgets(name , 100 , file);
                if(feof(file)) break;
                name[strlen(name)-1] = '\0';
                if(strcmp(name , argv[2]) == 0) {
                    exist = 1;
                    break;
                }
            }
            fclose(file);
            if(exist == 1) printf("\033[31m%s \033[0mhave already exist!\033[0m");
            else create_branch(argv[2]);
        }
        else if(argc == 2) {
            FILE* file = fopen("./config/branch-names.txt", "r");
            char name[100];
            int exist = 0;
            while(1) {
                fgets(name , 100 , file);
                if(feof(file)) break;
                name[strlen(name)-1] = '\0';
                if(strcmp(name , branch) == 0) printf("\033[36m*%s\033[0m\n", name);
                else printf("\033[35m%s\033[0m\n", name);
            }
            fclose(file);            
        }
        else printf("\033[33mplease enter a valid command!\033[0m");
        chdir(work_dir);
        return 0;
    }
    else if(strcmp(argv[1] , "checkout") == 0) {
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        printf("\033[31mthis command is not supported!\033[0m");
        return 1;
    }
    else if(strcmp(argv[1] , "revert") == 0) {
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        printf("\033[31mthis command is not supported!\033[0m");
        return 1;
    }
    else if(strcmp(argv[1] , "tag") == 0) {
        char work_dir[2000];
        getcwd(work_dir , 2000);
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        else {
            if(argc == 2) {
                char tag[100][100];
                int num_tag = 0;
                chdir(gitche);
                FILE* file = fopen("./config/tag-names.txt", "r");
                if(file == NULL) printf("\033[33mthere are no tags!\033[0m");
                else {
                    char tag_namee[100];
                    while(1) {
                        fgets(tag_namee , 100 , file);
                        if(feof(file)) break;
                        tag_namee[strlen(tag_namee)-1] = '\0';
                        strcpy(tag[num_tag] , tag_namee);
                        num_tag++;
                    }
                    for(int i=0 ; i<num_tag-1 ; i++) {
                        for(int j=i ; j<num_tag ; j++) {
                            if(strcmp(tag[i] , tag[j]) > 0) {
                                char temp[100];
                                strcpy(temp , tag[i]);
                                strcpy(tag[i] , tag[j]);
                                strcpy(tag[j] , temp);
                            }
                        }
                    }
                    for(int i=0 ; i<num_tag ; i++) {
                       printf("\033[36m%s\033[0m\n", tag[i]);
                    }
                }
                fclose(file);
                chdir(work_dir);
            }
            else if(argc == 4) {
                if(strcmp(argv[2] , "-a") == 0) {
                    int last_commit_id = found_last_commit();
                    run_tag(argv[3] , "" , last_commit_id , 0);
                }
                else if(strcmp(argv[2] , "show") == 0) {
                    int comm_id;
                    int write;
                    int x = find_tag_info(argv[3] , &comm_id , &write);
                    if(x == 1) {
                        printf("     \033[35mtag: \033[36m%s\n", tag_name);
                        printf("  \033[35mcommit: \033[36m%d\n", comm_id);
                        printf("  \033[35mAuthor: \033[36m%s\n", tag_username);
                        printf("    \033[35mDate: \033[36m%s\n", tag_date);
                        printf(" \033[35mMassage: \033[36m%s\n", tag_mass);
                    }
                    else printf("\033[33mtag does not exist!\033[0m");
                }
                else printf("\033[33mplease enter a valid command!\033[0m");
                return 0;
            }
            else if(argc == 5) {
                if((strcmp(argv[2] , "-a") == 0) && (strcmp(argv[4] , "-f") == 0)) {
                    int last_commit_id = found_last_commit();
                    run_tag(argv[3] , "" , last_commit_id , 1);
                }
                else printf("\033[33mplease enter a valid command!\033[0m");
            }
            else if(argc == 6) {
                if((strcmp(argv[2] , "-a") == 0) && (strcmp(argv[4] , "-m") == 0)) {
                    int last_commit_id = found_last_commit();
                    run_tag(argv[3] , argv[5] , last_commit_id , 0);
                }
                else if((strcmp(argv[2] , "-a") == 0) && (strcmp(argv[4] , "-c") == 0)) {
                    int comm_id;
                    sscanf(argv[5] , "%d" , &comm_id);
                    run_tag(argv[3] , "" , comm_id , 0);
                }
                else printf("\033[33mplease enter a valid command!\033[0m");
            }
            else if(argc == 7) {
                if((strcmp(argv[2] , "-a") == 0) && (strcmp(argv[4] , "-m") == 0) && (strcmp(argv[6] , "-f") == 0)) {
                    int last_commit_id = found_last_commit();
                    run_tag(argv[3] , argv[5] , last_commit_id , 1);
                }
                else if((strcmp(argv[2] , "-a") == 0) && (strcmp(argv[4] , "-c") == 0) && (strcmp(argv[6] , "-f") == 0)) {
                    int comm_id;
                    sscanf(argv[5] , "%d" , &comm_id);
                    run_tag(argv[3] , "" , comm_id , 1);
                }
                else printf("\033[33mplease enter a valid command!\033[0m");
            }
            else if(argc == 8) {
                if((strcmp(argv[2] , "-a") == 0) && (strcmp(argv[4] , "-m") == 0) && (strcmp(argv[6] , "-c") == 0)) {
                    int comm_id;
                    sscanf(argv[7] , "%d" , &comm_id);
                    run_tag(argv[3] , argv[5] , comm_id , 0); 
                }
                else printf("\033[33mplease enter a valid command!\033[0m");
            }
            else if(argc == 9) {
                if((strcmp(argv[2] , "-a") == 0) && (strcmp(argv[4] , "-m") == 0) && (strcmp(argv[6] , "-c") == 0) && (strcmp(argv[8] , "-f") == 0)) {
                    int comm_id;
                    sscanf(argv[7] , "%d" , &comm_id);
                    run_tag(argv[3] , argv[5] , comm_id , 1);                    
                }
                else printf("\033[33mplease enter a valid command!\033[0m");
            }
            else printf("\033[33mplease enter a valid command!\033[0m");
            return 0;
        }
    }
    else if(strcmp(argv[1] , "tree") == 0) {
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        printf("\033[31mthis command is not supported!\033[0m");
        return 1;
    }
    else if(strcmp(argv[1] , "stash") == 0) {
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        printf("\033[31mthis command is not supported!\033[0m");
        return 1;
    }
    else if(strcmp(argv[1] , "pre-commit") == 0) {
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        printf("\033[31mthis command is not supported!\033[0m");
        return 1;
    }
    else if(strcmp(argv[1] , "grep") == 0) {
        if(argc == 6) {
            if((strcmp(argv[2] , "-f") == 0) && (strcmp(argv[4] , "-p") == 0)) {
                int head = find_branch_head(branch);
                char comm_name[10];
                sprintf(comm_name , "%d" , head);
                run_grep(argv[3] , argv[5] , comm_name , 0);
            }
            else printf("\033[33mplease enter a valid command!\033[0m");
        }
        else if(argc == 7) {
            if((strcmp(argv[2] , "-f") == 0) && (strcmp(argv[4] , "-p") == 0) && (strcmp(argv[6] , "-n") == 0)) {
                int head = find_branch_head(branch);
                char comm_name[10];
                sprintf(comm_name , "%d" , head);
                run_grep(argv[3] , argv[5] , comm_name , 1);
            }
            else printf("\033[33mplease enter a valid command!\033[0m");           
        }
        else if(argc == 8) {
            if((strcmp(argv[2] , "-f") == 0) && (strcmp(argv[4] , "-p") == 0) && (strcmp(argv[6] , "-c") == 0)) {
                run_grep(argv[3] , argv[5] , argv[7] , 0);
            }
            else printf("\033[33mplease enter a valid command!\033[0m");
        }
        else if(argc == 9) {
            if((strcmp(argv[2] , "-f") == 0) && (strcmp(argv[4] , "-p") == 0) && (strcmp(argv[6] , "-c") == 0) && (strcmp(argv[8] , "-n") == 0)) {
                run_grep(argv[3] , argv[5] , argv[7] , 1);
            }
            else printf("\033[33mplease enter a valid command!\033[0m");
        }
        else printf("\033[33mplease enter a valid command!\033[0m");
    }
    else if(strcmp(argv[1] , "diff") == 0) {
        if(argc == 5) {
            if(strcmp(argv[2] , "-f") == 0) {
                file_compare(argv[3] , argv[4] , 1 , file_line(argv[3]) , 1 , file_line(argv[4]));
            }
            else if(strcmp(argv[2] , "-c") == 0) {
                char source1[2000];
                char source2[2000];
                sprintf(source1 , "%s\\%s\\files" , gitche , argv[3]);
                sprintf(source2 , "%s\\%s\\files" , gitche , argv[4]);
                DIR* dir1;
                DIR* dir2;
                struct dirent* file1;
                struct dirent* file2;
                char file_path1[2000];
                char file_path2[2000];
                char file_real1[2000];
                char file_real2[2000];
                dir1 = opendir(source1);
                while((file1 = readdir(dir1)) != NULL) {
                    if(file1->d_type != DT_DIR) {
                        sprintf(file_path1 , "%s\\%s\\files\\%s", gitche , argv[3] , file1->d_name);
                        find_full_path(file1->d_name);
                        strcpy(file_real1 , real_path);
                        printf("%S\n", file_real1);
                        int exist = 0;
                        dir2 = opendir(source2);
                        while((file2 = readdir(dir2)) != NULL) {
                            if(file2->d_type != DT_DIR) {
                                if(strcmp(file1->d_name , file2->d_name) == 0) {
                                    exist = 1;
                                    sprintf(file_path2 , "%s\\%s\\files\\%s", gitche , argv[4] , file2->d_name);
                                    find_full_path(file2->d_name);
                                    strcpy(file_real2 , real_path);
                                    break;
                                }
                            }
                        }
                        closedir(dir2);
                        if(exist == 0) {
                            printf("\033[35m%s\033[0m exist in commit \033[36m%s\033[0m but not in commit \033[36m%s\033[0m\n", file_real1 , argv[3] , argv[4]);
                        }
                        else {
                            file_compare(file_path1 , file_path2 , 1 , file_line(file_path1) , 1 , file_line(file_path2));
                        }
                    }
                }
                closedir(dir1);
                dir2 = opendir(source2);
                while((file2 = readdir(dir2)) != NULL) {
                    if(file2->d_type != DT_DIR) {
                        sprintf(file_path2 , "%s\\%s\\files\\%s", gitche , argv[4] , file2->d_name);
                        find_full_path(file2->d_name);
                        strcpy(file_real2 , real_path);
                        int exist = 0;
                        dir1 = opendir(source1);
                        while((file1 = readdir(dir1)) != NULL) {
                            if(file1->d_type != DT_DIR) {
                                if(strcmp(file1->d_name , file2->d_name) == 0) {
                                    exist = 1;
                                    sprintf(file_path1 , "%s\\%s\\files\\%s", gitche , argv[3] , file1->d_name);
                                    find_full_path(file1->d_name);
                                    strcpy(file_real1 , real_path);
                                    break;
                                }
                            }
                        }
                        closedir(dir1);
                        if(exist == 0) {
                            printf("\033[35m%s\033[0m exist in commit \033[36m%s\033[0m but not in commit \033[36m%s\033[0m\n", file_real2 , argv[4] , argv[3]);
                        }
                    }
                }
                closedir(dir2);
            }
            else printf("\033[33mplease enter a valid command!\033[0m");
        }
        else if(argc == 7) {
            if(strcmp(argv[2] , "-f") == 0) {
                if(strcmp(argv[5] , "-line1") == 0) {
                    int s1 , f1;
                    sscanf(argv[6] , "%d-%d" , &s1 , &f1);
                    file_compare(argv[3] , argv[4] , s1 , f1 , 1 , file_line(argv[4]));
                }
                else if(strcmp(argv[5] , "-line2") == 0) {
                    int s2 , f2;
                    sscanf(argv[6] , "%d-%d" , &s2 , &f2);
                    file_compare(argv[3] , argv[4] , 1 , file_line(argv[3]) , s2 , f2);
                }
                else printf("\033[33mplease enter a valid command!\033[0m");
            }
            else printf("\033[33mplease enter a valid command!\033[0m");
        }
        else if(argc == 9) {
            if((strcmp(argv[2] , "-f") == 0) && (strcmp(argv[5] , "-line1") == 0) && (strcmp(argv[7] , "-line2") == 0)) {
                int s1 , s2 , f1 , f2;
                sscanf(argv[6] , "%d-%d" , &s1 , &f1);
                sscanf(argv[8] , "%d-%d" , &s2 , &f2);
                file_compare(argv[3] , argv[4] , s1 , f1 , s2 , f2);
            }
            else printf("\033[33mplease enter a valid command!\033[0m");
        }
        else printf("\033[33mplease enter a valid command!\033[0m");
        return 0;
    }
    else if(strcmp(argv[1] , "merge") == 0) {
        int x = found_gitche();
        if(x == 0) {
            printf("\033[31mYou have to initialize first!\033[0m");
            return 1;
        }
        printf("\033[31mthis command is not supported!\033[0m");
        return 1;
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