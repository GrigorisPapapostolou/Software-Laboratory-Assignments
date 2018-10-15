#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "symtable.h"
#include <stdio.h>
/*******************************************************************************************/
struct Binding{
   const char *Key;      /*String pou xaraktirizei to binding*/ 
   const void *Value;    /*Data pou antistoixoun to sigkekrimeno key/string*/
  struct Binding *nextBinding;  
};

/*******************************************************************************************/
struct SymTable{
  int length;                          /*# of bindings*/
  struct Binding* FirstBinding;        /* ptr to the first Binding*/
};

/*******************************************************************************************/

/* Return a new SymTable object or NULL if insufficient memory */
SymTable_T SymTable_new(void){
  SymTable_T oSymTable=(SymTable_T)malloc(sizeof(struct SymTable));

  if(oSymTable==NULL)
    return NULL;
  
  oSymTable->length=0;
  oSymTable->FirstBinding=NULL;

  return oSymTable;
}

/*******************************************************************************************/

/*The SymTable_free function should release all the memory used by the oSymTable.
If oSymTable is NULL, then the function does not do anything.*/
void SymTable_free(SymTable_T oSymTable){
  struct Binding* NowBinding;
  struct Binding* NextBinding;
  assert(oSymTable);
  
  NowBinding=oSymTable->FirstBinding;
  NextBinding=NULL;
  while(NowBinding!=NULL){
      NextBinding=NowBinding->nextBinding;
      free((char*)NowBinding->Key);
      free(NowBinding);
      NowBinding=NextBinding;  
  }
  free(oSymTable);
}

/*******************************************************************************************/

/* Return the number of bindings in oSymTable */
int SymTable_getLength(SymTable_T oSymTable){
  assert(oSymTable!=NULL);
  return oSymTable->length;
}

/*******************************************************************************************/

/*The SymTable_contains function should:
      if(the oSymTable contains a binding whose key=pcKey): return 1 (TRUE)
      otherwise                                           : return 0 (FALSE)  

 It is controlled runtime error (checked runtime error) if oSymTable or pcKey is NULL.*/
int SymTable_contains(SymTable_T oSymTable,const char *pcKey){
  struct Binding* NowBinding;

  assert(oSymTable!=NULL);
  assert(pcKey!=NULL);
  
  for(NowBinding=oSymTable->FirstBinding;NowBinding!=NULL;NowBinding=NowBinding->nextBinding){
       if(strcmp(NowBinding->Key,pcKey)==0)
        return 1;
  }
  return 0;
}


/*******************************************************************************************/

/*The function SymTable_put must:  
if(there is no binding key=pcKey in oSymTable) :
      add a new binding to oSymTable which 
      composed of key=pcKey and pvValue=value  and returns 1 (TRUE). 
Otherwise: 
       the function should not change the oSymTable
       and must return 0 (FALSE). 

It is controlled runtime error (checked runtime error) if oSymTable or pcKey is NULL.*/
int SymTable_put(SymTable_T oSymTable,const char *pcKey,const void *pvValue){
  struct Binding* NewBinding;

  assert(oSymTable!=NULL);
  assert(pcKey!=NULL);
  printf("The Key is:%s and the Value is : %d\n",pcKey,*(int*)pvValue);
  if(SymTable_contains(oSymTable,pcKey))
    return 0;

  NewBinding=(struct Binding*)malloc(sizeof(struct Binding)); 
  if(NewBinding==NULL)
    return 0;
 
  NewBinding->Key=(const char*)malloc(strlen(pcKey)+1);
  if(NewBinding->Key==NULL){
    free(NewBinding);
    return 0;
  }

  /*Initialize new Binding:*/
  strcpy((char*)NewBinding->Key,pcKey);
  NewBinding->Value=pvValue;

  /*Conect the new Binding to the list*/
  NewBinding->nextBinding=oSymTable->FirstBinding;
  oSymTable->FirstBinding=NewBinding;

  oSymTable->length++;
  return 1;
}

/*******************************************************************************************/

/*The SymTable function remove must check:
     if(there is a binding key=pcKey in oSymTable) :
          remove the binding from oSymTable and finally returns 1 (TRUE). 
     Otherwise                                     :
          the function should not change the oSymTable and must return 0 (FALSE).

It is controlled runtime error (checked runtime error) if oSymTable or pcKey is NULL.*/
int SymTable_remove(SymTable_T oSymTable,const char *pcKey){
  struct Binding* SearchBinding;
  struct Binding* PrevBinding;
 
  assert(oSymTable!=NULL);
  assert(pcKey!=NULL);

  SearchBinding=oSymTable->FirstBinding;
  PrevBinding=NULL;
  
  while(SearchBinding!=NULL){
     if(strcmp(SearchBinding->Key,pcKey)==0){
        if(PrevBinding==NULL){
          oSymTable->FirstBinding=SearchBinding->nextBinding;
        }else{
          PrevBinding->nextBinding=SearchBinding->nextBinding;
        }
        free((char*)SearchBinding->Key);    
        free(SearchBinding);
        oSymTable->length--;
        return 1;
     }
    
    PrevBinding=SearchBinding;
    SearchBinding=SearchBinding->nextBinding;
  }
  return 0;
}

/*******************************************************************************************/

/*The SymTable_t function get should:
      if(there is a binding key=pcKey in oSymTable) :  
          return the value of the binding 
      Otherwise                                     :
          return NULL if there is no such binding. 

It is controlled runtime error (checked runtime error) if oSymTable or pcKey is NULL.*/

void* SymTable_get(SymTable_T oSymTable,const char *pcKey){
  struct Binding* NowBinding;

  assert(oSymTable!=NULL); 
  assert(pcKey!=NULL);

  for(NowBinding=oSymTable->FirstBinding;
      NowBinding!=NULL; NowBinding=NowBinding->nextBinding){
        if(strcmp(NowBinding->Key,pcKey)==0){
          return (void *)NowBinding->Value;
        }
  }
  return NULL;
}
/*******************************************************************************************/

/*The function SymTable map must implement the function *pfApply every binding in oSymTable, passing pvExtra as additional parameter.
 It is controlled runtime error (checked runtime error) if oSymTable or pfApply is NULL.*/

void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
                  const void *pvExtra)
{
  struct Binding* NowBinding;

  assert(oSymTable != NULL);
  assert(pfApply != NULL);
  assert(pvExtra != NULL);

  for (NowBinding= oSymTable->FirstBinding;
       NowBinding != NULL;
        NowBinding = NowBinding->nextBinding)
    (*pfApply)(NowBinding->Key,(void*)NowBinding->Value,(void *)pvExtra);
}

/*******************************************************************************************/

/*Print all the Bindings */
/*Suppose that the values  are integers*/

void Print_SymTable(SymTable_T oSymTable){
  struct Binding* NowBinding;
  assert(oSymTable!=NULL);


  NowBinding=oSymTable->FirstBinding;
  while(NowBinding!=NULL){
    printf("Key:%s, Value:%d \n",NowBinding->Key,*(int*)NowBinding->Value);
    NowBinding=NowBinding->nextBinding;
  }
}