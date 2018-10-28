#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <mpi.h>
#include "LinkedQueue.h"


// compile with gcc -o seq  sequential.c LinkedQueue.c
#define MAX(a,b) (((a)>(b))?(a):(b))
#define DELIMTER " "
#define MASTER 0 /* taskid of first task */
#define FROM_MASTER 1 /* setting a message type */
#define FROM_WORKER 2 /* setting a message type */


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
    float sum = 0;
    for(int i = low1; i <= high1; i++){
        for(int j = low2; j <= high2; j++){
            if( col1[i] == row2[j]  ){
                sum += (data1[i] * data2[j]);
                //printf("%f  %f\n",data1[i],data2[j]);
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

void matrixMutilplication(int* row1, int* col1, float* data1, int* row2, int* col2, float* data2 ,int sizeOne, int sizeTwo, int offset, int row, int sizeMatrix){
    //TODO
    int end = 0;
    if( (end = row+offset)>sizeMatrix ) end = sizeMatrix;
    for(int i = offset; i <= end; i++ ){
        for(int j =1; j <= sizeMatrix; j++ ){
            float result = caculation(i, j, row1, col1, data1, sizeOne, row2, col2, data2, sizeTwo);
            //if(result!=0) Enqueue(front, rear, i, j, result);
            printf("%d %d %f",i,j,result);
        }
    }
    //TODO
}

void i_toString(int* a, int start, int end){
    for(int i= start; i < end; i++){
        printf("%d ",a[i]);
    }
    printf("\n");
}

void f_toString(float* a, int start, int end){
    for(int i= start; i < end; i++){
        printf("%f ",a[i]);
    }
    printf("\n");
}

int main(int argc, char**argv){
    int numtasks, /* number of tasks in partition */
    taskid, /* a task identifier */
    numworkers, /* number of worker tasks */
    source, /* task id of message source */
    dest, /* task id of message destination */
    mtype, rc, /* message type */
    rows, /* rows of matrix A sent to each worker */
    averow, extra, offset; /* used to determine rows sent to each worker */

    int lines1 = getlines(argv[1]);
    printf("lines%d \n",lines1);
    int row1[lines1];
    int col1[lines1];
    float data1[lines1];
        
    int lines2 = getlines(argv[2]);
    int row2[lines2];
    int col2[lines2];
    float data2[lines2];

    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    if (numtasks < 2 ) {
        printf("Need at least two MPI tasks. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }
    numworkers = numtasks-1;

    if (taskid == MASTER){       
        printf("mpi_mm has started with %d tasks.\n",numtasks);
        printf("Initializing arrays...\n");

        readMatrix(argv[1],row1,col1,data1);
        readMatrix(argv[2],row2,col2,data2);

        if (lines1 == 0 || lines2 == 0) {
            printf("Empty file\n");
            MPI_Abort(MPI_COMM_WORLD, rc);
            exit(1);
        }

        averow = sizeOfMatrix/numworkers;
        extra = sizeOfMatrix%numworkers;
        offset = 0;

        mtype = FROM_MASTER;
        for (dest = 1; dest <= numworkers; dest++){
            rows = (dest <= extra) ? (averow + 1) : averow;
            printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
            MPI_Send(&offset , 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&sizeOfMatrix, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);

            MPI_Send(&lines1, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&row1, lines1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&col1, lines1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&data1, lines1, MPI_FLOAT, dest, mtype, MPI_COMM_WORLD);

            MPI_Send(&lines2, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&row2, lines2, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&col2, lines2, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&data2, lines2, MPI_FLOAT, dest, mtype, MPI_COMM_WORLD);
            offset = offset + rows;
        }
    }

    if(taskid > MASTER){
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&sizeOfMatrix, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

        MPI_Recv(&lines1, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD,&status);
        MPI_Recv(&row1, lines1, MPI_INT, MASTER, mtype,MPI_COMM_WORLD, &status);
        MPI_Recv(&col1, lines1, MPI_INT, MASTER, mtype,MPI_COMM_WORLD, &status);
        MPI_Recv(&data1, lines1, MPI_FLOAT, MASTER, mtype,MPI_COMM_WORLD, &status);

        MPI_Recv(&lines2, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD,&status);
        MPI_Recv(&row2, lines2, MPI_INT, MASTER, mtype,MPI_COMM_WORLD, &status);
        MPI_Recv(&col2, lines2, MPI_INT, MASTER, mtype,MPI_COMM_WORLD, &status);
        MPI_Recv(&data2, lines2, MPI_FLOAT, MASTER, mtype,MPI_COMM_WORLD, &status);

        printf("offset = %d\n",offset);
        printf("averow = %d\n",averow);
        printf("sizeOfMatrix = %d", sizeOfMatrix);

        i_toString(row1,0,lines1);
        i_toString(col1,0,lines1);
        f_toString(data1,0,lines1);

        i_toString(row2,0,lines2);
        i_toString(col2,0,lines2);
        f_toString(data2,0,lines2);
        //matrixMutilplication(row1, col1, data1, row2, col2, data2, lines1, lines2, offset, averow,sizeOfMatrix);
    }
    MPI_Finalize();

    return 0;
}
