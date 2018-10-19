#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>


// compile with gcc -o seq  sequential.c



#define DELIMTER " "
extern char**environ;

int getlines(FILE* dict){
    int lines = 0;
    while(!feof(dict)){
        char ch = fgetc(dict);
        if(ch == '\n') lines++;
    }
    lines++;
    return lines;
}
void readMatrix(char fileName[], int* row, int* col, float* data){
    char line[BUFSIZ];
    FILE *dict = fopen(fileName,"r");
    int j  = 0;
    while(fgets(line, sizeof line, dict) != NULL ){
        row[j] = atoi( strtok(line, DELIMTER) );
        col[j] = atoi( strtok(NULL, DELIMTER) );
        data[j] = atof( strtok(NULL, DELIMTER) );
        j++;
    }
    fclose(dict);
}

int main(int argc, char**argv){
    char* args[] = {"/usr/bin/sort","-t","\t","-n","-o","3.txt","-k","1",NULL};
    execve(args[0],args,environ);
    FILE *dict = fopen(argv[1],"r");
    int lines = getlines(dict);
    int row[lines];
    int col[lines];
    float data[lines];
    readMatrix(argv[1],row,col,data);
    
    
    return 0;
}
