#include "sudoku.h"
#include "grid.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define EXIT_FAILURE    1       /* Failing exit status.  */
#define EXIT_SUCCESS    0       /* Successful exit status.  */

/* Read a sudoku grid from stdin and return an object Grid_T
   initialized to these values. The input has the format:

1 2 3 4 5 6 7 8 9
4 5 6 7 8 9 1 2 3
7 8 9 1 2 3 4 5 6
2 3 4 5 6 7 8 9 1
5 6 7 8 9 1 2 3 4
8 9 1 2 3 4 5 6 7
3 4 5 6 7 8 9 1 2
6 7 8 9 1 2 3 4 5
9 1 2 3 4 5 6 7 8

   Each number is followed by a space. Each line is terminated with
   \n. Values of 0 indicate empty grid cells.*/




static void InitializeGrid(Grid_T* g){
  int i,j,k;
  g->unique=0;
  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      g->elts[i][j].choices.count=9;
      for(k=1;k<10;k++){
        g->elts[i][j].choices.num[k]=1;
      }
    }
  }
} 
/************************************************************************/
Grid_T sudoku_read(void){

 int num;
 int i=0,j=0;
 Grid_T grid;
 Grid_T* g=&grid;
  
 InitializeGrid(g);  

  while(!feof(stdin)){
     fscanf(stdin,"%d",&num);     
    
    if(0<=num && num<=9)
       grid_update_value(g,i,j,num);
  
     j++;
     
     if(j==9){
      j=0;
      i++;
    }
  }
  return grid;
}
 
/*****************************************************************************/
/* Print the sudoku puzzle defined by g to stream s in the same format
   as expected by sudoku_read(). */
void sudoku_print(FILE *s, Grid_T g){
  int i,j;
  int num;
  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      num=grid_read_value(g,i,j);
      if(j==8){
        fprintf(s,"%d \n",num);
      }else{
        fprintf(s,"%d ",num);
      }
    }
  }
}
/*****************************************************************************/


void sudoku_print_errors(Grid_T g){
int i,j,StrC,StrR,sum;
int ArrayRow[9][10]={{0}}; 
int ArrayCol[10][9]={{0}}; 

 /*Initialize ArrayRow and ArrayCol*/
  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      ArrayRow[i][g.elts[i][j].val]++;
      ArrayCol[g.elts[i][j].val][j]++;
    }
  }
  

  printf("\nRow Erros:\n");

  for(i=0;i<9;i++){
    for(j=1;j<10;j++){
      if(ArrayRow[i][j]==0){
        fprintf(stderr, "In the row %d missing the num: %d \n",i,j);
      }else if(ArrayRow[i][j]==2){
        fprintf(stderr, "In the row %d the number: %d appears more than once \n",i,j);
      }
    }
  }
  
  printf("\nCol Erros:\n");

  for(j=0;j<9;j++){
    for(i=1;i<10;i++){
      if(ArrayCol[i][j]==0){
        fprintf(stderr, "In the col %d missing the num: %d \n",j,i);
      }else if(ArrayCol[i][j]==2){
        fprintf(stderr, "In the col %d the number: %d appears more than once \n",j,i);
      }
    }
  }
    
  printf("\nGrid3x3 Erros:\n");

    for(StrR = 0 ; StrR < 9 ; StrR = StrR + 3){
      for (StrC = 0 ; StrC < 9 ; StrC = StrC +3){
        sum=0;
        for( i = 0 + StrR ; i <= StrR + 2; i++){
          for( j = 0 + StrC ; j <= StrC + 2 ; j++){
              sum = sum + g.elts[i][j].val; 
          }
        }
        if(sum!=45){
          fprintf(stderr,"The Grid 3x3 [%d..%d][%d..%d] has wrong!\n",StrR,(i-1),StrC,(j-1));
        }
      }
    }
}
/*****************************************************************************/
/* Return true iff puzzle g is correct. */
int sudoku_is_correct(Grid_T g){
  int i,j;
  int ArrayRow[9][10]={{0}}; 
  
  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      ArrayRow[i][g.elts[i][j].val]++;
    }
  }
  

  for(j=0;j<9;j++)
    for(i=1;i<10;i++)
      if(ArrayRow[i][j]!=1)
        return 0;

  return 1;
}

/*****************************************************************************/

static void UpDateToInitChoices(Grid_T *g, int r, int c, int value){
  if(value != 0 && g->elts[r][c].choices.num[value] == 1){
     g->elts[r][c].choices.num[value] = 0;
     g->elts[r][c].choices.count--;
   }
}

