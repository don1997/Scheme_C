#include "lisp.h"  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


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
        
        case COMPOUND_PROC:
        case PRIMITIVE_PROC:
            printf("#<procedure>");
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
