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


 static int numberOfDigits(int i) {
  int count = 0;
  while (i > 0) {
    i = i / 10;
    count++;
  }
  return count;
}

/* Return a pointer to a string of format ("key%d", i) */
static char *keyWithID(int i) {
  int iNum;
  char *pcKey;

  iNum = numberOfDigits(i);
  pcKey = (char *)malloc((3 + iNum) * sizeof(char) + 1);
  sprintf(pcKey, "%s%d", "csd", i);
  return pcKey;
}


/* Test SymTable_put() with tableSize number of bindings */

static void testPut(SymTable_T oSymTable, char **pcKeys, int *piVals, int tableSize) {
  int i, j;
  for (i = 0; i < tableSize; i++) {
    piVals[i] = i;                                               /*At this point fill the table piVals={ 0,1,2,...,size-1}*/
    pcKeys[i] = keyWithID(i);                                    /*At this point fill the table pcKeys with pointers */  
    j = SymTable_put(oSymTable, pcKeys[i], &piVals[i]);
    assert(j == 1);
  }

  /*check if the EXPECTED_lenght=size is equal with the length of Sym Table*/
  assert(SymTable_getLength(oSymTable) == tableSize);
  
  /*change the value of the last Binding of Sym Table*/
  i -= 1;
  j = SymTable_put(oSymTable, pcKeys[0], &piVals[i]);
  assert(j == 0);
  assert(SymTable_getLength(oSymTable) == tableSize);
}

/* Test SymTable_contains() with tableSize number of bindings */
static void testContains(SymTable_T oSymTable, char **pcKeys, const int *piVals, int tableSize) {
  int i, j;

  for (i = 0; i < tableSize; i++) {
    j = SymTable_contains(oSymTable, pcKeys[i]);      /*Check with the row if the table oSymTable */
    assert(j == 1);                                   /*contains all the keys of the table pcKeys*/
  }

  /* Checks the return value of the function contains with parameters a string("FOOBAR") 
   which is not contained in the table => the j=0*/
  j = SymTable_contains(oSymTable, "FOOBAR");
  assert(j == 0);
}

/* Test SymTable_remove() with tableSize number of bindings */
static void testRemove(SymTable_T oSymTable, char **pcKeys, const int *piVals, int tableSize) {
  int i, j;

  for (i = 0; i < tableSize; i++) {
    j=SymTable_remove(oSymTable, pcKeys[i]);
    assert(j == 1);
    j = SymTable_contains(oSymTable, pcKeys[i]);
    assert(j == 0);
  }
  assert(SymTable_getLength(oSymTable) == 0);
}

/* Test SymTable_map() with tableSize number of bindings */
static void testMap(SymTable_T oSymTable, char **pcKeys, const int *piVals, int tableSize) {
  int i, j, iExtra;
  int *piOldValue;
  void (*pf)(const char *, void *, void *);
  /*Refill the Symbol table*/
  for (i = 0; i < tableSize; i++) {
    j = SymTable_put(oSymTable, pcKeys[i], &piVals[i]);
    assert(j == 1);
  }
  
  assert(SymTable_getLength(oSymTable) == tableSize);
  pf = squarePlusExtra;
  
  Extra = 2;
  
  SymTable_map(oSymTable, pf, &iExtra);
  
  /*for (i = 0; i < tableSize; i++) {
    j = i * i + iExtra;
    piOldValue = (int *)SymTable_get(oSymTable, pcKeys[i]);
    assert(*piOldValue == j);
    assert(piVals[i] == j);
  }*/
}

int main(void){
  int i,tableSize;
  int *piVals;
  char **pcKeys;
  SymTable_T oSymTable;

  printf("Please Insert the Size of SymTable: \n");
  scanf("%d",&tableSize);

  oSymTable = SymTable_new();
  piVals = (int *)malloc(tableSize * sizeof(int));        /*piVals : pinakas pou tha exei mesa tou ola ta VALUES*/
  pcKeys = (char **)malloc(tableSize * sizeof(char *));   /*pcKeys : pinakas pou kathe keli tou exei mia diefthinsi se ena String*/
  

  assert(SymTable_getLength(oSymTable) == 0);

  testPut(oSymTable,pcKeys,piVals,tableSize);
 
  testContains(oSymTable,pcKeys, piVals, tableSize);
  
  testRemove(oSymTable,pcKeys,piVals,tableSize);
 
  testMap(oSymTable,pcKeys, piVals,tableSize);

 /* Print_SymTable(oSymTable);*/

  for (i = 0; i < tableSize; i++)
    free(pcKeys[i]);
  free(pcKeys);
  free(piVals);
  free(pcOffsetKey);
  free(pcExtraKey);
  SymTable_free(oSymTable);
}