#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>

// compile with cc -std=c99 -Wall -Werror -pedantic -o seq  sequential.c

#define DELIMTER " "

void readMatrix(char fileName[]){
    FILE *dict = fopen(fileName,"r");
    
    char   line[BUFSIZ];
    char *tempData = "";
    //
    int*row = (int *)malloc(sizeof(int)*0);
    int*col = (int *)malloc(sizeof(int)*0);
    float*data = (float *)malloc(sizeof(float)*10);
    int sizeAllocated = 0;
    //
    while(fgets(line, sizeof line, dict) != NULL ){
        sizeAllocated++;
        row = (int*)realloc(row,sizeAllocated);
        col = (int*)realloc(col,sizeAllocated);
        
        tempData = strtok(line, DELIMTER);
        row[sizeAllocated-1] = atoi(tempData);
        
        tempData = strtok(NULL, DELIMTER);
        col[sizeAllocated-1] = atoi(tempData);
        
        tempData = strtok(NULL, DELIMTER);
        data[sizeAllocated - 1] = atof(tempData);
        printf("data[%d] %f\n",sizeAllocated-1,data[0]);
    }
    
    /*for(int i = 0; i< sizeAllocated; i++){
        printf("%d ",row[i]);
    }
    printf("\n");
    for(int i = 0; i< sizeAllocated; i++){
        printf("%d ",col[i]);
    }*/
}

int main(int argc, char*argv[]){
    readMatrix(argv[1]);
    return 0;
}





