#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
// compile with gcc-6 -fopenmp -o lab2 lab2.c

static long steps = 1000000000;
double step;

int main(int arg, char*argv[]){
    clock_t begin,end;

    int i;
    double x;
    double pi, sum = 0.0;
    double start, delta;
    step = 1.0/(double) steps;
    
    begin=clock();
#pragma omp parallel reduction (+:sum) private(x)
    {
#pragma omp for
        for(i=0;i<steps;i++){
            x=(i+0.5)*step;
            sum+=4.0/(1.0+x*x);
        }
    }
    pi=sum*step;
    end=clock();
    printf("pi=%18.16f and time =%f\n",pi,(end - begin) / CLOCKS_PER_SEC);
}
