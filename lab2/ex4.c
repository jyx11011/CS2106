/*************************************
 * Lab 2 Exercise 4
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()
char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer);
void freeTokenArray(char** strArr, int size);

char* getCommand();
int checkCommandPath(char* commandPath);
int execute(char** tokens, char* commandPath, int tokenNum);
char** copyCommand(char* commandPath, char **tokens, int lf, int rt);
void removeQuotes(char* token,int len);
void parseArgument(char* token, int len);
void removeDollar(char *token, int len);

int main() {
    //TODO add your code
    int maxChars=3000;
    int maxTokens=130;
    int maxArgLen=19;

    char commandPath[25];
    char** command;

    int i,j;
    while(1) {
        int tokenNum = 0;
        char** tokens = readTokens(maxTokens, maxArgLen, &tokenNum, getCommand());
        if(tokenNum<=0) {freeTokenArray(tokens,maxTokens); continue;}
        int pdes[10][2];
        for(i=0;i<10;i++)
            pipe(pdes[i]);

        int cur=0;
        int out = dup(1);
        int in = dup(0);
        dup2(pdes[0][1],1);
        int curpipe=0;
        
        int commandNotFound=0,exit=0;

        for(i=0;i<tokenNum;i++){
            if(strcmp(tokens[i],"|")==0){
                command = copyCommand(commandPath,tokens,cur,i);

                int exe = execute(command, commandPath, i-cur);
                freeTokenArray(command, i-cur);
                
                if(exe ==-1){
                    commandNotFound=1;
                    break;
                } else if(exe==1) {
                    exit=1;
                    break;
                }

                dup2(pdes[curpipe][0],0);
                dup2(pdes[curpipe+1][1],1); 
                close(pdes[curpipe][1]);
                curpipe++;
                cur=i+1;
            }
        }
        dup2(out,1);
        close(out);
        if(commandNotFound){
            printf("%s not found\n",commandPath);
        } else if(exit) {
            printf("Goodbye!\n");
            freeTokenArray(tokens,tokenNum);
            return 0;
        } else{
            command = copyCommand(commandPath, tokens, cur, tokenNum);
            int exe=execute(command, commandPath, tokenNum-cur);
            freeTokenArray(command, tokenNum-cur);
            if(exe ==-1){
                printf("%s not found\n",commandPath);                
            } else if(exe==1) {
                printf("Goodbye!\n");
                freeTokenArray(tokens,tokenNum);
                return 0;
            }
        }
        dup2(in,0);
        close(in);
        freeTokenArray(tokens,tokenNum);
        printf("\n");
    }
    return 0;
}

char** copyCommand(char *commandPath, char **tokens, int lf, int rt) {
    char ** command=(char**) malloc(sizeof(char*) * (rt-lf));
    int i;
    for(i=lf;i<rt;i++){
        removeQuotes(tokens[i],strlen(tokens[i]));
        command[i-lf] = (char*) malloc(sizeof(char) * 19);
        strcpy(command[i-lf], tokens[i]);
    }
    strcpy(commandPath, tokens[lf]);
    return command;
}

void removeDollar(char* token, int len){
    int i;
    for(i=1;i<len;i++) token[i-1]=token[i];
    token[i-1]='\0';
}

void parseArgument(char* token, int len) {
    if(len>0 && token[0]=='$'){
        removeDollar(token,len);
        char *value = getenv(token);
        if(value!=NULL){
            strcpy(token,value);
        }else{
            strcpy(token,"");
        }
    }
}

void removeQuotes(char* token,int len) {
    int i,j=0;
    for(i=0;i<len;i++){
        if(token[i]!='"'){
            token[j++]=token[i];
        }
    }
    token[j]='\0';
}

int execute(char** tokens, char* commandPath, int tokenNum) {
    int check = checkCommandPath(commandPath);
    int i;
    if(check==0) {
        for(i=0;i<tokenNum;i++)
            parseArgument(tokens[i],strlen(tokens[i]));
        int childPid = fork();
        if(childPid==0) {
            tokens[tokenNum]=NULL;
            execv(commandPath, tokens);
        } else {
            waitpid(childPid,NULL,0);
        }
    } else if(check==2) {
        setenv(tokens[1],tokens[2],1);
    } else if(check == 3) {
        removeDollar(tokens[1],strlen(tokens[1]));
        unsetenv(tokens[1]);
    }
    return check;
}

char* getCommand() {
    char *command = NULL;
    size_t len = 0;
    printf("GENIE > ");
    getline(&command, &len, stdin);
    return command;
}
/*
* quit:return 1
* not found: return -1
* found: return 0
* set: return 2
* unset: return 3
*/
int checkCommandPath(char* commandPath) {
    struct stat p;
    if(strcmp(commandPath, "quit")==0){
        return 1;
    } else if(strcmp(commandPath, "set")==0){
        return 2;
    } else if(strcmp(commandPath, "unset")==0) {
        return 3;
    } else if(stat(commandPath, &p)==0){
        return 0;
    } else {
        char defaultPath[25]="/bin/";
        strcpy(commandPath,strcat(defaultPath,commandPath));
        if(stat(commandPath, &p)==0){
            return 0;
        }
    }
    return -1;
}

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer)
//Tokenize buffer
//Assumptions:
//  - the tokens are separated by " " and ended by newline
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - should use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* token;
    int i;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);
    
    //Nullify all entries
    for (int i = 0; i < maxTokenNum; i++) {
        tokenStrArr[i] = NULL;
    }

    token = strtok(buffer, " \n");
    
    i = 0;
    while (i < maxTokenNum && (token != NULL)) {
         //Allocate space for token string
        tokenStrArr[i] = (char*) malloc(sizeof(char*) * maxTokenSize);

        //Ensure at most 19 + null characters are copied
        strncpy(tokenStrArr[i], token, maxTokenSize - 1);

        //Add NULL terminator in the worst case
        tokenStrArr[i][maxTokenSize-1] = '\0';
        
        i++;
        token = strtok(NULL, " \n");
    }
    
    *readTokenNum = i;
    
    return tokenStrArr;
}

void freeTokenArray(char** tokenStrArr, int size) {
    int i = 0;

    //Free every string stored in tokenStrArr
    for (i = 0; i < size; i++) {
        if (tokenStrArr[i] != NULL) {
            free(tokenStrArr[i]);
            tokenStrArr[i] = NULL;
        }
    }
    //Free entire tokenStrArr
    free(tokenStrArr);

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}


