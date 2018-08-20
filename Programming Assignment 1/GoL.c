//
//  linkedList.c
//
//
//  Created by Cunjun Yin
//

#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "linkedList.h"

// compile with gcc-6 -fopenmp -o lab1 lab1.c

//header
int**malloc2DArray(int, int)

// functions
int main(int argc, char *argv[]){
    if(argc != 2) {
        exit(EXIT_FAILURE);
    }
    else {
        exit(EXIT_SUCCESS);
    }
    return 0;
}

int**malloc2DArray(int m, int n)
{
    int**array = (int**)malloc(m*sizeof(int*));
    
    if(array == NULL)
    {
        fprintf(stderr,"out of memory");
        exit(EXIT_FAILURE);
    }
#pragma omp parallel
    for(int i = 0; i < m; i++)
    {
        array[i] = (int*)malloc(n*sizeof(int));
        if(array[i]==NULL)
        {
            fprintf(stderr,"out of memory");
            exit(EXIT_FAILURE);
        }
    }
    return array;
}





