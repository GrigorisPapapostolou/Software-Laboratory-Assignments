#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"


/* Square the integer at pvValue and add pvExtra to it                            */
 static void Pithagoreio(const char *pcKey,void *pvValue,void *pvExtra) {
  int val = *(int *)pvValue;
  int extra = *(int *)pvExtra;
  int result =val*val +extra*extra;
  *(int *)pvValue = result;
}



int main(void){
  /*Create a Sym Table*/
  SymTable_T oSymTable=SymTable_new();
  if(oSymTable==NULL){
    printf("Error: return NULL pointer from SymTable_new \n");
    exit(0);
  }
  
  /*Full Sym Table with SymTable_put:*/
  SymTable_put(oSymTable,"Radiohead",0);
  /*SymTable_put(oSymTable,"Pink Floyd",1);
  SymTable_put(oSymTable,"Aerosmith",2);
  SymTable_put(oSymTable,"ACDC",3);
  SymTable_put(oSymTable,"Led Zeppelin",4);
  SymTable_put(oSymTable,"The Beatles",5);
  SymTable_put(oSymTable,"Black Sabbath",6);
  SymTable_put(oSymTable,"Queen",7);
  SymTable_put(oSymTable,"The Doors",8);
  SymTable_put(oSymTable,"R.E.M.",9);
  SymTable_put(oSymTable,"KISS",10);
*/
  /*Print Sym_Table*/
  Print_SymTable(oSymTable);

   /*Test SymTable_getLength:*/
    printf("\nThe length of SymTable:%d \n\n",SymTable_getLength(oSymTable));

   /*Test SymTable_contains:*/
   printf("The SymTable contains the Key=The Doors : %d\n",SymTable_contains(oSymTable,"The Doors"));
   printf("The SymTable contains the Key=WhiteStrips: %d\n\n",SymTable_contains(oSymTable,"WhiteStrips"));

   /*Test  SymTable_get:*/
    printf("The value of Aerosmith is: %d \n", *(int *)SymTable_get(oSymTable,"Aerosmith"));
    printf("The value of KISS is: %d \n\n", *(int *)SymTable_get(oSymTable,"KISS"));
   
   /*Test SymTable_remove:*/
    SymTable_remove(oSymTable,"R.E.M.");
    SymTable_remove(oSymTable,"Queen");

  /*Print Sym_Table:*/
  Print_SymTable(oSymTable);
  
  /*Test SymTable_map:*/
   SymTable_map(oSymTable,Pithagoreio,(void *)2);

   /*Test Free:*/
   SymTable_free(oSymTable);

  return 0;
}