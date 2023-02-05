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

object *global_environment;

object *define_symbol;
object *set_symbol;
object *ok_symbol;
object *the_empty_environment;

void add_binding_to_frame(object *var, object *val, object *frame){
    set_car(frame, cons(var, car(frame)));
    set_cdr(frame, cons(val, cdr(frame)));
    
}

object *enclosing_environment(object *env){
    return cdr(env);
}
object *first_frame(object *env){
    return car(env);
}

object *frame_variables(object *frame){
    return car(frame);
}

object *frame_values(object *frame){
    return cdr(frame);
}


object *make_frame(object *var, object *val){
    return cons(var, val);
}
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

char isEmptyList(object *obj){
    return obj == empty_list;
}

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
}

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
}
*/


object *extend_environment(object *var, object *values, object *base_env){

    return cons(make_frame(var, values), base_env);
}

void define_variable(object *var, object *value, object *env){
        object *frame;
        object *vars;
        object *vals;

        frame = first_frame(env);
        vars = frame_variables(frame);
        vals = frame_values(frame);

        while(!isEmptyList(vars)){
            if(var == car(vars)){
                set_car(var,value);
                return;
            }

            vars = cdr(vars);
            vals = cdr(vals);
        }
        
        add_binding_to_frame(var,value,frame);

}

object *lookup_variable_value(object *var, object *env){
    object *frame;
    object *vars;
    object *vals;

    while (!isEmptyList(env)) {

        frame = first_frame(env);
        vars = frame_variables(frame);
        vals = frame_values(frame);

        while (!isEmptyList(vars)) {

            if (var == car(vars)) {
                return car(vals);
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }

        env = enclosing_environment(env);
    }

    fprintf(stderr, "unbound variable\n");
    exit(1);
}

void set_variable_value(object *var, object *val, object *env){
    object *frame;
    object *vars;
    object *vals;

    while (!isEmptyList(env)) {
        frame = first_frame(env);
        vars = frame_variables(frame);
        vals = frame_values(frame);
        while (!isEmptyList(vars)) {
            if (var == car(vars)) {
                set_car(vals, val);
                return;
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = enclosing_environment(env);
    }
    fprintf(stderr, "unbound variable\n");
    exit(1);
}

object *setup_environment(void){
    object *initial_env;

    initial_env = extend_environment(empty_list, empty_list, the_empty_environment);

    return initial_env;
}
