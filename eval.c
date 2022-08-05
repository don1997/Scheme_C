#include "lisp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*      EVAL        */

char is_self_eval(object *exp){
    return isBool(exp) || isNum(exp) || isCharLiteral(exp) ||
        isString(exp);
}


char is_tagged_list(object *exp, object *tag){

    object *the_car;

    if(isPair(exp)) {
        the_car = car(exp);
        return isSymbol(the_car) && (the_car == tag);

    }
    return 0;

}

char is_quoted(object *exp){
    return is_tagged_list(exp, quote_symbol);
}

object *text_of_quotation(object *exp){
    return cadr(exp);
}

object *eval(object *exp){

    if(is_self_eval(exp)){
        return exp;
    }


    else if(is_quoted(exp)){
        return text_of_quotation(exp);
    }
    
    else {
        fprintf(stderr, "cannot eval unknown expression type\n");
        exit(1);
    }

}

