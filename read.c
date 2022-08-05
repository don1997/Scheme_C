#include "lisp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*      READ        */


//  Selectors   //
char isBool(object *obj){

    return obj->type == BOOL;
}

char isFalse(object *obj){
    return obj == false;
}

char isTrue(object *obj){
    return obj == true;
}

int isNum(object *obj){
    return obj->type == NUMBER;
}

char isCharLiteral(object *obj){
    return obj->type == CHAR_LITERAL;
}

int isdelim(int a){

    return isspace(a) || a == EOF || a == '"' || a == ';' || a == ' ' || a == '(' || a == ')';
}

int isNewLine(int a){
    return a =='\n';
}

char isString(object *obj){

    return obj->type == STRING;
}

char isEmptyList(object *obj){
    return obj->type == EMPTYLIST;
}


char isPair(object *obj){
    return obj->type == PAIR;
}

char isSymbol(object *obj){
    return obj->type == SYMBOL;
}

char is_initial(int c){
    return isalpha(c) || c == '*'|| c == '+' || c == '-' || c == '/' ||
                      c == '>' || c =='<' || c == '=' || c =='?' || c == '!';
                      
}

//  Read() Helpers    //
int peek(){
    int a;

    a = getchar();

    ungetc(a, stdin);

    return a;
}


void expect_delim(){

     if (!isdelim(peek())) {
        fprintf(stderr, "character not followed by delimiter\n");
        exit(1);
    }
}



void eat_expect_string(char *str){

    int c;

    while (*str != '\0') {
        
        c = getchar();

        if (c != *str) {
            fprintf(stderr, "unexpected character '%c'\n", c);
            exit(1);
        }

        str++;
    }

}

void eat_whitespace() {
    int c;
    
    while ((c = getchar()) != EOF) {
        if (isspace(c)) {
            continue;
        }

        else if (c == ';') { /* comments are whitespace also */
            
            while (((c = getchar()) != EOF) && (c != '\n'));
            continue;
        }
        
        ungetc(c, stdin);
        break;
    }
}


object *read_character(void){

    int a;

    a = getchar();

    switch(a){
        case EOF:
                fprintf(stderr, "ERROR: incomplete character literal\n");
                exit(1);
            break;
        case 's':
            if(peek() == 'p'){
                
                eat_expect_string("pace");
                expect_delim();
                return make_char_literal(' ');
            }
            break;
        case 'n':

            if(peek() == 'e'){
                eat_expect_string("ewline");
                expect_delim();
                return make_char_literal('\n');
            }
            break;
    }


    expect_delim();
    return make_char_literal(a);
}

object *read(void);

object *read_pair(){

    int a;

    object *car_obj;
    object *cdr_obj;

    eat_whitespace();

    a = getchar();

    if(a == ')'){
        return empty_list;
    }

    ungetc(a, stdin);

    car_obj = read();

    eat_whitespace();


    a = getchar();
    if(a == '.'){
        a = peek();
        if(!isdelim(a)){
            printf("ERROR\n");
        }

        cdr_obj = read();
        eat_whitespace();
        a = getchar();
        if(a != ')'){
            printf("ERROR\n");
        }

        return cons(car_obj, cdr_obj);
    } else {

    ungetc(a, stdin);
    cdr_obj = read_pair();
    return cons(car_obj, cdr_obj);

    }
}

object *read(void){
   
    int i;

#define BUFFER_MAX 1000

    char buffer[BUFFER_MAX];

    short sign = 1;
    int num = 0;
    eat_whitespace();
    int a = getchar();
   
    /*
    //reads number
    if(isdigit(a)){
         
        ungetc(a, stdin);
        while(isdigit(a = getchar()))
            num =  (num * 10) + (a - '0');

        if(isdelim(a)){
            ungetc(a, stdin);
            return makeNum(num);
        }
    }
    */

    if (isdigit(a) || (a == '-' && (isdigit(peek())))) {
        /* read a fixnum */
        if (a == '-') {
            sign = -1;
        }
        else {
            ungetc(a, stdin);
        }
        while (isdigit(a = getchar())) {
            num = (num * 10) + (a - '0');
        }
        num *= sign;
        if (isdelim(a)) {
            ungetc(a, stdin);
            return makeNum(num);
        }
        else {
            fprintf(stderr, "number not followed by delimiter\n");
            exit(1);
        }
    }
    else if(a == '#'){

        a = getchar();
        
        switch(a){

            case 't':
                return true;
            case 'f':
                return false;
            case '\\':
                return read_character();
            default:
                fprintf(stderr, "ERROR: UNKNOWN BOOL OR CHAR LITERAL\n");
                exit(1);
        }
    }   

    else if(a == '"'){

        i = 0;

        while ((a = getchar()) != '"'){
            if(a == '\\'){
                a = getchar();
                if (a == '\n'){
                    a ='\n'; 
                }
            }

            if(a == EOF){
                fprintf(stderr, "non-terminated string literal\n");
            }

            //subtracts 1 to save space for '\0'

            if(i < BUFFER_MAX - 1){
                buffer[i++] = a;
            }
            else {
                printf("ERROR\n");
            }
        }
        buffer[i] = '\0';
        return make_string(buffer);
    }

    //READ SYMBOL HERE
    else if(is_initial(a) ||
            ((a == '+' || a == '-') &&
            isdelim(peek()))){
        
        i = 0;
        
        while(is_initial(a) || isdigit(a) ||
                a == '+' || a == '-'){
                    if(i < BUFFER_MAX){
                        buffer[i++] = a;
                    } else {
                    fprintf(stderr, "symbol too long. " "maxium length is %d\n", BUFFER_MAX);
                    exit(1);
                    
                }

            a = getchar();
        }
        if(isdelim(a)){
            buffer[i] = '\0';
            ungetc(a, stdin);
            return make_symbol(buffer);
        }
        else {
            fprintf(stderr, "symbol not followed by delim. " "Found '%c'\n", a);
            exit(1);
        }
    }

    else if(a == '('){
        
       return read_pair();
    }
    else if(a == '\''){
        return cons(quote_symbol, cons(read(), empty_list));
    }
    
    else {
        fprintf(stderr, "bad input. Unexpected '%c'\n", a);
        exit(1);
    }
    
    //else
    return make_error(); 

}




