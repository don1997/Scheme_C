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

//global 
object *false;
object *true;

void Initenv(void){

    false = make_obj();
    false->type = BOOL;
    false->data.bool.value = 0;

    true = make_obj();
    true->type = BOOL;
    true->data.bool.value = 1;
}


/*      READ        */

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

int isdelim(int a){
    
    return isspace(a) || a == EOF;

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

object *read(void){
   
    int num = 0;
    eat_whitespace();
    int a = getchar();
    
    //reads number
    if(isdigit(a)){
        
        ungetc(a, stdin);

        while(isdigit(a = getchar()))
            num =  (num * 10) + (a - '0');

        return makeNum(num);    
    }
    
    else if(a == '#'){

        a = getchar();
        
        switch(a){

            case 't':
                return true;
            case 'f':
                return false;
            default:
                fprintf(stderr, "ERROR: UNKNOWN BOOL\n");
                exit(1);
        }
    }    

    return make_error(); 

}


object *eval(object *exp){
    
    return exp;

}

/*      WRITE       */
void write(object *obj){
    switch(obj->type){
        
        case BOOL:
            printf("%c", isFalse(obj) ? 'f' : 't');   
            break;
        case NUMBER:
            printf("%d", obj->data.number.value);
            break;
        case ERROR:
            printf("ERROR\n");
            exit(1); 
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
