#include "lisp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

object *car(object *pair){
    return pair->data.pair.car;
}

void set_car(object *obj, object *value){
    obj->data.pair.car = value;
}

object *cdr(object *pair){
    return pair->data.pair.cdr;
}

void set_cdr(object *obj, object *value){
    obj->data.pair.cdr =  value;
}



void Initenv(void){

    empty_list = make_obj();     
    empty_list->type = EMPTYLIST;

    false = make_obj();     
    false->type = BOOL;
    false->data.boolean.value = 0;

    true = make_obj();     
    true->type = BOOL;
    true->data.boolean.value = 1;
    
    symbol_table = empty_list;
    
    quote_symbol = make_symbol("quote");  
}


int main(void)
{
    
    printf("Welcome to the interpreter! Press ctrl+c to exit\n");
    
    Initenv();

    while(1){

        printf("> ");
        write(eval(read()));
        printf("\n");
    }
   
    return 0;
}
