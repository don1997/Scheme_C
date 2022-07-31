#include <stdio.h>
#include "lisp.h"
#include <stdlib.h>
#include <string.h>


/*      DATA        */


//make number
/*      READ        */

//peek at next byte
/*
object *next_token(int c){
   
    char buffer[1024];
    int ch, index;

    ch = getchar();

    if(ch == ')' || ch == '(')
       return atom(")");
    while(ch != ')'){
        buffer[index++] = c;
        ch = getchar();
    }
    
    buffer[index++] = '\0';
    if(ch == ')')
        ungetc(c, stdin);
    
    return atom(buffer);
}
*/





/*      EVAL        */





/*      WRITE       */



int main(void)
{

    while(1){

        
        
        //write(eval(read(c)));
    }


    return 0;
}
