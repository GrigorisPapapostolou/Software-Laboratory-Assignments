#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "sudoku.h"
#include "grid.h"

int main(int argc, char *argv[]){
   Grid_T g;
   int nelts;
       if( argc > 1){
   	       if(!strcmp(argv[1], "-g")){
   	       	  nelts=atoi(argv[2]);
         	  g=sudoku_generate(nelts);
              sudoku_print(stderr,g);
   	       }else if(!strcmp(argv[1],"-c")){
     	      g=sudoku_read();
     	      sudoku_print(stderr,g);
              fprintf(stderr,"The sudoku is correct? ... %d\n",sudoku_is_correct(g));
              sudoku_print_errors(g);
        	}

      }else{
    	 g=sudoku_read();
     	 sudoku_print(stderr,g);
   	     g=sudoku_solve(g,0,0);
   	     fprintf(stderr,"The puzzle has unique solution ? ... %d\n",sudoku_solution_is_unique(g));
   	     sudoku_print(stdout,g);
      }
        
   return 0;
}
