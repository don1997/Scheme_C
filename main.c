#include <stdio.h>
#include "lisp.h"
#include <stdlib.h>
#include <string.h>


/*      DATA        */

//make number
object *atom(char  *atom_name){
    atom_object *ptr;
    char *name;
    size_t name_len;

    name_len = strlen(atom_name) + 1;//with '\0'
    
    ptr = (atom_object*)malloc(sizeof(atom_object));
    if (ptr == NULL){
        printf("ERROR");
        exit(1);
    }
    

    ptr->type = ATOM;
    name = (char*)malloc(name_len);
    if(name == NULL){
        printf("ERROR");
        exit(1);
    }

    strncpy(name, atom_name, name_len);
    ptr->name = name;
    printf("name: %s", name); 
    return (object*)ptr;
}



/*      READ        */

//peek at next byte
object *next_token(int c){
   
    char buffer[1024];
    int ch, index;

    ch = getchar();

    if(ch == ')' || ch == '(')
       return atom(")");
    while(ch != ')'){
        buffer[index++] = c;
        ch = getchar();
    }
    
    buffer[index++] = '\0';
    if(ch == ')')
        ungetc(c, stdin);
    
    return atom(buffer);
}


object *read(int in){
    
    object *token = next_token(in);
    
   // if(strcmp(name(token), "(") == 0)
     //   return read_tail(in);
    //else
        return token;
}




/*      EVAL        */





/*      WRITE       */



int main(void)
{

    while(1){

        int a = getchar();

        read(a);

        printf("%d", a);
        
        
        //write(eval(read(c)));
    }


    return 0;
}
