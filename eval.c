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

char is_variable(object *expression) {
    return isSymbol(expression);
}
char is_false(object *obj) {
    return obj == false;
}
char is_true(object *obj) {
    return !is_false(obj);
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

////

char is_assignment(object *exp) {
    return is_tagged_list(exp, set_symbol);
}

object *assignment_variable(object *exp) {
    return car(cdr(exp));
}

object *assignment_value(object *exp) {
    return car(cdr(cdr(exp)));
}

char is_definition(object *exp) {
    return is_tagged_list(exp, define_symbol);
}

object *definition_variable(object *exp) {
    return cadr(exp);
}

object *make_lambda(object *parameters, object *body);


object *definition_value(object *exp) {
  if(isSymbol(cadr(exp))){
    return cadr(exp);
  } else {
    return make_lambda(cdadr(exp), cddr(exp));
  }

}

char is_if(object *expression) {
    return is_tagged_list(expression, if_symbol);
}

object *if_predicate(object *exp) {
    return cadr(exp);
}

object *if_consequent(object *exp) {
    return caddr(exp);
}

object *if_alternative(object *exp) {
    if (isEmptyList(cdddr(exp))) {
        return false;
    }
    else {
        return cadddr(exp);
    }
}
//lambdas
//
object *make_lambda(object *parameters, object *body){
  return cons(lambda_symbol, cons(parameters, body));
}

char is_lambda(object *exp){
  return is_tagged_list(exp, lambda_symbol);
}

object *lambda_parameters(object *exp){
  return cadr(exp);
}

object *lambda_body(object *exp){
  return cddr(exp);
}
object *first_exp(object *seq) {
    return car(seq);
}

object *rest_exps(object *seq) {
    return cdr(seq);
}

//lambdas



char is_application(object *exp) {
    return isPair(exp);
}

object *operator(object *exp) {
    return car(exp);
}

object *operands(object *exp) {
    return cdr(exp);
}

char is_no_operands(object *ops) {
    return isEmptyList(ops);
}

object *first_operand(object *ops) {
    return car(ops);
}

object *rest_operands(object *ops) {
    return cdr(ops);
}

object *eval(object *exp, object *env);

object *list_of_values(object *exps, object *env) {
    if (is_no_operands(exps)) {
        return empty_list;
    }
    else {
        return cons(eval(first_operand(exps), env),
                    list_of_values(rest_operands(exps), env));
    }
}

object *eval_assignment(object *exp, object *env) {
    set_variable_value(assignment_variable(exp),
                       eval(assignment_value(exp), env),
                       env);
    return ok_symbol;
}

object *eval_definition(object *exp, object *env) {
    define_variable(definition_variable(exp),
                    eval(definition_value(exp), env),
                    env);
    return ok_symbol;
}


/////
///

//put with other selectors 
char is_last_exp(object *seq) {
    return isEmptyList(cdr(seq));
}

object *eval(object *exp, object *env) {
    object *procedure;
    object *arguments;

tailcall:
    if (is_self_eval(exp)) {
        return exp;
    }
    else if (is_variable(exp)) {
        return lookup_variable_value(exp, env);
    }
    else if (is_quoted(exp)) {
        return text_of_quotation(exp);
    }
    else if (is_assignment(exp)) {
        return eval_assignment(exp, env);
    }
    else if (is_definition(exp)) {
        return eval_definition(exp, env);
    }
    else if (is_if(exp)) {
        exp = is_true(eval(if_predicate(exp), env)) ?
                  if_consequent(exp) :
                  if_alternative(exp);
        goto tailcall;
    }
    else if (is_lambda(exp)) {
        return make_compound_proc(lambda_parameters(exp),
                                  lambda_body(exp),
                                  env);
    }
    else if (is_application(exp)) {
        procedure = eval(operator(exp), env);
        arguments = list_of_values(operands(exp), env);
        if (is_primitive_proc(procedure)) {
            return (procedure->data.primitive_proc.fn)(arguments);
        }
        else if (is_compound_proc(procedure)) {
            env = extend_environment( 
                       procedure->data.compound_proc.parameters,
                       arguments,
                       procedure->data.compound_proc.env);
            exp = procedure->data.compound_proc.body;
            while (!is_last_exp(exp)) {
                eval(first_exp(exp), env);
                exp = rest_exps(exp);
            }
            exp = first_exp(exp);
            goto tailcall;
        }
        else {
            fprintf(stderr, "unknown procedure type\n");
            exit(1);
        }
    }
    else {
        fprintf(stderr, "cannot eval unknown expression type\n");
        exit(1);
    }
    fprintf(stderr, "eval illegal state\n");
    exit(1);
}

