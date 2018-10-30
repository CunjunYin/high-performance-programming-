#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include "linkedList.h"

void checkMalloc();
// compile with gcc -o seq  sequential.c linkedList.c

#define DELIMTER " "
#define MAX(a,b) (((a)>(b))?(a):(b))

int sizeOfMatrix = 10;

int lines1 = 0;
int* row1 = NULL;
int* col1 = NULL;
float* data1 = NULL;

int lines2 = 0;
int* row2 = NULL;
int* col2 = NULL;
float* data2 = NULL;

int result_size = 0;
int* result_row = NULL;
int* result_col = NULL;
float* result_data =NULL;

bool file1 = true;

void readMatrix(char fileName[]){
    char line[BUFSIZ];
    FILE *dict = fopen(fileName,"r");
    
    if(file1 == true){
        int j = 0;
        while(fgets(line, sizeof line, dict) != NULL ){
            lines1++;
            row1 = (int*) realloc( row1,  lines1 * sizeof(int) );
            col1 = (int*) realloc( col1, lines1 * sizeof(int) );
            data1 = (float*) realloc( data1, lines1 * sizeof(float) );
            
            row1[j] = atoi( strtok(line, DELIMTER) );
            col1[j] = atoi( strtok(NULL, DELIMTER) );
            data1[j] = atof( strtok(NULL, DELIMTER) );
            
            sizeOfMatrix = MAX(row1[j], sizeOfMatrix);
            sizeOfMatrix = MAX(col1[j], sizeOfMatrix);
            j++;
        }
    }else{
        int j = 0;
        while(fgets(line, sizeof line, dict) != NULL ){
            lines2++;
            row2 = (int*) realloc( row2,  lines2 * sizeof(int) );
            col2 = (int*) realloc( col2, lines2 * sizeof(int) );
            data2 = (float*) realloc( data2, lines2 * sizeof(float) );
            
            row2[j] = atoi( strtok(line, DELIMTER) );
            col2[j] = atoi( strtok(NULL, DELIMTER) );
            data2[j] = atof( strtok(NULL, DELIMTER) );
            
            sizeOfMatrix = MAX(row2[j], sizeOfMatrix);
            sizeOfMatrix = MAX(col2[j], sizeOfMatrix);
            j++;
        }
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

float perofrmCaculation(int* f1_col, float* f1_data, int* f2_row, float* f2_data, int low1, int high1, int low2, int high2){
    int i = high1, j = high2;
    float sum = 0;
    for(i = low1 ; i <= high1; i++){
        for( j = low2 ; j <= high2; j++){
            if( f1_col[i] == f2_row[j]  ){
                sum += (f1_data[i] * f2_data[j]);
                break;
            }
        }
    }
    return sum;
}

float caculation(int rowC, int colC,
                 int* f1_row, int* f1_col, float* f1_data, int f1_size,
                 int* f2_row, int* f2_col, float* f2_data, int f2_size){
    float result = 0;
    int startIndex1 = -1,endIndex1 = -1,startIndex2 = -1, endIndex2 = -1;
    getLowAndHighIndex(f1_row, f1_size, rowC, &startIndex1, &endIndex1);
    getLowAndHighIndex(f2_col, f2_size, colC, &startIndex2, &endIndex2);
    
    result  = perofrmCaculation(f1_col, f1_data, f2_row, f2_data, startIndex1, endIndex1, startIndex2, endIndex2);
    
    return result;
    
}

void matrixMutilplication(int* f1_row, int* f1_col, float* f1_data,
                          int* f2_row, int* f2_col, float* f2_data,
                          int sizeOne, int sizeTwo){
#pragma omp parallel
    {
#pragma omp for
        //TODO change the sizeOfMatrix to the send size;
        for(int i =1; i <= sizeOfMatrix; i++ ){
            for(int j =1; j <= sizeOfMatrix; j++ ){
                float result = caculation(i, j, f1_row, f1_col, f1_data, sizeOne, f2_row, f2_col, f2_data, sizeTwo);
                if(result != 0) printf("%d %d %f\n",i,j,result);
            }
        }
    }
}

void init(){
    row1 = (int*) malloc( sizeof(int) * lines1 );
    col1 = (int*) malloc( sizeof(int) * lines1 );
    data1 = (float*) malloc( sizeof(float) * lines1 );
    
    row2 = (int*) malloc( sizeof(int) * lines2 );
    col2 = (int*) malloc( sizeof(int) * lines2 );
    data2 = (float*) malloc( sizeof(float)* lines2 );
    
    result_row = (int*) malloc( sizeof(int) * result_size );
    result_col = (int*) malloc( sizeof(int) * result_size );
    result_data = (float*) malloc( sizeof(float) * result_size );
    checkMalloc();
    

    /*
     master receive
     result_size
     result_row = (int*) realloc( row1,  result_size * sizeof(int) );
     result_col = (int*) realloc( col1, result_size * sizeof(int) );
     result_data = (float*) realloc( data1, result_size * sizeof(float) );
     */
    
    //TODO worker after worker receive
    /*
     
     row1 = (int*) realloc( row1,  lines1 * sizeof(int) );
     col1 = (int*) realloc( col1, lines1 * sizeof(int) );
     data1 = (float*) realloc( data1, lines1 * sizeof(float) );
     
     row2 = (int*) realloc( row2,  lines2 * sizeof(int) );
     col2 = (int*) realloc( col2, lines2 * sizeof(int) );
     data2 = (float*) realloc( data2, lines2 * sizeof(float) );
     */
    
}

int main(int argc, char**argv){
    init();
    //TODO master
    readMatrix(argv[1]);
    file1 = false;
    readMatrix(argv[2]);
    matrixMutilplication(row1, col1, data1, row2, col2, data2, lines1, lines2);
    
    return 0;
}

void checkMalloc(){
    if(row1 == NULL){
        printf("malloc error");
        exit(0);
    }
    
    if(col1 == NULL){
        printf("malloc error");
        exit(0);
    }
    
    if(data1 == NULL){
        printf("malloc error");
        exit(0);
    }
    
    if(row2 == NULL){
        printf("malloc error");
        exit(0);
    }
    
    if(col2 == NULL){
        printf("malloc error");
        exit(0);
    }
    
    if(data2 == NULL){
        printf("malloc error");
        exit(0);
    }
    
    if(result_row == NULL){
        printf("malloc error");
        exit(0);
    }
    
    if(result_col == NULL){
        printf("malloc error");
        exit(0);
    }
    
    if(result_data == NULL){
        printf("malloc error");
        exit(0);
    }
}
