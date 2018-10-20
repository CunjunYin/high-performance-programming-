#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include "linkedList.h"


// compile with gcc -o seq  sequential.c linkedList.c

#define DELIMTER " "
extern char**environ;
int sizeOfMatrix;


int getlines(char fileName[]){
    int lines = 0;
    FILE *dict = fopen(fileName,"r");
    while(!feof(dict)){
        char ch = fgetc(dict);
        if(ch == '\n') lines++;
    }
    fclose(dict);
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

int binarysearch(int* a, int value, int size){
    int low, high, mid;
    low = 0;
    high = size-1;
    while(low<=high){
        mid = (low+high)/2;
        if(a[mid]==value)
            return mid;
        if(a[mid]>value)
            high = mid-1;
        if(a[mid]<value)
            low = mid+1;
    }
    return -1;
}

int InsertionSearch(int a[], int value, int low, int high){
    int mid = low+(value-a[low])/(a[high]-a[low])*(high-low);
    if(a[mid]==value)
        return mid;
    if(a[mid]>value)
        return InsertionSearch(a, value, low, mid-1);
    if(a[mid]<value)
        return InsertionSearch(a, value, mid+1, high);
    return -1;
}

//prform the search operation to get high and low index
//******11111*******
//      ^   ^
void getLowAndHighIndex(int* array,int size, int value, int index, int*start, int* end){
    // get high index
    int i = index + 1;
    for(    ; i< size; i++){
        if(array[i]!= value){
            *end = --i;
            break;
        }
    }
    
    // get low index
    i = index + -1;
    for(    ; i< 0; i++ ){
        if(array[i] != value){
            *start = ++i;
            break;
        }
    }
}

int caculation(int rowC, int colC, int*row1, int*col1, int*data1, int*row2, int* col2, int* data2){
    int result = 0;
    //int startIndex1,endIndex1,startIndex2, endIndex2;
    
    //TODO
    return result;
    
}

void matrixMutilplication(int*row1, int* col1, int*data1,int* row2,int* col2, int*data2 ,int sizeOne, int sizeTwo){
    node*head  = NULL;
    for(int i =0; i < sizeOfMatrix; i++ ){
        for(int j =0; j < sizeOfMatrix; j++ ){
            int result = caculation(i,j,row1,col1,data1,row2,col2,data2);
            if(result !=0){
                push(head,i,j,result);
            }
        }
    }
    //TODO perofrm matrix into file
}

void sortFile(char fileName[],char column[] ){
    char* args[] = {"/usr/bin/sort","-t","\t","-n","-o",fileName,"-k",column,fileName,NULL};
    execve(args[0],args,environ);
}

int main(int argc, char**argv){
    sortFile(argv[1],"1");
    sortFile(argv[2],"2");
    
    int lines1 = getlines(argv[1]);
    int row1[lines1];
    int col1[lines1];
    float data1[lines1];
    
    int lines2 = getlines(argv[2]);
    int row2[lines2];
    int col2[lines2];
    float data2[lines2];
    
    readMatrix(argv[1],row1,col1,data1);
    readMatrix(argv[2],row2,col2,data2);
    
    
    return 0;
}
