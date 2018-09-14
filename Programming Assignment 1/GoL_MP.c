/*Cits3402 Assignment 1
 Cunjun Yin(22249435)
 Qianhao Liu(21952083)
 */

// test sumlation site https://bitstorm.org/gameoflife/ use: 10 cell row

#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <stdio.h>

//compile with gcc-6 -fopenmp -o gol GoL_MP.c

static int DEAD = -1; // value of dead cell will be -1
static int ALIVE = 1; // value of dead cell will be 1

//Function header
int**malloc2DArray(int, int);
int**malloc2DArray_OMP(int, int);
void printMap(int**);
void init(void);
void swapAndClean(int** , int **,int, int);
void swapAndClean_OMP(int** , int **,int, int);
void fate(int, int);
int detect(int , int );
int my_atoi(const char *);
void display_OMP();
void display();


//Globale Controlling Variable
int MAXROW = 0, MAXCOL = 0;
int**map;
int**newMap;

// Main Function that takes 3 input parameters
// First parameter will be the array size
// Second parameter is the number of stepes
// Third parameter is whether using open MP or not(1 is yes, 2 is no)
int main(int argc, char *argv[]){
    if(argc != 4) {
        exit(EXIT_FAILURE);
    }
    else {
        
        MAXROW = MAXCOL = my_atoi(argv[1]); // change the type of the inout string to number
        int Steps = my_atoi(argv[2]); // set up how many strps will be simulated
       
        // Whether use open_MP or Not(1 is yes, 2 is no)
        if(my_atoi(argv[3]) == 1){
            map = malloc2DArray_OMP(MAXROW, MAXROW);
            newMap = malloc2DArray_OMP(MAXROW, MAXROW);
        }
        else if(my_atoi(argv[3]) == 2){
            map = malloc2DArray(MAXROW, MAXROW);
            newMap = malloc2DArray(MAXROW, MAXROW);
        }
        
        // intinallize the starting pattern on the 2d array
        init();
        
    
        int a[100000];
        int i,sum;
        struct timeval start, end;
        gettimeofday(&start, NULL);
        
        // whether use openMP to process or not(1 is yes, 2 is no )
        for(int i = 0; i < Steps;i++){
            if(my_atoi(argv[3]) == 1){
                display_OMP();
                swapAndClean_OMP(map,newMap,MAXROW,MAXCOL);
                //printMap(map);
            }
            else if(my_atoi(argv[3]) == 2){
                display();
                swapAndClean(map,newMap,MAXROW,MAXCOL);
                //printMap(map);
            }
        }
        
        gettimeofday(&end, NULL);
        double delta = ((end.tv_sec  - start.tv_sec) * 1000000u +
                 end.tv_usec - start.tv_usec) / 1.e6;
        
        printf("time=%12.10f\n",delta);
        
    }
    return 0;
}

// An function that allocate the memeries for the 2d array depends on the secong parameter and process the game with Open mp
int**malloc2DArray_OMP(int m, int n){
    int**array = (int**)malloc(m*sizeof(int*));
    
    if(array == NULL){
        fprintf(stderr,"out of memory");
        exit(EXIT_FAILURE);
    }
    int i,j;
    //omp_set_num_threads(4);
#pragma omp parallel
    {
#pragma omp for private(i, j)
        for(i = 0; i < m; i++){
            array[i] = (int*)malloc(n*sizeof(int));
            for(j = 0; j< n; j++){
                array[i][j] = DEAD;
            }
            if(array[i]==NULL){
                fprintf(stderr,"out of memory");
                exit(EXIT_FAILURE);
            }
        }
    }
    return array;
}

// Function that allocate the memeries for 2d array depends on the second parameters without using openMP
int**malloc2DArray(int m, int n){
    int**array = (int**)malloc(m*sizeof(int*));
    
    if(array == NULL){
        fprintf(stderr,"out of memory");
        exit(EXIT_FAILURE);
    }
    int i,j;
        for(i = 0; i < m; i++){
            array[i] = (int*)malloc(n*sizeof(int));
            for(j = 0; j< n; j++){
                array[i][j] = DEAD;
            }
            if(array[i]==NULL){
                fprintf(stderr,"out of memory");
                exit(EXIT_FAILURE);
            }
        }
    return array;
}

// Self defined function change the type of string to int
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

// Function that use to judge whether the cell is live or dead depends on the rules of Game of Life
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

// Call the function detect after know the single cell is dead or live and then put the cell on the new map 2darray
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

// After checking each cell, and put all the live and dead cells to the new map and clean the old map using openMP
void swapAndClean_OMP(int**a , int **b,int row, int col){
    int i,j;
    //omp_set_num_threads(4);
#pragma omp parallel
    {
#pragma omp for private(i, j)
        for(i = 0 ; i< row; i++){
            for(j = 0 ; j< col; j++){
                a[i][j] = b[i][j];
                b[i][j] = DEAD;
            }
        }
    }
}

// After checking each cell, and put all the live and dead cells to the new map and clean the old map without using openMP
void swapAndClean(int**a , int **b,int row, int col){
    int i,j;
        for(i = 0 ; i< row; i++){
            for(j = 0 ; j< col; j++){
                a[i][j] = b[i][j];
                b[i][j] = DEAD;
            }
        }
}


// initialise the pattern
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

// print the map
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
    printf("\n");
}

// Function that calls the function fate and using for loop to decided all the cell in the new map whether its dead or live using openMp
void display_OMP(){
    int row,col;
    //omp_set_num_threads(4);
#pragma omp parallel
    {
#pragma omp for private(row, col)
            for (row = 0; row < MAXROW; row++){
                for (col = 0; col < MAXCOL; col++){
                    fate(row,col);
                }
            }
    }
}

// Function that calls the function fate and using for loop to decided all the cell in the new map whether its dead or live withou using openMp
void display(){
    for (int row = 0; row < MAXROW; row++){
        for (int col = 0; col < MAXCOL; col++){
            fate(row,col);
        }
    }
}
