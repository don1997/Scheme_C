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
int isdelim(int a){
    
    return isspace(a) || a == EOF;

}

object *read(void){
   
    int num = 0;

    int a = getchar();
    
    //reads number
    if(isdigit(a)){
        
        ungetc(a, stdin);

        while(isdigit(a = getchar()))
            num =  (num * 10) + (a - '0');

    
        return makeNum(num);

    
    }
    
    

    return make_error(); 

}


object *eval(object *exp){
    
    return exp;

}

/*      WRITE       */
void write(object *obj){
    
    if(obj->type == ERROR){
        
        printf("ERROR\n");
        exit(1);
    }

    else{

        printf("%d", obj->data.number.value);
    }

    
}


int main(void)
{
    while(1){

        printf("> ");
        write(eval(read()));
        printf("\n");
    }
   
    return 0;
}
