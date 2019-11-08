/*
 * @author  George Balaouras
 * @version 1.0 {final}
 * @since   22-01-2019
 *
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SPACE_DELIM  " \t\n\r"
#define SEMI_AMP_DELIM ";&&"
#define GREATER_LESS_DELIM "><"

#define MAX_INSTRUCT 250
#define MAX_LENGTH 514

void loop (int argc, char* argv[]);

void Parse_Semi_Amper(char* line, char** ptr_arg, char *delim_used, int *count);
void Parse_Greater_LESS_Than(char* line, char GR_LS[MAX_INSTRUCT][MAX_LENGTH], char *delim_used, int* count);
char** split_line (char* line);

int execute (char** args, int *error);
int execute_GR_LS(char instruct[MAX_INSTRUCT][MAX_LENGTH], char *delim_used, int count, int *error);
int BuiltIn(char** args, int *flag_builtin);

int check(char* line);

int interactive = 1; // Global variable

int main (int argc, char* argv[]) {
    
    loop(argc, argv);
    
    return 0;
}

void loop(int argc, char* argv[]) {
    FILE *file;
    ssize_t buf_size;
    
    char** ptr_instruct = malloc(MAX_INSTRUCT * sizeof(char*));
    char GR_LS[MAX_INSTRUCT][MAX_LENGTH];
    char** ptr_arg = malloc(MAX_INSTRUCT * sizeof(char*));
    
    char* line; 
    char *delim_used = (char*) malloc( (MAX_LENGTH-1)* sizeof(char));
    char *delim_used_redirect = (char*) malloc( (MAX_LENGTH-1) * sizeof(char));
    
    int pos = 0, status, i, j,exit_status = 1, count_in = 0, flag_builtin = 0, count_SA = 1, count_GR_LS = 1;
    int error = 0;    
    file = stdin;
    if (argc == 2){
        file = fopen(argv[1],"r");   
        if (file == NULL){
            printf("Error at the handling of batchfile.\n");
            exit(1);
        }
        interactive = 0;
    }else if (argc > 2){
        printf("Too many input items.\n");
        exit(1);
    }
    do {
        do{
            if (interactive) printf("Balaouras_8861> ");
            getline(&line, &buf_size, file);
                
            if (feof(file)){
                printf("Exiting...\n");
                printf("Bye bye!\n");
                exit(1);
            }
               
        }while(!strcmp(line,"\n") );
        
        error = 0;                                                                              // For every new line, error is 0
        
        if ( check (line) ){
            Parse_Semi_Amper(line, ptr_arg, delim_used, &count_SA);
        
            for(i = count_SA-1; i >-1 ; i--){
                delim_used[i+1] = delim_used[i];
            }
            delim_used[0] = ' ';
            
            for (i = 0; i < count_SA; i++){
                flag_builtin = 0;                                                                                       // Make sure that the current instruction it isn't Build In.
                if (!check(ptr_arg[i])) break;                                                               // Double check for NULL command
                if ( delim_used[i] == ';'){                                                                          // ; was found
                    Parse_Greater_LESS_Than(ptr_arg[i], GR_LS, delim_used_redirect, &count_GR_LS);
                    if( count_GR_LS != 1) {                                                                // > Found!!
                        status = execute_GR_LS(GR_LS, delim_used_redirect, count_GR_LS, &error);        
                    }else{                                                                                             //No > found
                        ptr_instruct = split_line(ptr_arg[i]); 
                        exit_status = BuiltIn(ptr_instruct, &flag_builtin);
                        if (!exit_status) exit(1);
                        if (!flag_builtin) status = execute (ptr_instruct, &error);
                    }
                }else if (delim_used[i] == '&' && !error){                                                // && was found AND status is good to good
                    Parse_Greater_LESS_Than(ptr_arg[i], GR_LS, delim_used_redirect, &count_GR_LS);
                    if( count_GR_LS != 1) {                                                                // > Found!!
                        status = execute_GR_LS(GR_LS, delim_used_redirect, count_GR_LS, &error);        
                    }else{
                        ptr_instruct = split_line(ptr_arg[i]);                                                  //No > found
                        exit_status = BuiltIn(ptr_instruct, &flag_builtin);
                        if (!exit_status) exit(1);
                        if (!flag_builtin) status = execute (ptr_instruct, &error);
                    }
                }else if ( delim_used[i] != '&' && delim_used[i] != ';') {                           // One instruct per line
                    Parse_Greater_LESS_Than(ptr_arg[i], GR_LS, delim_used_redirect, &count_GR_LS);
                    if( count_GR_LS != 1) {                                                  // > Found!!
                        status = execute_GR_LS(GR_LS, delim_used_redirect, count_GR_LS, &error);        
                    }else{
                        ptr_instruct = split_line(ptr_arg[i]);                                // No > found
                        exit_status = BuiltIn(ptr_instruct, &flag_builtin);
                        if (!exit_status) exit(1);
                        if (!flag_builtin) status = execute (ptr_instruct, &error);
                    }
                }
            }
         
        }
        
    }while(exit_status); //!status
}

void Parse_Semi_Amper(char* line, char** ptr_arg, char *delim_used, int *count){
    int pos = 0, i;
    if (!ptr_arg){
        fprintf(stderr, "Something went wrong. Not enough memory. Abort!\n");
        exit( EXIT_FAILURE );
    }
    char *arg_token, *duplicate;
    duplicate = strdup(line);
    arg_token = strtok(line, SEMI_AMP_DELIM);
    while (arg_token!= NULL) {
        ptr_arg[pos] = arg_token;
        delim_used[pos] = duplicate[arg_token - line + strlen(arg_token)];
        pos++;
        arg_token = strtok(NULL, SEMI_AMP_DELIM);
    }
    ptr_arg[pos] = (char*)"\0";
    for (i = 0; i<pos; i++){
    strcat(ptr_arg[i], ptr_arg[pos]);
    }
    *count = pos;
}

void Parse_Greater_LESS_Than(char* line, char GR_LS[MAX_INSTRUCT][MAX_LENGTH], char *delim_used, int* count){
    int pos = 0;
    
    if (!GR_LS){
        fprintf(stderr, "Something went wrong. Not enough memory. Abort!\n");
        exit( EXIT_FAILURE );
    }
    char *arg_token, *duplicate;
    duplicate = strdup(line);
    arg_token = strtok(line, GREATER_LESS_DELIM);
    while (arg_token!= NULL) {
        memcpy(GR_LS[pos], arg_token, strlen(arg_token)+1);
        delim_used[pos] = duplicate[arg_token - line + strlen(arg_token)];
        pos++;
        arg_token = strtok(NULL, GREATER_LESS_DELIM);
    }
   *count = pos;
}

char** split_line(char*  line){
    int pos = 0;
    char** args = (char**)malloc(MAX_LENGTH * sizeof(char*));
    
    if (!args){
        fprintf(stderr, "Something went wrong. Not enough memory. Abort!\n");
        exit( EXIT_FAILURE );
    }
    char* token;    
    token = strtok(line, SPACE_DELIM);
    while (token != NULL) {
        args[pos] = token;
        pos++;
        token = strtok(NULL, SPACE_DELIM);
    }
    args[pos] = NULL;
    
    return args;
}

int execute(char** args, int *error){
    pid_t pid, wait_pid;
    int status;
    pid = fork( );
    if (pid == 0){    //Chlid Process
        execvp( args[0], &args[0]);
        // Only if execvp, return error the following will be executed
        printf("Ooops, something went wrong. My fault. \n");
        _exit( EXIT_FAILURE );
        
    } else if (pid > 0) {
      do{
            wait_pid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status)); 
       if (status) *error = 1;
    }
    return status;
}

int execute_GR_LS(char instruct[MAX_INSTRUCT][MAX_LENGTH], char *delim_used, int count, int *error){
    pid_t pid, wait_pid;
    int status, pos = 0, file_no_gr, file_no_ls, i, flag_gr = 0, flag_ls = 0, exit_status = 0;
    char *name_file;
    char** ptr_instruct;
    char *mode;
    FILE *file_gr, *file_ls;
    
    for (i = 0; i < count-1; i++) {
        if (delim_used[i] == '>') {
            flag_gr = flag_gr + 1;
            mode = "w+";
            file_no_gr = STDOUT_FILENO;
            name_file = strtok(instruct[i+1], SPACE_DELIM);
            file_gr = fopen(name_file, mode);
            if (file_gr == NULL){
                printf("Error at the handling of the file.\n");
                exit_status = 1;
            }
        }else if (delim_used[i] == '<') {
            flag_ls = flag_ls + 1;
            mode = "r";
            file_no_ls = STDIN_FILENO;
            name_file = strtok(instruct[i+1], SPACE_DELIM);
            file_ls = fopen(name_file, mode);
            if (file_ls == NULL){
                printf("Error at the handling of the file.\n");
                exit_status = 1;
            }
        }else{
            exit_status = 1;
        }
        
    }
    pid = fork( );
    if (pid == 0){    // Chlid Process
        if(exit_status) _exit(EXIT_FAILURE);
        if (flag_gr) {
            dup2(fileno(file_gr),file_no_gr); // Redirect stdout to file
            fclose(file_gr);
        }
        if (flag_ls) {
            dup2(fileno(file_ls),file_no_ls); // Redirect stdin to file
            fclose(file_ls);
        }
        
        ptr_instruct = split_line(instruct[0]); 
        execvp(ptr_instruct[0], &ptr_instruct[0]);
        // Only if execvp, return error the following will be executed
        _exit( EXIT_FAILURE );
    }
    else if (pid > 0) {
       do{
            wait_pid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status)); 
        if(status){
            *error = 1;
            printf("Ooops, something went wrong. My fault.\n");
        }
    }
    
    return status;    
}

int BuiltIn(char** args, int *flag_builtin){
    int exit_status = 1, result = 0;
    if(strcmp(args[0], "quit") == 0){
        printf("Exiting...\n");
        printf("Bye bye!\n");
        exit_status = 0;
        *flag_builtin = 1;
        return exit_status;;
    }else if (strcmp(args[0], "help") == 0){
        printf("                            Georgios Balaouras, AEM:8861\n");
        printf("cd,help and quit are handled as Built-In functions. \n");
        printf("For other functions type @function_name @arguments. If you need more info, type man @function_name. \n");
        printf("~~ Improper space handle.\n");
        printf("~~ Redirection with > handle.\n");
        printf("~~ Redirection with < handle.\n");
        printf("~~ Two modes:\n");
        printf("        i)  Interactive.\n");
        printf("        ii) Batch.\n");
        *flag_builtin = 1;
        return exit_status;
    }else if (strcmp(args[0], "cd") == 0){
        *flag_builtin = 1;
        result = chdir(args[1]);
        if (result < 0) {
            printf("Error executing cd.\n");
        }else {
            printf("The current directory is: ");
            fflush(stdout);
            system( " echo $PWD");
        }
        return exit_status;
    }
    return exit_status;
}

int check(char* line){
    int check_status = 1, i, j;
    char *ret, *token;
    char space_pat[5] = " \n";
    if (strlen(line) > 512){
        check_status = 0;
        printf("Bad syntax. Too many characters was given!\n");
        return check_status;
       
    }
    char* backup = strdup(line);
    token = strtok(backup, " \t");
    if (token[0] == '&' || token[0] == ';' || token[0] == ' '){
        check_status = 0;
        printf("Bad syntax. Unexpected first character\n");
        return check_status;
    }
        
    backup = strdup(line);
    if (!strcmp(strtok(backup," \t"), "\n") || !strcmp(strtok(backup," \t"), ";\n") || !strcmp(strtok(backup," \t"), "&\n") || !strcmp(strtok(backup," \t"), "&&\n") ){
        check_status = 0;
        printf("Bad syntax. NULL command was given!\n");
        return check_status;
    }
    
    char semi_pat[5] = ";;" ;
    ret = strstr(line,semi_pat);
    if (ret != NULL){
        check_status = 0;
        printf("Bad syntax. Two or more (sequentially) ; has been found!\n");
        return check_status;
    }
    char amper_pat[5] = "&&&";
    ret = strstr(line,amper_pat);
    if (ret != NULL){
        check_status = 0;
        printf("Bad syntax. Three or more (sequentially) & has been found!\n");
        return check_status;
    }
    for ( i = 0; i < strlen(line)-1; i++){
        if (line[i] == ';'){
            ret = strstr(line,space_pat);
            if (ret != NULL){
                check_status = 0;
                printf("Bad syntax. NULL command was given!\n");
                return check_status;
            }
            for ( j = i+1; j < strlen(line)-1; j++){
                if(line[j] != ' ' && line[j] != '\t'){
                    if (line[j] == ';' || line[j] == '&'){
                        check_status = 0;
                        printf("Bad syntax. Unexpected token after ; !\n");
                        return check_status;
                    }else {
                        break;
                    }
                }
            }
        }
    }
    
    for ( i = 0; i < strlen(line)-1; i++){
        if (line[i] == '&' && line[i+1] != '&'){
            ret = strstr(line,space_pat);
            if (ret != NULL){
                check_status = 0;
                printf("Bad syntax. NULL command was given!\n");
                return check_status;
            }
            for ( j = i+1; j < strlen(line)-1; j++){
                if(line[j] != ' ' && line[j] != '\t'){
                    if (line[j] == ';' || line[j] == '&'){
                        check_status = 0;
                        printf("Bad syntax. Unexpected token after && !\n");
                        return check_status;
                    }else {
                        break;
                    }
                }
            }
        }
    }
    
    return check_status;
}

