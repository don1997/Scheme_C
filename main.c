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

    printf("ERROR\n");
    //since its not a digit it must be just an error....
    return make_error(); 
}

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
void write(object *obj){
    
    if(obj->type == ERROR){
        
        printf("ERROR\n");
        exit(1);
    }

    else{

        printf("%d\n", obj->data.number.value);
    }
}


int main(void)
{

//    while(1){
        
        //int a = getchar();

        //read(a);

    write(read());

    //if error
    


        //write(eval(read(c)));
  //  }


    return 0;
}
