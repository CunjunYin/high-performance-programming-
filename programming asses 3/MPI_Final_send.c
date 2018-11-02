#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <mpi.h>
#include "LinkedQueue.h"
#include <omp.h>

void checkMalloc();
// compile with gcc -o seq  sequential.c linkedList.c

#define DELIMTER " "
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MASTER 0 /* taskid of first task */
#define FROM_MASTER 1 /* setting a message type */
#define FROM_WORKER 2 /* setting a message type */

int sizeOfMatrix = 0; // send to worker

int lines1 = 0; // send to worker
int* row1 = NULL; // send
int* col1 = NULL;   // send
float* data1 = NULL;   // send

int lines2 = 0; // send
int* row2 = NULL; // send
int* col2 = NULL; // send
float* data2 = NULL; //send

int result_size = 0; // worker to  master
int* result_row = NULL; // worker to mster
int* result_col = NULL; // worker to master
float* result_data =NULL; // worker to master

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
                 int* f2_row, int* f2_col, float* f2_data, int f2_size,
                int index1, int index2){
    float result = 0;
    int startIndex1 = index1, endIndex1 = index2, startIndex2 = -1, endIndex2 = -1;
    bool found = false;
    for(int i = 0; i < f2_size; i++){
        if(f2_col[i] == colC && found == false){
            startIndex2 = i;
            found = true;
        }
        if(found == true){
            if(f2_col[i] == colC){
                endIndex2 = i;
            }else{
                break;
            }
        }
    }
    
    if(found == false) return result;
    if(found == true && endIndex2 == -1) endIndex2 = f2_size-1;
    
    
    result  = perofrmCaculation(f1_col, f1_data, f2_row, f2_data, startIndex1, endIndex1, startIndex2, endIndex2);
    
    return result;
    
}

