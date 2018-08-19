#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
// compile with gcc -fopenmp -o lab1 lab1.c
int charToInt(char c){
	
}
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



int main(int argc, char *argv[]){
    forLoopPall();
	return 0;
}
