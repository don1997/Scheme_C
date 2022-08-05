#include "lisp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

object *symbol_table; 
object *false;        
object *true;         
object *empty_list;
object *quote_symbol;


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

object *make_char_literal(char a){
    
    object *obj;

    obj = make_obj();
    obj->type = CHAR_LITERAL;
    obj->data.character_literal.value = a;
    
    return obj;

}

object *make_string(char *a){
    
    object *obj;

    obj = make_obj();
    obj->type = STRING;
    obj->data.string_val.value = malloc(strlen(a) + 1);
    if(obj->data.string_val.value == NULL){
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    strcpy(obj->data.string_val.value, a);
    return obj;
}

object *cons(object *car, object *cdr){
    object *obj;

    obj = make_obj();
    obj->type = PAIR;
    
    obj->data.pair.car = car;
    obj->data.pair.cdr = cdr;
    return obj;
}

char isEmptyList(object *obj);
object *make_symbol(char *a){
    object *obj;

    object *element;

    element = symbol_table;
    
    while(!isEmptyList(element)){
        if(strcmp(car(element)->data.symbol.value, a) == 0){
            return car(element);
        }
        
        element = cdr(element);
    }

    //create symbol and add it to the symbol table
    obj = make_obj();
    obj->type = SYMBOL;
    obj->data.symbol.value = malloc(strlen(a) + 1);

    if(obj->data.symbol.value == NULL){
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    strcpy(obj->data.symbol.value, a);

    symbol_table = cons(obj, symbol_table);
    return obj;
//////////////////////
///
///

/*
    object *obj;
    object *element;
    
    element = symbol_table;
    while (!isEmptyList(element)) {
        if (strcmp(car(element)->data.symbol.value, value) == 0) {
            return car(element);
        }
        element = cdr(element);
    };
    
    obj = make_obj();
    obj->type = SYMBOL;
    obj->data.symbol.value = malloc(strlen(value) + 1);
    if (obj->data.symbol.value == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    strcpy(obj->data.symbol.value, value);
    symbol_table = cons(obj, symbol_table);
    return obj;
*/
}

