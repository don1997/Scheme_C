#include <stdio.h>
#include "lisp.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

///TESTING
int Count = 0;


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

/*      READ        */


int get(void){
    
    int a;

    //helpful link: https://stackoverflow.com/questions/28375031/ignore-whitespace-getchar
    //removes the bug where it would print try to evaluate a new line which would obv. trigger an ERROR statement 
    do {
    a = getchar();
    } while (a == (unsigned char) ' ' || a == (unsigned char) '\t' || a == (unsigned char) '\n');

    return a;
}

/*
int peek(void) { 
    
    char a;

    do {
    a = getchar();
    } while (a == (unsigned char) ' ' || a == (unsigned char) '\t' || a == (unsigned char) '\n');

    //note on using ungetc(c, std); here: If is placed here causes an infinite loop since the byte is placed back onto the stream.
    //but if commented out program will end if enter two or more digit num.
    ungetc(a, stdin); 
    
    return a; 
}
*/
//eat whitespace()

//object *read(void){

    /*
    int a = peek();
    
    a = get();
    //if digit then...
    
    if(isdigit(a)){
       
        return makeNum(a);    

    }

    //since its not a digit it must be just an error....
    return make_error(); 
*/


/*
    int a = get();
    int num = 0;

    if(isdigit(a)){
        ungetc(a, stdin);

        while(isdigit(a = get())){
             num = (num * 10) + (a - '0');

        }

        return makeNum(num);
    }

    return make_error();

}
*/

char is_delimiter(int c) {
    return isspace(c) || c == EOF ||
           c == '('   || c == ')' ||
           c == '"'   || c == ';';
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

int peek() {
    int c;

    c = getchar();
    ungetc(c, stdin);
    return c;
}


object *read(void) {
    int c;
    short sign = 1;
    int num = 0;

    eat_whitespace();

    c = getchar();    

    if (isdigit(c)) {
        /* read a fixnum */
        if (c == '-') {
            sign = -1;
        }
     //   else {
        ungetc(c, stdin);
        //}
        while (isdigit(c = getchar())) {
            //num = (num * 10) + (c - '0');
            num = (num * 10)  + (c - '0');
        }
        num *= sign;
        if (is_delimiter(c)) {
           ungetc(c, stdin);
            return makeNum(num);
        }
        else {
            fprintf(stderr, "number not followed by delimiter\n");
            exit(1);
        }
    }
    else {
        fprintf(stderr, "bad input. Unexpected '%c'\n", c);
        exit(1);
    }
    fprintf(stderr, "read illegal state\n");
    exit(1);
}

















/*      EVAL        */
object *eval(object *exp){
    
    return exp;

}

/*      WRITE       */
void write(object *obj){
    
    if(obj->type == ERROR){
        
        printf("ERROR\n");
    }

    else{

        printf("%d", obj->data.number.value);
    }

    //TESTING
    Count++;
    
}


int main(void)
{
    while(1){
   
        write(eval(read()));
        printf("COUNT: %d", Count);
        printf("\n");
    }
   
    return 0;
}
