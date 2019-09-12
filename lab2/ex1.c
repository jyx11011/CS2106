/*************************************
 * Lab 2 Exercise 1
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     //for fork(), wait()
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void readFromFile (char who[50], int fd);
void freadFromFile (char who[50], FILE* file);

int main(int argc, char **argv) {
    char* fileName = NULL;
    int nChild = 0;
    int fd = 0;
    char who[50];
    int i;
    
    if (argc < 3)
    {
        printf ("Usage: %s <filename> <number_of_processes>\n", argv[0]);
        return 1;
    }
    
    fileName = argv[1];
    nChild = atoi(argv[2]);

    fd = open( fileName, O_RDONLY );
    //TODO check the error code
    //TODO add your code
    if(fd == -1) {
    	printf("Fail to open file.");
    	return 0;
    }
    char charBuf;
    ssize_t readBytes = read( fd, &charBuf, 1 );
    if( readBytes != 1 ) 
    {
		if( readBytes == 0 ) 
        {
			printf("Empty file");
			return 0;
		}
	}
	char strBuf[128];
	sprintf(who, "Parent [%d]", getpid());
    sprintf(strBuf, "%s: %c\n", who, charBuf);
	write (STDOUT_FILENO, strBuf, strlen(strBuf));
    
    for(i=0;i<nChild;i++){
    	int chidpid = fork();
    	if(chidpid == 0) {
    		sprintf(who, "Child %d[%d]", i+1, getpid());
    		readFromFile(who, fd);
    		sprintf(strBuf, "Parent: %s done.\n",who);
			write (STDOUT_FILENO, strBuf, strlen(strBuf));
    		exit(0);
    	}
    }

    sprintf(who, "Parent [%d]", getpid());

    readFromFile(who, fd );
    for(i=0;i<nChild;i++){
    	wait(NULL);
    }
    close (fd);

    //TODO add your code
    FILE* file = fopen(fileName, "r");
    if(file==NULL){
    	printf("Fail to open file");
    	return 0;
    }
    size_t readsize = fread(&charBuf,1,1,file);
    if( readsize != 1 ) 
    {
		if( readsize == 0 ) 
        {
			printf("Empty file");
			return 0;
		}
	}
	sprintf(who, "Parent [%d]", getpid());
	sprintf(strBuf, "%s: %c\n", who, charBuf);
	fwrite (strBuf, sizeof(char), strlen(strBuf), stdout);

	for(i=0;i<nChild;i++){
    	int chidpid = fork();
    	if(chidpid == 0) {
    		sprintf(who, "Child %d[%d]", i+1, getpid());
    		freadFromFile(who, file);
    		sprintf(strBuf, "Parent: %s done.\n",who);
			fwrite (strBuf, sizeof(char), strlen(strBuf),stdout);
    		exit(0);
    	}
    }

    sprintf(who, "Parent [%d]", getpid());

    freadFromFile(who, file );
    for(i=0;i<nChild;i++){
    	wait(NULL);
    }


    fclose (file);
    sprintf(strBuf, "Parent: Exiting.\n");
    fwrite(strBuf, sizeof(char), strlen(strBuf), stdout);
    return 0;
}   

void readFromFile (char who[50], int fd) 
{
	ssize_t readBytes = 1;
	char charBuf = 0;
    char strBuf[128];

	while (readBytes > 0) {
		usleep (1000);
		charBuf = 0;
		readBytes = read( fd, &charBuf, 1 );

		if( readBytes != 1 ) 
        {
			if( readBytes == 0 ) 
            {
				//printf( "EOF\n");
				return;
			}
		}
        sprintf(strBuf, "%s: %c\n", who, charBuf);
	    write (STDOUT_FILENO, strBuf, strlen(strBuf));
		//TODO add your code
	}
}


void freadFromFile (char who[50], FILE* file) 
{
	size_t readBytes = 1;
	char charBuf = 0;
    char strBuf[128];

	while (readBytes > 0) {
		usleep (1000);
		charBuf = 0;
		readBytes = fread(&charBuf,1,1,file);

		if( readBytes != 1 ) 
        {
			if( readBytes == 0 ) 
            {
				//printf( "EOF\n");
				return;
			}
		}
        sprintf(strBuf, "%s: %c\n", who, charBuf);
	    fwrite (strBuf, sizeof(char), strlen(strBuf), stdout);
		//TODO add your code
	}
}