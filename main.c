#include <stdio.h>
#include "lisp.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*      DATA        */

object *make_obj(){

    object *obj;

    obj = malloc(sizeof(object));

    if(obj == NULL){
        printf("ERROR\n");
        exit(1);
    }

    return obj;
}

object *makeNum(int a){

    object *obj;

    obj = make_obj();
    obj->type = NUMBER;
    obj->data.number.value = a;
    
    return obj;

}

object *make_error(){

    object *obj;

    obj = make_obj();
    obj->type = ERROR;

    return obj;

}

/*      READ        */

int peek(void) { 
    
    char c = getchar(); 
    
    ungetc(c, stdin); 
    
    return c; 
}


object *read(void){

    int a = peek();
    //if digit then...
    if(isdigit(a)){
        
        return makeNum(a); 
    
        


    }

    //since its not a digit it must be just an error....
    return make_error(); 
}

/*      EVAL        */
object *eval(object *exp){
    
    return exp;

}

/*      WRITE       */
int write(object *obj){
    
    if(obj->type == ERROR){
        
        printf("ERROR\n");
        exit(1);
    }

    else{

        printf("%d\n", obj->data.number.value);
    }

    return 1;
}




int main(void)
{
    int a = 0;

    do{
        
        printf("> ");
        a = write(eval(read()));
        printf("\n");
    
    }while(a == 1);

    return 0;
}
