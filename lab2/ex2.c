/*************************************
 * Lab 2 Exercise 2
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
int isEnd(char *ch);

char* getCommand();
int checkCommandPath(char* commandPath);

int main() {
    //TODO add your code
    char commandPath[25];
    int i;
    while(1) {
        int tokenNum = 0;
        char** tokens = readTokens(10, 19, &tokenNum, getCommand());
        if(tokenNum<=0) {freeTokenArray(tokens,tokenNum); continue;}
        
        strcpy(commandPath, tokens[0]);
        int check = checkCommandPath(commandPath);
        if(check==1) {
            int childPid = fork();
            if(childPid==0) {
                tokens[tokenNum]=NULL;
                execv(commandPath, tokens);
            } else {
                waitpid(childPid,NULL,0);
            }
        } else if(check==-1){
            printf("Goodbye!\n");
            fflush(stdout);
            freeTokenArray(tokens,tokenNum);
            return 0;
        } else {
            printf("%s not found\n", commandPath);
        }
        printf("\n");
        freeTokenArray(tokens,tokenNum);
        fflush(stdout);
    }
    return 0;
}

char* getCommand() {
    char *command = NULL;
    size_t len = 0;
    printf("GENIE>");
    fflush(stdout);
    getline(&command, &len, stdin);
    return command;
}

int checkCommandPath(char* commandPath) {
    struct stat p;
    if(strcmp(commandPath, "quit")==0){
        return -1;
    } else if(stat(commandPath, &p)==0){
        return 1;
    } else {
        char defaultPath[25]="/bin/";
        strcpy(commandPath,strcat(defaultPath,commandPath));
        if(stat(commandPath, &p)==0){
            return 1;
        }
    }
    return 0;
}

int isEnd(char *ch){
    return ch==NULL || *ch=='\n' || *ch=='\0';
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
    int i,j;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);
  
    for (int i = 0; i < maxTokenNum; i++) {
        tokenStrArr[i] = (char*) malloc(sizeof(char) * (maxTokenSize+1));
    }

    i=0;
    char* ch = buffer;
    while(!isEnd(ch)){
        while(ch!=NULL && *ch==' ') {ch++;}
        if(isEnd(ch)) break;
        j=0;
        while(!isEnd(ch) && *ch!=' '){
            if(*ch=='"'){
                ch++;
                while(*ch!='"') {tokenStrArr[i][j++]=*ch;ch++;}
                ch++;
            }else{
                tokenStrArr[i][j++]=*ch;
                ch++;
            }
        }
        tokenStrArr[i][j]='\0';
        i++;
    }

    /*
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
    */
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


