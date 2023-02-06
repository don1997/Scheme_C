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

object *if_symbol;
object *lambda_symbol;

void add_binding_to_frame(object *var, object *val, object *frame){
    set_car(frame, cons(var, car(frame)));
    set_cdr(frame, cons(val, cdr(frame)));
    
}


object *make_primitive_proc(
           object *(*fn)(struct object *arguments)) {
    object *obj;

    obj = make_obj();
    obj->type = PRIMITIVE_PROC;
    obj->data.primitive_proc.fn = fn;
    return obj;
}


//PROCS
char is_primitive_proc(object *obj) {
    return obj->type == PRIMITIVE_PROC;
}

object *is_null_proc(object *arguments){
    return isEmptyList(car(arguments)) ? true :false;
}

object *is_boolean_proc(object *arguments){
    return isBool(car(arguments)) ? true : false;
}

object *is_symbol_proc(object *arguments){
    return isNum(car(arguments)) ? true : false;
}

object *is_integer_proc(object *arguments){
  return isNum(car(arguments)) ? true : false;
}

object *is_char_proc(object *arguments){
  return isCharLiteral(car(arguments)) ? true : false;
}

object *is_string_proc(object *arguments){
    return isString(car(arguments)) ? true : false;
}

object *is_pair_proc(object *arguments){
    return isPair(car(arguments)) ? true : false;
}

char is_compound_proc(object *obj);

object *is_procedure_proc(object *arguments){
  object * obj;
  obj = car(arguments);

  return (is_primitive_proc(obj) || 
          is_compound_proc(obj)) ?
          true: false;
}

object *char_to_integer_proc(object *arguments){
    return makeNum((car(arguments))->data.character_literal.value);
}

object *integer_to_char_proc(object *arguments){
    return makeNum((car(arguments))-> data.number.value);
}

object *interger_to_char_proc(object *arguments){
    return make_char_literal((car(arguments))->data.number.value);
}

object *number_to_string_proc(object *arguments){
    char buffer[100];

    sprintf(buffer, "%d", (car(arguments))-> data.number.value);
    return make_string(buffer);
}

object *string_to_number_proc(object *arguments){
    return makeNum(atoi((car(arguments))-> data.string_val.value));
}

object *symbol_to_string_proc(object *arguments){
    return make_string((car(arguments))-> data.symbol.value);
}

object *string_to_symbol_proc(object *arguments){
    return make_symbol((car(arguments))-> data.string_val.value);
}


////MATH PROCS
object *add_proc(object *arguments) {
    long result = 0;
    
    while (!isEmptyList(arguments)) {
        result += (car(arguments))->data.number.value;
        arguments = cdr(arguments);
    }
    return makeNum(result);
}

object *sub_proc(object *arguments){
    long result;

    result = (car(arguments))->data.number.value;
    while(!isEmptyList(arguments = cdr(arguments))){
        result -=(car(arguments))->data.number.value;
    }

    return makeNum(result);

}
object *mul_proc(object *arguments) {
    long result = 1;
    
    while (!isEmptyList(arguments)) {
        result *= (car(arguments))->data.number.value;
        arguments = cdr(arguments);
    }
    return makeNum(result);
}
object *quotient_proc(object *arguments){
    return makeNum(
        ((car(arguments) )-> data.number.value)/
        ((cadr(arguments))->data.number.value));
}

object *remainder_proc(object *arguments){
    return makeNum(
        ((car(arguments))-> data.number.value)%
        ((cadr(arguments))->data.number.value));
    
}

object *is_number_equal_proc(object *arguments){
    long value;

    value = (car(arguments))-> data.number.value;
    while(isEmptyList(arguments = cdr(arguments))){
        if(value != ((car(arguments))->data.number.value)){
            return false;
        }
    }

    return true;
}

//361 line data.c

object *is_less_than_proc(object *arguments){
  int previous;
  int next;

  previous = (car(arguments))->data.number.value;
  while(!isEmptyList(arguments = cdr(arguments))){
    next = (car(arguments))->data.number.value;
    if(previous < next){

    } else {
      return false;
    }
  }

  return true;
}

object *is_greater_than_proc(object *arguments){
  int previous;
  int next;

  previous = (car(arguments))->data.number.value;
  while(!isEmptyList(arguments = cdr(arguments))){
    next = (car(arguments))->data.number.value;
    if(previous > next){

    } else {
      return false;
    }
  }

  return true;
}

object *cons_proc(object *arguments){
  return cons(car(arguments), cadr(arguments));
}

object *car_proc(object *arguments) {
    return caar(arguments);
}

object *cdr_proc(object *arguments){
  return cdar(arguments);
}

object *set_car_proc(object *arguments){
  set_car(car(arguments), cadr(arguments));
  return ok_symbol;
}
object *set_cdr_proc(object *arguments){
  set_cdr(car(arguments), cadr(arguments));
  return ok_symbol;
}
object *list_proc(object *arguments){
  return arguments;
}

object *is_eq_proc(object *arguments){
  object *obj1;
  object *obj2;

  obj1  = car(arguments);
  obj2 = cadr(arguments);

  if(obj1->type != obj2->type){
    return false;
  }

  switch(obj1->type){
    case NUMBER:
      return(obj1->data.number.value ==
            obj2->data.number.value) ?
              true : false;

      break;

    case CHAR_LITERAL:
      return(obj1->data.character_literal.value ==
             obj2->data.character_literal.value) ?
                true: false;
  
      break;

    case STRING:
      return (strcmp(obj1->data.string_val.value,
                    obj2->data.string_val.value) == 0) ?
                      true : false;

      break;
    default: 
      return (obj1 == obj2) ? true : false;
  }
}
//MATH PROCS

object *make_compound_proc(object *parameters, object *body, object *env){

  object *obj;

  obj = make_obj();
  obj-> type = COMPOUND_PROC;
  obj->data.compound_proc.parameters = parameters;
  obj->data.compound_proc.body = body;
  obj->data.compound_proc.env = env;
  return obj;
}

char is_compound_proc(object *obj) {
    return obj->type == COMPOUND_PROC;
}

//PROCS



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

    fprintf(stderr, "unbound variable lookup\n");
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
    fprintf(stderr, "unbound variable set var value\n");
    exit(1);
}

object *setup_environment(void){
    object *initial_env;

    initial_env = extend_environment(empty_list, empty_list, the_empty_environment);

    return initial_env;
}
