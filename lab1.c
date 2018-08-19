#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
// compile with gcc-6 -fopenmp -o lab1 lab1.c
int charToInt(char c){
	
}

// header
void quickSort(int[], int , int);
// Q1 print with thre num and num threads in cup
void openMPrint(void){
    int thread_num = 0, num_progeam = 1;
    //Q3 clock of cpu time
    clock_t begin = clock();
    omp_set_num_threads(2);
#pragma omp parallel default(shared) private(thread_num, num_progeam)
    {
#if defined (_OPENMP)
        num_progeam = omp_get_num_threads();
        thread_num = omp_get_thread_num();
#endif
        // Q2 sleep for 1 sec
        if(omp_get_thread_num() == 0){
            sleep(1);
        }
         printf("Hello from thread %d out of %d\n", thread_num, num_progeam);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("time spent=%10.6f\n",time_spent);
}

//Q4 parallelize for loop
void forLoopPall(void){
    int n = 4;
    float localSum = 0.0;
    float arr[] = {0.362, 0.912, 0.775, 0.551, 0.066, 0.714, 0.728, 0.950, 0.415, 0.797};
    omp_set_num_threads(n);
#pragma omp parallel reduction (+:localSum)
    {
#pragma omp for
            for( int i = 0; i < 10; i++ ){
                localSum += arr[i];
            }
    }
    printf("local sum: %f\n",localSum);
}

void forLoopPall2(void){
    int n = 4;
    float sum = 0.0;
    float localSum = 0.0;
    float arr[] = {0.362, 0.912, 0.775, 0.551, 0.066, 0.714, 0.728, 0.950, 0.415, 0.797};
    omp_set_num_threads(n);
#pragma omp parallel private(localSum)
    {
#pragma omp for
        for( int i = 0; i < 10; i++ ){
            localSum += arr[i];
        }
#pragma omp critical
        {
            sum+=localSum;
        }
    }
    printf("local sum: %f\n",sum);
}

// Q5 parallel quick sort
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[],int p,int r){
    int x = arr[r];
    int i = p-1;
//#pragma omp parallel for
    for(int j = p; j < r; j++){
        if(arr[j] <=x){
            i = i +1;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[r]);
    return i+1;
}

void quickSort(int arr[], int p, int r)
{
    int q = 0;
    if(p<r)
    {
#pragma omp parallel sections num_threads(4)
        {
        q = partition(arr,p,r);
#pragma omp section
        quickSort(arr,p,q-1);
#pragma omp section
        quickSort(arr,q+1,r);
        }
    }
}

void parallelQuickSort(void){
    int arr[] = {2,1,4,6,8,0,11,3,50,40};
    quickSort(arr,0,9);
    for(int i = 0 ; i< 10 ; i++){
        printf("%d ",arr[i]);
    }
}

// function calling step
int main(int argc, char *argv[]){
    parallelQuickSort();
	return 0;
}
