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


int get(void){
    
    int a;

    //helpful link: https://stackoverflow.com/questions/28375031/ignore-whitespace-getchar
    //removes the bug where it would print try to evaluate a new line which would obv. trigger an ERROR statement 
    do {
    a = getchar();
    } while (a == (unsigned char) ' ' || a == (unsigned char) '\t' || a == (unsigned char) '\n');

    return a;
}

int peek(void) { 
    
    char a;

    do {
    a = getchar();
    } while (a == (unsigned char) ' ' || a == (unsigned char) '\t' || a == (unsigned char) '\n');

    //note on using ungetc(c, std); here: If is placed here causes an infinite loop since the byte is placed back onto the stream.
    //but if commented out program will end if enter two or more digit num.
    ungetc(a, stdin); 
    
    return a; 
}

//eat whitespace()

object *read(void){

    int a = peek();
    
    a = get();
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
void write(object *obj){
    
    if(obj->type == ERROR){
        
        printf("ERROR\n");
        //exit(1);
      //  return 1;
    }

    else{

        printf("%d\n", obj->data.number.value);
    }


    //return 0;
}




int main(void)
{
    /*
    int quit = 0;
    int choice = 0;

    do{
        switch(choice){
            case 0: 
                choice = write(eval(read())); 
                break;

            case 1: 
                printf("ERROR\n");
                quit = 1;
                break;
        }
    }while (quit == 0);
*/
/*

    do{
        
        printf("> ");
        a = write(eval(read()));
        printf("\n");
    
    }while(a == 1);
*/
/*
    while(write(eval(read())) == 0){
        
        write(eval(read()));
    }
  */
    while(1){
    write(eval(read()));
    };
    return 0;
}
