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
#define ALIVE 1
#define DEAD -1

//header
int**malloc2DArray(int, int)

//Globale Controlling Variable
int MAXROW = 0, MAXCOL = 0;
int **gameArr;

// functions
int main(int argc, char *argv[]){
    if(argc != 2) {
        exit(EXIT_FAILURE);
    }
    else {
        MAXROW = MAXCOL = my_atoi(argv[1]);
        exit(EXIT_SUCCESS);
    }
    return 0;
}

int**malloc2DArray(int m, int n){
    int**array = (int**)malloc(m*sizeof(int*));
    
    if(array == NULL){
        fprintf(stderr,"out of memory");
        exit(EXIT_FAILURE);
    }
//#pragma omp parallel
    for(int i = 0; i < m; i++){
        array[i] = (int*)malloc(n*sizeof(int));
        if(array[i]==NULL){
            fprintf(stderr,"out of memory");
            exit(EXIT_FAILURE);
        }
    }
    return array;
}

int my_atoi(const char *str){
    int num = 0;
    if (*str == '-') str++;
    else if (*str == '+') str++;
    
    while (*str != '\0'){
        if ((*str < '0') || (*str > '9')) exit(EXIT_FAILURE);
        num = num * 10 + (*str - '0');
        str++;
    }
    
    return num;
}

int detect(int row, int col){
    int count = 0;
    int r = row - 1; if( r < 0 ) r = 0;
    int c = col - 1; if( c < 0 ) c = 0;
    int maxR = row + 1; if( maxR > MAXROW) maxR = MAXROW;
    int macC = col + 1; if( macR > MAXCOL) macR = MAXCOL;
    
    for(r = row - 1; r <= row+1; r++){
        for(c = col - 1; c <= col+1; c++){
            if(gameArr[r][c] == ALIVE) count++;
        }
    }

    if(gameArr[row][col] == ALIVE) count--;
    return count;
}

int init(void){
    
}

void fate(int col, int row){
    switch (detect(row, col)) {
        case 0:
            statements
            break;
        case 1:
            <#statements#>
            break;
        case 2:
            <#statements#>
            break;
        default:
            break;
    }
}
















