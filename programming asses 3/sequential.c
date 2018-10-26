#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include "LinkedQueue.h"
#include <omp.h>
#include <sys/time.h>


// compile with gcc -o seq  sequential.c LinkedQueue.c
#define MAX(a,b) (((a)>(b))?(a):(b))
#define DELIMTER " "
extern char**environ;
int sizeOfMatrix = 0;


int getlines(char fileName[]){
    int lines = 0;
    FILE *dict = fopen(fileName,"r");
    while(!feof(dict)){
        char ch = fgetc(dict);
        if(ch == '\n') lines++;
    }
    fclose(dict);
    return lines;
}
void readMatrix(char fileName[], int* row, int* col, float* data){
    char line[BUFSIZ];
    FILE *dict = fopen(fileName,"r");
    int j  = 0;
    while(fgets(line, sizeof line, dict) != NULL ){
        row[j] = atoi( strtok(line, DELIMTER) );
        col[j] = atoi( strtok(NULL, DELIMTER) );
        sizeOfMatrix = MAX(row[j], sizeOfMatrix);
        sizeOfMatrix = MAX(col[j], sizeOfMatrix);
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
void getLowAndHighIndex(int* array,int size, int value, int*start, int* end){
    // get high index

    int index = binarysearch(array,value,size);
    int i = index + 1;
    for(    ; i<= size; i++){
        if(array[i]!= value){
            break;
        }
    }
    *end = i-1;
    
    // get low index
    int j = index - 1;
    for(    ; j<= 0; j-- ){
        if(array[j] != value){
            break;
        }
    }
    *start = j+1;
}

float perofrmCaculation(int*col1, float* data1, int*row2, float* data2, int low1, int high1, int low2, int high2){
    int i = high1, j = high2;
    float sum = 0;
    for(i = low1 ; i <= high1; i++){
        for( j = low2 ; j <= high2; j++){
            if( col1[i] == row2[j]  ){
                sum += (data1[i] * data2[j]);
                break;
            }
        }
    }
    return sum;
}

float caculation(int rowC, int colC, int*row1, int*col1, float*data1, int size1, int*row2, int* col2, float* data2, int size2){
    float result = 0;
    int startIndex1 = -1,endIndex1 = -1,startIndex2 = -1, endIndex2 = -1;
    getLowAndHighIndex(row1, size1, rowC, &startIndex1, &endIndex1);
    getLowAndHighIndex(col2, size2, colC, &startIndex2, &endIndex2);
    
    result  = perofrmCaculation(col1, data1, row2, data2, startIndex1, endIndex1, startIndex2, endIndex2);
    
    return result;
    
}

void matrixMutilplication(int* row1, int* col1, float* data1, int* row2, int* col2, float* data2 ,int sizeOne, int sizeTwo){
#pragma omp parallel
{
    #pragma omp for
    for(int i =1; i <= sizeOfMatrix; i++ ){
        for(int j =1; j <= sizeOfMatrix; j++ ){
            float result = caculation(i, j, row1, col1, data1, sizeOne, row2, col2, data2, sizeTwo);
            if(result!=0) Enqueue(i,j,result);
        }
    }
}
}

int sortFile(char fileName[],char column[] ){
    char* args[] = {"/usr/bin/sort","-t","\t","-n","-o",fileName,"-k",column,fileName,NULL};
    return execve(args[0],args,environ);
}

int main(int argc, char**argv){
    
    //TODO fork error, dosent sort the file, second file sorted into reverse order.
    
    pid_t pid_1,pid_2;
    if( (pid_1 = fork() ) < 0 ){
        perror("Fork Failed.");
    }else if (pid_1 == 0){
        int status_1 = sortFile(argv[1],"1");
        int status_2 = 0;
        pid_t pid_2 =fork();
        
        if( (pid_2 = fork()) < 0){
            perror("Fork Failed.");
        }else if(pid_2 == 0){
            status_2 = sortFile(argv[2],"2");
        }else{
            int returnStatus_2;
            waitpid(pid_2, &returnStatus_2, 0);
            exit(status_2);
        }
        exit(status_1);
    }else{
        int returnStatus;
        waitpid(pid_1, &returnStatus, 0);
        if (returnStatus == 0)  // Verify child process terminated without error.
        {
            struct timeval start, end;
            gettimeofday(&start, NULL);

            int lines1 = getlines(argv[1]);
            printf("lines%d \n",lines1);
            int row1[lines1];
            int col1[lines1];
            float data1[lines1];
            
            int lines2 = getlines(argv[2]);
            int row2[lines2];
            int col2[lines2];
            float data2[lines2];
            
            readMatrix(argv[1],row1,col1,data1);
            readMatrix(argv[2],row2,col2,data2);

            matrixMutilplication(row1, col1, data1, row2, col2, data2, lines1, lines2);
            Print();
            gettimeofday(&end, NULL);
            double delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
            printf("time=%12.10f\n",delta);
        }
        
    }
    
    return 0;
}
