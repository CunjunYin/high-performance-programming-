//
//  linkedList.c
//
//
//  Created by Cunjun Yin
//
// test sumlation site https://bitstorm.org/gameoflife/ use: 10 cell row

#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <stdio.h>

// ignore: compile with gcc-6 -fopenmp -o lab1 lab1.c
// compile with: gcc -Wall -std=c99 -o gol GoL.c
static int DEAD = -1;
static int ALIVE = 1;

//header
int**malloc2DArray(int, int);
void printMap(int**);
void init(void);
void swapAndClean(int** , int **,int, int);
void fate(int, int);
int detect(int , int );
int my_atoi(const char *);
void display();
void timing(int);

//Globale Controlling Variable
int MAXROW = 0, MAXCOL = 0;
int**map;
int**newMap;

// functions
int main(int argc, char *argv[]){
    if(argc != 3) {
        exit(EXIT_FAILURE);
    }
    else {
        MAXROW = MAXCOL = my_atoi(argv[1]);
        int Steps = my_atoi(argv[2]);
        map = malloc2DArray(MAXROW, MAXROW);
        newMap = malloc2DArray(MAXROW, MAXROW);
        init();
        
        //clock_t begin = clock_start();
        int a[100000];
        int i,sum;
        struct timeval start, end;
        gettimeofday(&start, NULL);
        
        for(int i = 0; i < Steps;i++){
            display();
            swapAndClean(map,newMap,MAXROW,MAXCOL);
            //printMap(map);
        }
        
        gettimeofday(&end, NULL);
        double delta = ((end.tv_sec  - start.tv_sec) * 1000000u +
                 end.tv_usec - start.tv_usec) / 1.e6;
        
        printf("time=%12.10f\n",delta);
        
        //clock_t end = clock_stop();
        //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        //printf("time spent=%10.6f\n", time_spent);
        
        //timing(Steps);
        //exit(EXIT_SUCCESS);
    }
    
    return 0;
}

int**malloc2DArray(int m, int n){
    int**array = (int**)malloc(m*sizeof(int*));
    
    if(array == NULL){
        fprintf(stderr,"out of memory");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < m; i++){
        array[i] = (int*)malloc(n*sizeof(int));
        for(int j = 0; j< n; j++){
            array[i][j] = DEAD;
        }
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
        for( int r = row-1; r <= row+1; r++){
            for(int c = col - 1; c <= col+1; c++){
                if (r < 0 || r >= MAXROW || c < 0 || c >= MAXCOL) continue;
                if(map[r][c] == ALIVE) count++;
            }
        }
    if(map[row][col] == ALIVE) count--;
    return count;
}

void fate(int row, int col){
    switch (detect(row, col)) {
        case 2:
            newMap[row][col] = map[row][col];
            break;
        case 3:
            newMap[row][col] = ALIVE;
            break;
        default:
            break;
    }
}

void swapAndClean(int**a , int **b,int row, int col){
    int i,j;
#pragma omp parallel
    {
#pragma omp barrier
#pragma omp for private(i, j) nowait
        for(i = 0 ; i< row; i++){
            for(j = 0 ; j< col; j++){
                a[i][j] = b[i][j];
                b[i][j] = DEAD;
            }
        }
    }
}

void init(void){
    /*
generation
     0 **********
     
     1 ********
       ********
       ********
     
     2   ******
        *      *
       *        *
        *      *
         ******
     3
          ****
         ******
        ********
       **      **
        ********
         ******
          ****
        
     */
    
    int stratr = MAXROW/2;
    int stratc = MAXCOL/2;
    map[stratr][stratc] = ALIVE;
    map[stratr][stratc-1] = ALIVE;
    map[stratr][stratc-2] = ALIVE;
    map[stratr][stratc-3] = ALIVE;
    map[stratr][stratc-4] = ALIVE;
    map[stratr][stratc+5] = ALIVE;
    map[stratr][stratc+4] = ALIVE;
    map[stratr][stratc+3] = ALIVE;
    map[stratr][stratc+2] = ALIVE;
    map[stratr][stratc+1] = ALIVE;
}

void printMap(int**m){
    for(int i = 0 ; i< MAXCOL; i++){
        for(int j = 0 ; j< MAXROW; j++){
            if(m[i][j] == ALIVE){
                printf("*");
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

void display(){
    int row,col;
#pragma omp parallel
    {
#pragma omp barrier
#pragma omp for private(row, col) nowait
            for (row = 0; row < MAXROW; row++){
                for (col = 0; col < MAXCOL; col++){
                    fate(row,col);
                }
            }
    }
}