void matrixMutilplication(int* f1_row, int* f1_col, float* f1_data,
                          int* f2_row, int* f2_col, float* f2_data,
                          int sizeOne, int sizeTwo){
    int j;
    int startIndex1 = -1,endIndex1 = -1;
    bool found;
#pragma omp parallel reduction(+:result_size)
    {
        //TODO change the sizeOfMatrix to the send size;
        for(int i = f1_row[0]; i <= f1_row[sizeOne-1]; i++ ){
            startIndex1 = -1;
            found = false;
            for(int k = endIndex1+1; k < sizeOne; k++){
                if(f1_row[k] == i && found == false){
                    startIndex1 = k;
                    found = true;
                }
                if(found == true){
                    if(f1_row[k] == i){
                        endIndex1 = k;
                    }else{
                        break;
                    }
                }
            }
            if(found == false) continue;
            if(found == true && endIndex1 == -1) endIndex1 = sizeOne-1;
            
            #pragma omp for firstprivate(i) private(j)
            for(j =1; j <= sizeOfMatrix; j++ ){
                float result = caculation(i, j, f1_row, f1_col, f1_data, sizeOne, f2_row, f2_col, f2_data, sizeTwo, startIndex1, endIndex1);
                if(result != 0) {
                    //printf("%d %d %f\n",i,j,result);
                    Enqueue(i, j, result);
                    result_size++;
                }
            }
        }
    }
    printf("%d\n",result_size);
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
    partition, /* rows of matrix A sent to each worker */
    extra_partition, offset, index_need_find, temp_offset; /* used to determine rows sent to each worker */
    
    init();
    
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    if (numtasks < 2 ) {
        printf("Need at least two MPI tasks. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }
    
    if(taskid == MASTER){
        //printf("Mpi_mm has started with %d tasks.\n",numtasks);
        //printf("Reading the matrix and allocate the array dynamiclly...\n");
    
        readMatrix(argv[1]);
        file1 = false;      //
        readMatrix(argv[2]);
        //printf("Memory allocating finsished for rows, clos and data\n");
        
        if (lines1 == 0 || lines2 == 0) {
            printf("Empty file\n");
            MPI_Abort(MPI_COMM_WORLD, rc);
            exit(1);
        }
        
        numworkers = numtasks - 1;
        partition = lines1/numworkers;
        //extra_partition = lines1%numworkers;
        offset=0;
        int temp_line1 = lines1;
        
        mtype = FROM_MASTER;
        //printf("Mater stats to dristribute data if number of line1 are 5 times more than number of workers\n");
        if( lines1>numworkers*5) {
            for (dest = 1; dest <= numworkers; dest++){
                MPI_Send(&sizeOfMatrix, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                if(dest != numworkers){
                    
                    //temp_offset = (dest <= extra_partition) ? (partition + 1) : partition;
                    
                    if(partition > temp_line1){
                        partition = temp_line1;   
                    }
                    int temp_number = row1[partition-1];
                    int counter = 0;
                    for(int i = 0; i < temp_line1; i++){
                        if (row1[i] != temp_number){
                            break;
                        }
                        counter++;
                    }
                    
                    offset = counter;
                    MPI_Send(&lines1, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(row1, offset, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(col1, offset, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(data1, offset, MPI_FLOAT, dest, mtype, MPI_COMM_WORLD);
                    
                    MPI_Send(&lines2, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(row2, lines2, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(col2, lines2, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(data2, lines2, MPI_FLOAT, dest, mtype, MPI_COMM_WORLD);
                    
                    temp_line1 -= offset;
                    if(temp_line1 > 0){
                        for (int i = 0; i < offset; i++){
                            row1 ++;
                            col1 ++;
                            data1 ++;
                        }
                    }else{
                        temp_line1 = 0;
                    }
                    
                    offset = 0;
                    
                }else{
                    offset = temp_line1;
                    
                    MPI_Send(&lines1, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(row1, offset, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(col1, offset, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(data1, offset, MPI_FLOAT, dest, mtype, MPI_COMM_WORLD);
                    
                    MPI_Send(&lines2, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(row2, lines2, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(col2, lines2, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                    MPI_Send(data2, lines2, MPI_FLOAT, dest, mtype, MPI_COMM_WORLD);
                }
            }
        }else{
            offset = lines1;
            MPI_Send(&sizeOfMatrix, 1, MPI_INT, 1, mtype, MPI_COMM_WORLD);
            
            MPI_Send(&lines1, 1, MPI_INT, 1, mtype, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, 1, mtype, MPI_COMM_WORLD);
            MPI_Send(row1, offset, MPI_INT, 1, mtype, MPI_COMM_WORLD);
            MPI_Send(col1, offset, MPI_INT, 1, mtype, MPI_COMM_WORLD);
            MPI_Send(data1, offset, MPI_FLOAT, 1, mtype, MPI_COMM_WORLD);
            
            MPI_Send(&lines2, 1, MPI_INT, 1, mtype, MPI_COMM_WORLD);
            MPI_Send(row2, lines2, MPI_INT, 1, mtype, MPI_COMM_WORLD);
            MPI_Send(col2, lines2, MPI_INT, 1, mtype, MPI_COMM_WORLD);
            MPI_Send(data2, lines2, MPI_FLOAT, 1, mtype, MPI_COMM_WORLD);
            offset = 0;
            for (dest = 2; dest <= numworkers; dest++){
                MPI_Send(&sizeOfMatrix, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                
                MPI_Send(&lines1, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(row1, offset, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(col1, offset, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(data1, offset, MPI_FLOAT, dest, mtype, MPI_COMM_WORLD);
                
                MPI_Send(&lines2, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(row2, lines2, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(col2, lines2, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(data2, lines2, MPI_FLOAT, dest, mtype, MPI_COMM_WORLD);
            }
        }
        
        //printf("Master finish sending\n");
        //printf("Master start to receive\n");
        mtype = FROM_WORKER;
        for (int i=1; i<=numworkers; i++){
            source = i;
            MPI_Recv(&result_size, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            free(result_row);
            free(result_col);
            free(result_data);
            result_row = (int*) malloc( sizeof(int) * result_size );
            result_col = (int*) malloc( sizeof(int) * result_size );
            result_data = (float*) malloc( sizeof(float) * result_size );     
            MPI_Recv(result_row, result_size, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(result_col, result_size, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(result_data, result_size, MPI_FLOAT, source , mtype, MPI_COMM_WORLD, &status);
            
            for(int i = 0; i < result_size; i++){
                //printf("workerrrrrrrrrrrrrrrrrrrrrrrrrrr %d\n", i);
                printf("%d %d %f\n",result_row[i],result_col[i],result_data[i]);
            }
        }
        //printf("Master receive finished\n");
    }
    

    if(taskid > MASTER){
       // printf("worker starts to receive\n");
        mtype = FROM_MASTER;
        MPI_Recv(&sizeOfMatrix, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD,&status);
        
        MPI_Recv(&lines1, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD,&status);
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD,&status);
        row1 = (int*) realloc( row1,  offset * sizeof(int) );
        col1 = (int*) realloc( col1, offset * sizeof(int) );
        data1 = (float*) realloc( data1, offset * sizeof(float) );
        MPI_Recv(row1, offset, MPI_INT, MASTER, mtype,MPI_COMM_WORLD, &status);
        MPI_Recv(col1, offset, MPI_INT, MASTER, mtype,MPI_COMM_WORLD, &status);
        MPI_Recv(data1, offset, MPI_FLOAT, MASTER, mtype,MPI_COMM_WORLD, &status);
        
        
        MPI_Recv(&lines2, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD,&status);
        row2 = (int*) realloc( row2,  lines2 * sizeof(int) );
        col2 = (int*) realloc( col2, lines2 * sizeof(int) );
        data2 = (float*) realloc( data2, lines2 * sizeof(float) );
        MPI_Recv(row2, lines2, MPI_INT, MASTER, mtype,MPI_COMM_WORLD, &status);
        MPI_Recv(col2, lines2, MPI_INT, MASTER, mtype,MPI_COMM_WORLD, &status);
        MPI_Recv(data2, lines2, MPI_FLOAT, MASTER, mtype,MPI_COMM_WORLD, &status);


        //printf("worker id: is doing the matrix matrixMutilplication%d \n",taskid);
        if(offset > 0){
            matrixMutilplication(row1, col1, data1,
                                row2, col2, data2,
                                 offset, lines2);

        result_row = (int*) realloc(result_row, sizeof(int) * result_size );
        result_col = (int*) realloc( result_col, sizeof(int) * result_size );
        result_data = (float*) realloc(result_data, sizeof(float) * result_size );
        
        queueToArrays(result_row, result_col, result_data);

        //i_toString(result_row,0,result_size-1);
        //Print();
        }

        //printf("Worker finish the work and sending back to Master\n");
        mtype = FROM_WORKER;
        MPI_Send(&result_size, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(result_row,result_size,MPI_INT,MASTER,mtype,MPI_COMM_WORLD);
        MPI_Send(result_col,result_size,MPI_INT,MASTER,mtype,MPI_COMM_WORLD);
        MPI_Send(result_data,result_size,MPI_FLOAT,MASTER,mtype,MPI_COMM_WORLD);
    }
    
    //printf("Fished Yeah!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    MPI_Finalize();
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
