/*****
* Author: Grigoris Papapapostolou
* File  : symtable.h
* Usage : Ιmplementation of a Table Symbols as ADT
******/

#ifndef SYMTABLE
#define SYMTABLE

/* A SymTable_T object is an unordered collection of key-value bindings.*/
typedef struct SymTable* SymTable_T;

/* The SymTable_new function returns a new SymTable_T which is empty or NULL if insufficient memory */
SymTable_T SymTable_new(void);

/*The SymTable_free function should release all the memory used by the oSymTable.
If oSymTable is NULL, then the function does not do anything.*/
void SymTable_free(SymTable_T oSymTable);

/*The function SymTable_getLength must return the number of bindings in oSymTable.
It is controlled wrong program runtime (checked runtime error) the oSymTable be NULL.*/
int SymTable_getLength(SymTable_T oSymTable);

/*The function SymTable_put must, if there is no binding key=pcKey in oSymTable, 
add a new binding to oSymTable which composed of key=pcKey and pvValue=value, and returns 1 (TRUE). 
Otherwise, the function should not change the oSymTable,and must return 0 (FALSE). 
It is controlled runtime error (checked runtime error) if oSymTable or pcKey is NULL.*/
int SymTable_put(SymTable_T oSymTable,const char *pcKey,const void *pvValue);

/*The SymTable function must check if there is a binding key pcKey in oSymTable,then to remove the binding from oSymTable 
and finally returns 1 (TRUE). Otherwise, the function should not change the oSymTable, and must return 0 (FALSE). 
It is controlled runtime error (checked runtime error) if oSymTable or pcKey is NULL.*/
int SymTable_remove(SymTable_T oSymTable,const char *pcKey);

/*The SymTable contains function should return 1 (TRUE) if the oSymTable contains a binding whose 
key is pcKey, and 0 (FALSE) otherwise. It is controlled runtime error (checked runtime error) if oSymTable or pcKey is NULL.*/
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*The SymTable_t function should return the value of the binding oSymTable whose key is pcKey, or NULL, 
if there is no such binding. It is controlled runtime error (checked runtime error) if oSymTable or pcKey is NULL.*/
void* SymTable_get(SymTable_T oSymTable,const char *pcKey);

/*The function SymTable map must implement the function * pfApply every binding in oSymTable, passing pvExtra as additional parameter.
 It is controlled runtime error (checked runtime error) if oSymTable or pfApply is NULL.*/
void  SymTable_map(SymTable_T oSymTable,void (*pfApply)(const char *pcKey,void *pvValue,void *pvExtra),const void *pvExtra); 

/*Print the Bindings of SymTable*/
void Print_SymTable(SymTable_T oSymTable);
#endif
