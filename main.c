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
    define_symbol = make_symbol("define");
    set_symbol = make_symbol("set!");
    ok_symbol = make_symbol("ok");
    if_symbol = make_symbol("if");

    lambda_symbol = make_symbol("lambda");
    the_empty_environment = empty_list;

    global_environment = setup_environment();

    #define add_procedure(scheme_name, c_name)  \
            define_variable(make_symbol(scheme_name),    \
                          make_primitive_proc(c_name), \
                          global_environment);

    add_procedure("null?", is_null_proc);
     add_procedure("boolean?"  , is_boolean_proc);
    add_procedure("symbol?"   , is_symbol_proc);
    add_procedure("integer?"  , is_integer_proc);
    add_procedure("char?"     , is_char_proc);
    add_procedure("string?"   , is_string_proc);
    add_procedure("pair?"     , is_pair_proc);
    add_procedure("procedure?", is_procedure_proc);
    
    add_procedure("char->integer" , char_to_integer_proc);
    add_procedure("integer->char" , integer_to_char_proc);
    add_procedure("number->string", number_to_string_proc);
    add_procedure("string->number", string_to_number_proc);
    add_procedure("symbol->string", symbol_to_string_proc);
    add_procedure("string->symbol", string_to_symbol_proc);
      
    add_procedure("+"        , add_proc);
    add_procedure("-"        , sub_proc);
    add_procedure("*"        , mul_proc);
    add_procedure("/" , quotient_proc);
    add_procedure("remainder", remainder_proc);
    add_procedure("="        , is_number_equal_proc);
    add_procedure("<"        , is_less_than_proc);
    add_procedure(">"        , is_greater_than_proc);

    add_procedure("cons"    , cons_proc);
    add_procedure("car"     , car_proc);
    add_procedure("cdr"     , cdr_proc);
    add_procedure("set-car!", set_car_proc);
    add_procedure("set-cdr!", set_cdr_proc);
    add_procedure("list"    , list_proc);

    add_procedure("eq?", is_eq_proc);


}


int main(void)
{
    
    printf("Welcome to the interpreter! Press ctrl+c to exit\n");
    
    Initenv();

    while(1){

        printf("> ");
        write(eval(read(), global_environment));
        printf("\n");
    }
   
    return 0;
}
