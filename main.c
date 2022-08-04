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

void make_symbol(){

    //INSERT HERE

}


//global 
object *false;
object *true;

//INSERT SYM table 

object *empty_list;

void Initenv(void){

    empty_list = make_obj();
    empty_list->type = EMPTYLIST;

    false = make_obj();
    false->type = BOOL;
    false->data.boolean.value = 0;

    true = make_obj();
    true->type = BOOL;
    true->data.boolean.value = 1;
}


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
    return obj->type = NUMBER;
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


////set cdr and car

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

#define caar(obj)   car(car(obj))
#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))

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
//    else if(){
        
  //  }

    else if(a == '('){
        
       return read_pair();
    }

    
    else {
        fprintf(stderr, "bad input. Unexpected '%c'\n", a);
        exit(1);
    }
    
    //else
    return make_error(); 

}

/*      EVAL        */

object *eval(object *exp){
    
    return exp;

}

/*      WRITE       */


void write(object *obj);

void write_pair(object *pair){

    object *car_obj;
    object *cdr_obj;

    car_obj = car(pair);
    cdr_obj = cdr(pair);

    write(car_obj);
    if(cdr_obj-> type == PAIR){
        printf(" ");
        write_pair(cdr_obj);
    }

    else if(cdr_obj->type == EMPTYLIST){
        return;
    }

    else {
        printf(" . ");
        write(cdr_obj);
    }
}

void write(object *obj){
    char c;
    char *str;

    switch(obj->type){
        

        case BOOL:
            printf("#%c", isFalse(obj) ? 'f' : 't');   
            break;
        case NUMBER:
            printf("%d", obj->data.number.value);
            break;
        case CHAR_LITERAL:

            c = obj->data.character_literal.value;
            
            printf("#\\");

            switch(c){
                case '\n':
                    printf("newline");
                    break;
                case ' ':
                    printf("space");
                    break;
                default:
                    putchar(c);
                    break;
            }

            break;
        case STRING:
            
            str = obj->data.string_val.value;
            
            putchar('"');
            while(*str != '\0'){
                switch(*str){

                    case '\\':
                        printf("\\\\");
                        break;
                    case '\n':
                        printf("\\n");
                        break;
                    case '"' :
                        printf("\\\"");
                        break;
                    default:
                        putchar(*str);
                }
                str++;
            }
            putchar('"');
            break;

        case EMPTYLIST:
            
            printf("()");
            break;

        case PAIR:
            printf("(");
            write_pair(obj);
            printf(")");
            break;
        case SYMBOL:
            printf("%s", obj->data.symbol.value);
            break;
///Note: couldn't get this to work for single characters 
///kept throwing an error not sure what else was in the stream
///only problem is now it doesn't throw any errors with input with numbers
/*
        case ERROR:
               
                if(!isspace(c)){ 
                    printf("C IS: %c\n", c ); 
                    printf("------WRITE ERROR--------\n");
                    exit(1); 
                
                }
            
            break;
*/
        default:
            break;
    }
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
