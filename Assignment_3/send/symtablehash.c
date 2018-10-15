#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "symtable.h"
#include <stdio.h>

#define HASH_MULTIPLIER 65599

/* Constants for the expandable hash table */
enum {BUCKET_COUNT=509,MAX_BUCKET_COUNT=65521};

/* Bucket sizes for expanding hash table*/
static const int BucketSizes[] = {BUCKET_COUNT,1021,2039,4093,8191,16381,32749,MAX_BUCKET_COUNT};

/*******************************************************************************************/
struct Binding{
   const char *Key;      /*String pou xaraktirizei to binding*/ 
   const void *Value;    /*Data pou antistoixoun to sigkekrimeno key/string*/
  struct Binding *nextBinding;  
};

/*******************************************************************************************/
struct SymTable{
  int length;                    /*# of bindings */
  struct Binding** Buckets;     /*FirstBucket is pointer to first Bucket which points to Binding*/ 
  int CountBuckets;             /* #Buckets in SymTable*/
  int IndexAtBucketSizes;
};
/*******************************************************************************************/

/* Return a hash code for pcKey. */
static unsigned int HashFunction(const char *pcKey,int BucketCount){
  int i;
  unsigned int uiHash = 0U;

  assert(pcKey!=NULL);
  assert(BucketCount!=0);

  for (i = 0; pcKey[i] != '\0'; i++)
    uiHash = uiHash * (unsigned int)HASH_MULTIPLIER + (unsigned int)pcKey[i];
  return (int)(uiHash % (unsigned int)BucketCount);
  
} 

/*******************************************************************************************/
/* Return a new SymTable object or NULL if insufficient memory */
SymTable_T SymTable_new(void){
  SymTable_T oSymTable=(SymTable_T)malloc(sizeof(struct SymTable));
  struct Binding**Buckets=(struct Binding**)calloc(BUCKET_COUNT,sizeof(struct Binding*));

  if(oSymTable==NULL || Buckets==NULL)
    return NULL;

  oSymTable->length=0;
  oSymTable->IndexAtBucketSizes=0;
  oSymTable->CountBuckets=BucketSizes[oSymTable->IndexAtBucketSizes];
  oSymTable->Buckets=Buckets;
  
  return oSymTable;
}

/*******************************************************************************************/
/*The SymTable_free function should release all the memory used by the oSymTable.
If oSymTable is NULL, then the function does not do anything.*/
void SymTable_free(SymTable_T oSymTable){
  struct Binding* NowBinding;
  struct Binding* NextBinding;
  int i;

  assert(oSymTable);
  
   
   for(i=0;i<oSymTable->CountBuckets;i++){
   	   for(NowBinding=oSymTable->Buckets[i];NowBinding!=NULL;NowBinding=NextBinding){
   	   	      NextBinding=NowBinding->nextBinding;
   	   	      free((char*)NowBinding->Key);
   	   	      free(NowBinding);
   	   }
   }
   free(oSymTable->Buckets);
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
  int hash;

  assert(oSymTable!=NULL);
  assert(pcKey!=NULL);
  
  hash=HashFunction(pcKey,oSymTable->CountBuckets);
  for(NowBinding=oSymTable->Buckets[hash];NowBinding!=NULL;NowBinding=NowBinding->nextBinding){
       if(strcmp(pcKey,NowBinding->Key)==0)
        return 1;
  }
  return 0;
}

/*******************************************************************************************/

int Expand(SymTable_T oSymTable){
	int i,hash,NewBucketsSize;
	struct Binding** NewBuckets;
	struct Binding* NowBinding;
  struct Binding* NextBinding;

    assert(oSymTable!=NULL);
	  oSymTable->IndexAtBucketSizes++;
    NewBucketsSize=BucketSizes[oSymTable->IndexAtBucketSizes];

    NewBuckets=(struct Binding**) malloc(NewBucketsSize*sizeof(struct Binding*));
    if(NewBuckets==NULL) return 0;

    for(i=0;i < oSymTable->CountBuckets;i++){
    	for(NowBinding=oSymTable->Buckets[i],NextBinding=NULL;
    		NowBinding!=NULL;NowBinding=NextBinding)
    	{
              hash=HashFunction(NowBinding->Key,NewBucketsSize);
              NextBinding=NowBinding->nextBinding;
              NowBinding->nextBinding=NewBuckets[hash];
              NewBuckets[hash]=NowBinding;                                               
    	}
    }
    free(oSymTable->Buckets);
    oSymTable->Buckets=NewBuckets;
    oSymTable->CountBuckets=NewBucketsSize;
    return 1;
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
  int hash;

  assert(oSymTable!=NULL);
  assert(pcKey!=NULL);

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

  /*Check if the array of Bucket is full => EXPAND*/
  if((oSymTable->CountBuckets < oSymTable->length+1) && (oSymTable->CountBuckets < MAX_BUCKET_COUNT))
         Expand(oSymTable);

  /*Conect the new Binding to the "RIGHT" list*/
  hash=HashFunction(pcKey,oSymTable->CountBuckets);
  NewBinding->nextBinding=oSymTable->Buckets[hash];
  oSymTable->Buckets[hash]=NewBinding;

  oSymTable->length++;
  return 1;
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
  int hash;
  
  assert(oSymTable!=NULL); 
  assert(pcKey!=NULL);

  hash=HashFunction(pcKey,oSymTable->CountBuckets);
  for(NowBinding=oSymTable->Buckets[hash]; NowBinding!=NULL;NowBinding=NowBinding->nextBinding){
        if(strcmp(NowBinding->Key,pcKey)==0){
          return (void*)NowBinding->Value;
        }
  }
  return NULL;
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
  int hash;

  assert(oSymTable!=NULL);
  assert(pcKey!=NULL);
  
  hash=HashFunction(pcKey,oSymTable->CountBuckets);
  SearchBinding=oSymTable->Buckets[hash];
  PrevBinding=NULL;
  
  while(SearchBinding!=NULL){
     if(strcmp(SearchBinding->Key,pcKey)==0){
        if(PrevBinding==NULL){
          oSymTable->Buckets[hash]=SearchBinding->nextBinding;
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

/*The function SymTable map must implement the function *pfApply every binding in oSymTable, passing pvExtra as additional parameter.
 It is controlled runtime error (checked runtime error) if oSymTable or pfApply is NULL.*/

void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
                  const void *pvExtra)
{
  struct Binding* NowBinding;
  int i; 
  assert(oSymTable != NULL);
  assert(pfApply != NULL);
  assert(pvExtra != NULL);

   for(i=0;i<oSymTable->CountBuckets;i++)
    for (NowBinding= oSymTable->Buckets[i];
         NowBinding != NULL;
         NowBinding = NowBinding->nextBinding)
           (*pfApply)(NowBinding->Key,(void*)NowBinding->Value,(void *)pvExtra);
}


/*******************************************************************************************/

/*Print all the Bindings */
/*Suppose that the values  are integers*/

void Print_SymTable(SymTable_T oSymTable){
  struct Binding* NowBinding;
  int i;
  assert(oSymTable!=NULL);
  
  printf("The Bindings of SymTable are: \n");
  for(i=0;i<oSymTable->CountBuckets;i++)
   for(NowBinding=oSymTable->Buckets[i];NowBinding!=NULL;NowBinding=NowBinding->nextBinding){
    printf("Key:%s, Value:%d \n",NowBinding->Key,*(int*)NowBinding->Value);
  }
}