static void sudoku_init_choices(Grid_T *g){
  int i,j,k,z,StrRow,StrCol;

  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      if(g->elts[i][j].val==0){
        
        StrRow= (i/3)*3;
        StrCol= (j/3)*3;
        
        for(k=0;k<9;k++){
          UpDateToInitChoices(g,i,j,g->elts[i][k].val);
          UpDateToInitChoices(g,i,j,g->elts[k][j].val);
          UpDateToInitChoices(g,i,j,g->elts[StrRow + (k%3)][StrCol+(k/3)].val);
        }
         
      
    }
  }
 }
}



static int sudoku_update_choice(Grid_T *g, int i, int j, int n){
   g-> elts[i][j].choices.num[n]=0;
   g-> elts[i][j].choices.count--;
}


static void UpDateToReduceChoice(Grid_T *g, int r, int c, int n){
  if(g->elts[r][c].val == 0 && g->elts[r][c].choices.num[n]==1){
     g->elts[r][c].choices.num[n] = 0;
     g->elts[r][c].choices.count--;
   }
}

static void sudoku_eliminate_choice(Grid_T *g, int r, int c, int n){
  int i;
  int StrRow = (r/3)*3;
  int StrCol= (c/3)*3;

  for(i=0;i<9;i++){
    UpDateToReduceChoice(g,r,i,n);
    UpDateToReduceChoice(g,i,c,n);
    UpDateToReduceChoice(g,(StrRow+(i%3)),(StrCol+(i/3)),n);
  }
}

 static int  Get_1st_Choice(Grid_T g,int row, int col){
  int i=1;
  int choice;
  
  while( i <= 9){
    choice=i;
    if( grid_choice_is_valid( g, row, col, choice)){
      break;
    }
    i++;
  }
   return choice;
} 

static int HaveUniqueSolution(Grid_T g){
  int i,j,k,count;

  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
       if(g.elts[i][j].val!=0){
          count=0;
          for(k=1;k<10;k++)
            if(g.elts[i][k].choices.num[k]==1)
              count++;
       }
       if(count > 1) return 0;
    }
  }
}

/* Sovle puzzle g and return the solved puzzle; if the puzzle has
   multiple solutions, return one of the possible solutions. */
int finish=0;
Grid_T sudoku_solve(Grid_T g,int row,int col){
  int i,choice;
  Grid_T helpGrid;

  if(col == 0 && row == 0){
      sudoku_init_choices(&g);
      g.unique=HaveUniqueSolution(g);
  }

  if(col >= 9){
    col = 0;
    row++;
     if( row >= 9){
       finish=1;
       return g;
     }
  }

  if (g.elts[row][col].val != 0 ){
     return sudoku_solve(g,row,col+1);
  }else{
    if ( g.elts[row][col].choices.count == 0){
      return g;
    }else{
      
      while( g.elts[row][col].choices.count > 0){
         choice = Get_1st_Choice(g, row, col);                   /*if you have one choice do all the work at original grid*/
         g.elts[row][col].choices.num[choice]=0;
        
         if( g.elts[row][col].choices.count == 1){ 
               g.elts[row][col].val=choice;
               sudoku_eliminate_choice(&g,row,col,choice);
               helpGrid = sudoku_solve(g,row,col+1);
         }else{
              helpGrid=g; 
              helpGrid.elts[row][col].val=choice;
              sudoku_eliminate_choice(&helpGrid,row,col,choice);
              helpGrid = sudoku_solve(helpGrid,row,col+1);
         }

        if(finish == 1){
          return helpGrid;
        }
        
        g.elts[row][col].choices.count--;
     }
   
  }
 }
}

/*****************************************************************************/

/* Returns true if solution g, as returned by sudoku_solve, has a
   unique choice for each step (no backtracking required). (Note, g
   must have been computed with the use of sudoku_solve.) */
int sudoku_solution_is_unique(Grid_T g){
   if(g.unique==1)
    return 1;
  else
    return 0;
}

/*****************************************************************************/


/* Generate and return a sudoku puzzle with "approximately" nelts
   elements having non-0 value. The smaller nelts the harder may be to
   generate/solve the puzzle. For instance, nelts=81 should return a
   completed and correct puzzle. */
Grid_T sudoku_generate(int nelts){
  srand(time(NULL));
  int row ;
  int tries;
  int col ;
  Grid_T g;
  int i,j,k,rc;

  g.unique=0;
  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      g.elts[i][j].val=0;
      g.elts[i][j].choices.count=9;
      for(k=1;k<10;k++)
        g.elts[i][j].choices.num[k]=1;
    }
  }

   i=0; 
   while(1){
    row = rand()%9;
    col = rand()%9;
    if(g.elts[row][col].val == 0 ){
       rc=rand()%8+1;
       tries=0;
       while(g.elts[row][col].choices.num[rc]==0 && tries<10){
           rc=rand()%8+1;
           tries++;
       }
      g.elts[row][col].val=rc;
      sudoku_eliminate_choice(&g,row,col,rc);
      i++;   
    }
    if(i==nelts) break;  
  }
 return g; 
}
