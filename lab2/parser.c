#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer);
void freeTokenArray(char** tokenStrArr, int size);

int main(){
	size_t len = 0;
	char* command=NULL;
	getline(&command, &len, stdin);
	int num;
	char** tokens=readTokens(10,19,&num,command);
	for(int i=0;i<num;i++)printf("%s\n",tokens[i]);
	freeTokenArray(tokens,num);
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
	//printf("%s",buffer);
    char** tokenStrArr;
    int i,j;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);
    //Nullify all entries

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